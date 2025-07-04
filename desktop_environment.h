#ifndef DESKTOP_ENVIRONMENT_H
#define DESKTOP_ENVIRONMENT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>

class TaskBar;
class DesktopBackground;

class DesktopEnvironment : public QWidget {
    Q_OBJECT

public:
    explicit DesktopEnvironment(QWidget *parent = nullptr);
    
    // Make these public so TaskBar can access them
    void openTerminal();
    void openFileManager();
    void openSettings();
    void showAbout();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

public slots:
    void showDesktopMenu();

private:
    void setupDesktop();
    void setupTaskBar();
    void setupTrayIcon();
    
    TaskBar *m_taskBar;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_desktopMenu;
    QTimer *m_clockTimer;
};

class TaskBar : public QWidget {
    Q_OBJECT

public:
    explicit TaskBar(QWidget *parent = nullptr);
    
private slots:
    void updateClock();
    void showStartMenu();

private:
    void setupTaskBar();
    
    QPushButton *m_startButton;
    QLabel *m_clockLabel;
    QTimer *m_clockTimer;
    QMenu *m_startMenu;
};

#endif // DESKTOP_ENVIRONMENT_H