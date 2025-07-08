#include "python_manager.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <QFileInfo>
#include <QStandardPaths>

// Undefine slots before including Python headers to avoid conflicts
#ifdef slots
#undef slots
#endif

// Include Python headers
#include <Python.h>

// Redefine slots for Qt after Python headers
#ifndef slots
#define slots Q_SLOTS
#endif

PythonManager::PythonManager(QObject *parent) 
    : QObject(parent), m_initialized(false) {
}

PythonManager::~PythonManager() {
    cleanup();
}

bool PythonManager::initialize() {
    if (m_initialized) {
        return true;
    }
    
    qDebug() << "Initializing Python interpreter...";
    
    try {
        // Check if Python is already initialized
        if (!Py_IsInitialized()) {
            // Configure Python using modern initialization API
            if (!initializePythonModern()) {
                qDebug() << "Failed to initialize Python with modern API";
                return false;
            }
            
            qDebug() << "Python interpreter initialized successfully";
        }
        
        // For Python 3.7+, GIL is automatically initialized
        // No need for deprecated PyEval_InitThreads()
        qDebug() << "GIL is automatically initialized in Python 3.7+";
        
        // Test Python initialization immediately
        qDebug() << "Testing Python initialization state...";
        qDebug() << "Py_IsInitialized():" << Py_IsInitialized();
        
        // Set up sys.path to include current directory and common Python paths
        if (!setupPythonPath()) {
            qDebug() << "Failed to setup Python path";
            cleanup();
            return false;
        }
        
        // Test basic functionality
        if (!testPythonBasics()) {
            qDebug() << "Python basic functionality test failed";
            cleanup();
            return false;
        }
        
        m_initialized = true;
        qDebug() << "Python manager fully initialized";
        qDebug() << "Python version:" << getVersion();
        
        return true;
        
    } catch (...) {
        qDebug() << "Exception occurred during Python initialization";
        cleanup();
        return false;
    }
}

bool PythonManager::initializePythonModern() {
    // Use the new PyConfig API for Python 3.8+
    PyStatus status;
    PyConfig config;
    
    // Initialize config with default values
    PyConfig_InitPythonConfig(&config);
    
    // Find Python installation
    QString pythonExecutable;
    QString pythonHome;
    
    QStringList possiblePythonPaths = {
        "C:/msys64/ucrt64/bin/python.exe",
        "C:/msys64/mingw64/bin/python.exe",
        "C:/Python39/python.exe",
        "C:/Python310/python.exe",
        "C:/Python311/python.exe",
        "C:/Python312/python.exe"
    };
    
    // Find valid Python executable
    for (const QString& path : possiblePythonPaths) {
        if (QFileInfo::exists(path)) {
            pythonExecutable = path;
            pythonHome = QFileInfo(path).dir().absolutePath();
            qDebug() << "Found Python at:" << pythonExecutable;
            break;
        }
    }
    
    if (pythonExecutable.isEmpty()) {
        qDebug() << "Could not find Python executable";
        PyConfig_Clear(&config);
        return false;
    }
    
    // Set program name
    std::wstring programNameWide = pythonExecutable.toStdWString();
    status = PyConfig_SetString(&config, &config.program_name, programNameWide.c_str());
    if (PyStatus_Exception(status)) {
        qDebug() << "Failed to set program name";
        PyConfig_Clear(&config);
        return false;
    }
    
    // Set Python home
    std::wstring pythonHomeWide = pythonHome.toStdWString();
    status = PyConfig_SetString(&config, &config.home, pythonHomeWide.c_str());
    if (PyStatus_Exception(status)) {
        qDebug() << "Failed to set Python home";
        PyConfig_Clear(&config);
        return false;
    }
    
    // Configure module search paths
    QStringList pythonPaths;
    
    // Add standard library paths for MSYS2
    if (pythonHome.contains("msys64")) {
        pythonPaths << pythonHome + "/../lib/python3.12";
        pythonPaths << pythonHome + "/../lib/python3.12/lib-dynload";
        pythonPaths << pythonHome + "/../lib/python3.12/site-packages";
    }
    
    // Add current working directory and application directory
    pythonPaths << QDir::currentPath();
    pythonPaths << QCoreApplication::applicationDirPath();
    
    // Set module search paths
    for (const QString& path : pythonPaths) {
        std::wstring pathWide = path.toStdWString();
        status = PyWideStringList_Append(&config.module_search_paths, pathWide.c_str());
        if (PyStatus_Exception(status)) {
            qDebug() << "Failed to add path:" << path;
        }
    }
    
    // Enable module search paths
    config.module_search_paths_set = 1;
    
    // Set other configuration options
    config.optimization_level = 0;  // No optimization for debugging
    config.use_environment = 1;     // Use environment variables
    config.user_site_directory = 1; // Enable user site directory
    config.isolated = 0;            // Don't isolate Python
    
    // Initialize Python with the configuration
    status = Py_InitializeFromConfig(&config);
    
    // Clean up configuration
    PyConfig_Clear(&config);
    
    if (PyStatus_Exception(status)) {
        qDebug() << "Failed to initialize Python from config";
        if (PyStatus_IsError(status) && status.err_msg) {
            // Convert char* to QString properly
            QString errorMsg = QString::fromUtf8(status.err_msg);
            qDebug() << "Python initialization error:" << errorMsg;
        }
        return false;
    }
    
    return Py_IsInitialized();
}

