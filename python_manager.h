#ifndef PYTHON_MANAGER_H
#define PYTHON_MANAGER_H

#include <QObject>
#include <QString>

class PythonManager : public QObject
{
    Q_OBJECT

public:
    explicit PythonManager(QObject *parent = nullptr);
    ~PythonManager();

    bool initialize();
    bool isInitialized() const;
    
    bool executeString(const QString &code);
    bool executeFile(const QString &filename);
    bool addToPath(const QString &path);
    
    QString getVersion() const;
    QString evaluateExpression(const QString &expression);
    
    void cleanup();

private:
    bool initializePythonModern();
    bool configurePythonPaths(); // Deprecated, kept for compatibility
    bool setupPythonPath();
    bool testPythonBasics();
    
    bool m_initialized;
};

#endif // PYTHON_MANAGER_H