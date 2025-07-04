#ifndef SYSTEM_CHECKER_H
#define SYSTEM_CHECKER_H

#include <QObject>
#include <QString>
#include <QDir>

class SystemChecker : public QObject {
    Q_OBJECT

public:
    explicit SystemChecker(QObject *parent = nullptr);
    
    bool isSystemConfigured();
    bool runOnboarding();
    
private:
    bool checkZoraPerlDirectory();
    bool checkConfigFile();
    QString findOnboardingExecutable();
    QString getZoraPerlPath();
    
    QString m_zoraPerlPath;
    QString m_configPath;
};

#endif // SYSTEM_CHECKER_H