bool PythonManager::isInitialized() const {
    bool pyInitialized = Py_IsInitialized();
    qDebug() << "isInitialized() check - m_initialized:" << m_initialized << "Py_IsInitialized():" << pyInitialized;
    return m_initialized && pyInitialized;
}

bool PythonManager::executeString(const QString &code) {
    qDebug() << "executeString called - isInitialized():" << isInitialized();
    
    if (!Py_IsInitialized()) {
        qDebug() << "Python interpreter not initialized";
        return false;
    }
    
    qDebug() << "Executing Python code:" << code.left(100) + (code.length() > 100 ? "..." : "");
    
    // Acquire GIL
    PyGILState_STATE gstate = PyGILState_Ensure();
    
    bool success = false;
    try {
        // Execute the code
        int result = PyRun_SimpleString(code.toUtf8().constData());
        success = (result == 0);
        
        if (!success) {
            qDebug() << "Python execution failed";
            // Print Python error if any
            if (PyErr_Occurred()) {
                PyErr_Print();
                PyErr_Clear();
            }
        } else {
            qDebug() << "Python execution successful";
        }
        
    } catch (...) {
        qDebug() << "Exception occurred during Python code execution";
        PyErr_Clear();
    }
    
    // Release GIL
    PyGILState_Release(gstate);
    
    return success;
}

bool PythonManager::executeFile(const QString &filename) {
    if (!Py_IsInitialized()) {
        qDebug() << "Python not initialized";
        return false;
    }
    
    QFileInfo fileInfo(filename);
    if (!fileInfo.exists() || !fileInfo.isReadable()) {
        qDebug() << "Python file not found or not readable:" << filename;
        return false;
    }
    
    qDebug() << "Executing Python file:" << filename;
    
    // Acquire GIL
    PyGILState_STATE gstate = PyGILState_Ensure();
    
    bool success = false;
    FILE* file = nullptr;
    
    try {
        // Open file
#ifdef _WIN32
        if (fopen_s(&file, filename.toLocal8Bit().constData(), "r") != 0) {
            file = nullptr;
        }
#else
        file = fopen(filename.toLocal8Bit().constData(), "r");
#endif
        
        if (file) {
            // Execute file
            int result = PyRun_SimpleFile(file, filename.toLocal8Bit().constData());
            success = (result == 0);
            
            if (!success) {
                if (PyErr_Occurred()) {
                    PyErr_Print();
                    PyErr_Clear();
                }
            }
        } else {
            qDebug() << "Failed to open Python file:" << filename;
        }
        
    } catch (...) {
        qDebug() << "Exception occurred during Python file execution";
        PyErr_Clear();
    }
    
    if (file) {
        fclose(file);
    }
    
    // Release GIL
    PyGILState_Release(gstate);
    
    return success;
}

