#include <QApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDebug>
#include <QTimer>
#include <QSplashScreen>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QCoreApplication>
#include "system_checker.h"
#include "python_manager.h"
#include "desktop_environment.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("ZoraPerl");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ZoraPerl");
    
    // Create splash screen
    QPixmap splashPixmap(400, 300);
    splashPixmap.fill(QColor(16, 16, 16)); // Dark background
    
    QPainter painter(&splashPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw logo text
    painter.setPen(Qt::white);
    painter.setFont(QFont("Segoe UI", 24, QFont::Medium));
    painter.drawText(splashPixmap.rect(), Qt::AlignCenter, "ZoraPerl");
    
    QSplashScreen splash(splashPixmap);
    splash.show();
    app.processEvents();
    
    // Initialize system checker
    SystemChecker checker;
    
    splash.showMessage("Checking system configuration...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
    app.processEvents();
    
    // Check if system is properly configured
    if (!checker.isSystemConfigured()) {
        splash.showMessage("System not configured. Starting setup...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
        app.processEvents();
        
        QTimer::singleShot(1000, [&]() {
            splash.close();
            if (!checker.runOnboarding()) {
                QMessageBox::critical(nullptr, "Error", "Failed to start onboarding process. Please run ZoraPerl_Onboarding manually.");
                app.quit();
                return;
            }
            // After onboarding, we should restart the main application
            QMessageBox::information(nullptr, "Setup Complete", "ZoraPerl has been configured. Please restart the application.");
            app.quit();
        });
        
        return app.exec();
    }
    
    // Initialize Python interpreter
    splash.showMessage("Initializing Python interpreter...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
    app.processEvents();
    
    PythonManager pythonManager;
    if (!pythonManager.initialize()) {
        splash.close();
        QMessageBox::critical(nullptr, "Error", "Failed to initialize Python interpreter.");
        return -1;
    }
    
    splash.showMessage("Starting desktop environment...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
    app.processEvents();
    
    // Create and show desktop environment
    DesktopEnvironment desktop;
    
    QTimer::singleShot(1500, [&]() {
        splash.close();
        desktop.show();
    });
    
    return app.exec();
}