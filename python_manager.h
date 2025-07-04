#ifndef PYTHON_MANAGER_H
#define PYTHON_MANAGER_H

#include <QObject>
#include <QString>

class PythonManager : public QObject {
    Q_OBJECT

public:
    explicit PythonManager(QObject *parent = nullptr);
    ~PythonManager();
    
    bool initialize();
    bool isInitialized() const;
    
    // Execute Python code
    bool executeString(const QString &code);
    bool executeFile(const QString &filename);
    
    // Python environment management
    bool addToPath(const QString &path);
    QString getVersion() const;
    
private:
    bool m_initialized;
    void cleanup();
};

#endif // PYTHON_MANAGER_H