bool PythonManager::addToPath(const QString &path) {
    if (!Py_IsInitialized()) {
        qDebug() << "Python not initialized for addToPath";
        return false;
    }
    
    qDebug() << "Adding to Python path:" << path;
    
    // Acquire GIL
    PyGILState_STATE gstate = PyGILState_Ensure();
    
    bool success = false;
    try {
        // Get sys module
        PyObject* sysModule = PyImport_ImportModule("sys");
        if (sysModule) {
            // Get sys.path
            PyObject* sysPath = PyObject_GetAttrString(sysModule, "path");
            if (sysPath && PyList_Check(sysPath)) {
                // Create Python string from path
                PyObject* pathString = PyUnicode_FromString(path.toUtf8().constData());
                if (pathString) {
                    // Append to sys.path
                    int result = PyList_Append(sysPath, pathString);
                    success = (result == 0);
                    Py_DECREF(pathString);
                }
                Py_DECREF(sysPath);
            }
            Py_DECREF(sysModule);
        }
        
        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
        }
        
    } catch (...) {
        qDebug() << "Exception occurred while adding to Python path";
        PyErr_Clear();
    }
    
    // Release GIL
    PyGILState_Release(gstate);
    
    return success;
}

QString PythonManager::getVersion() const {
    if (!Py_IsInitialized()) {
        return "Not initialized";
    }
    
    return QString::fromUtf8(Py_GetVersion());
}

QString PythonManager::evaluateExpression(const QString &expression) {
    if (!Py_IsInitialized()) {
        return "Error: Python not initialized";
    }
    
    // Acquire GIL
    PyGILState_STATE gstate = PyGILState_Ensure();
    
    QString result = "Error: Failed to evaluate expression";
    
    try {
        // Create a new dictionary for local variables
        PyObject* globals = PyDict_New();
        PyObject* locals = PyDict_New();
        
        if (globals && locals) {
            // Add builtins to globals
            PyDict_SetItemString(globals, "__builtins__", PyEval_GetBuiltins());
            
            // Evaluate expression
            PyObject* pyResult = PyRun_String(expression.toUtf8().constData(), 
                                            Py_eval_input, globals, locals);
            
            if (pyResult) {
                // Convert result to string
                PyObject* strResult = PyObject_Str(pyResult);
                if (strResult) {
                    const char* cStr = PyUnicode_AsUTF8(strResult);
                    if (cStr) {
                        result = QString::fromUtf8(cStr);
                    }
                    Py_DECREF(strResult);
                }
                Py_DECREF(pyResult);
            } else {
                if (PyErr_Occurred()) {
                    PyErr_Print();
                    PyErr_Clear();
                }
            }
        }
        
        if (globals) Py_DECREF(globals);
        if (locals) Py_DECREF(locals);
        
    } catch (...) {
        qDebug() << "Exception occurred during expression evaluation";
        PyErr_Clear();
    }
    
    // Release GIL
    PyGILState_Release(gstate);
    
    return result;
}

void PythonManager::cleanup() {
    if (m_initialized) {
        qDebug() << "Cleaning up Python interpreter...";
        
        // Don't call Py_Finalize() in embedded applications
        // as it can cause issues with subsequent initializations
        
        m_initialized = false;
    }
}

bool PythonManager::setupPythonPath() {
    qDebug() << "Setting up Python path...";
    
    // Add current directory
    QString currentDir = QDir::currentPath();
    if (!addToPath(currentDir)) {
        qDebug() << "Failed to add current directory to Python path";
        return false;
    }
    
    // Add application directory
    QString appDir = QCoreApplication::applicationDirPath();
    if (!addToPath(appDir)) {
        qDebug() << "Failed to add application directory to Python path";
        return false;
    }
    
    // Add common Python script directories
    QStringList scriptDirs;
    scriptDirs << "scripts" << "python" << "py";
    
    for (const QString& dir : scriptDirs) {
        QString fullPath = QDir(appDir).absoluteFilePath(dir);
        if (QDir(fullPath).exists()) {
            addToPath(fullPath);
        }
    }
    
    qDebug() << "Python path setup completed";
    return true;
}

bool PythonManager::testPythonBasics() {
    qDebug() << "Testing Python basics...";
    
    // Test basic Python functionality
    QString testCode = "import sys\nprint('Python test successful')\nprint('Python version:', sys.version)";
    
    bool result = executeString(testCode);
    qDebug() << "Python basics test result:" << result;
    
    return result;
}