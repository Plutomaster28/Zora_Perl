#include "desktop_environment.h"
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QProcess>
#include <QTime>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QCursor>

DesktopEnvironment::DesktopEnvironment(QWidget *parent)
    : QWidget(parent), m_taskBar(nullptr), m_trayIcon(nullptr), m_desktopMenu(nullptr) {
    setupDesktop();
    setupTaskBar();
    setupTrayIcon();
}

void DesktopEnvironment::setupDesktop() {
    // Set up the desktop to be full screen
    setWindowTitle("ZoraPerl Desktop");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    
    // Get screen geometry (Qt6 way)
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        setGeometry(screenGeometry);
    }
    
    // Set up layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Add stretch to push taskbar to bottom
    mainLayout->addStretch();
    
    // Create desktop context menu
    m_desktopMenu = new QMenu(this);
    
    QAction *terminalAction = m_desktopMenu->addAction("Open Terminal");
    QAction *fileManagerAction = m_desktopMenu->addAction("File Manager");
    m_desktopMenu->addSeparator();
    QAction *settingsAction = m_desktopMenu->addAction("Settings");
    QAction *aboutAction = m_desktopMenu->addAction("About ZoraPerl");
    m_desktopMenu->addSeparator();
    QAction *exitAction = m_desktopMenu->addAction("Exit");
    
    connect(terminalAction, &QAction::triggered, this, &DesktopEnvironment::openTerminal);
    connect(fileManagerAction, &QAction::triggered, this, &DesktopEnvironment::openFileManager);
    connect(settingsAction, &QAction::triggered, this, &DesktopEnvironment::openSettings);
    connect(aboutAction, &QAction::triggered, this, &DesktopEnvironment::showAbout);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    // Set minimum size
    setMinimumSize(800, 600);
    
    qDebug() << "Desktop environment initialized";
}

void DesktopEnvironment::setupTaskBar() {
    m_taskBar = new TaskBar(this);
    
    // Add taskbar to layout
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
    if (layout) {
        layout->addWidget(m_taskBar);
    }
}

void DesktopEnvironment::setupTrayIcon() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "System tray not available";
        return;
    }
    
    m_trayIcon = new QSystemTrayIcon(this);
    
    // Create tray icon menu
    QMenu *trayMenu = new QMenu(this);
    trayMenu->addAction("Show Desktop", this, &DesktopEnvironment::show);
    trayMenu->addAction("Hide Desktop", this, &DesktopEnvironment::hide);
    trayMenu->addSeparator();
    trayMenu->addAction("Exit", qApp, &QApplication::quit);
    
    m_trayIcon->setContextMenu(trayMenu);
    m_trayIcon->setToolTip("ZoraPerl Desktop");
    
    // Set a simple icon (you can replace this with an actual icon file)
    QPixmap iconPixmap(16, 16);
    iconPixmap.fill(QColor(0, 122, 255));
    m_trayIcon->setIcon(QIcon(iconPixmap));
    
    m_trayIcon->show();
}

void DesktopEnvironment::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    // Create gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(135, 206, 235));  // Sky blue
    gradient.setColorAt(1.0, QColor(25, 25, 112));    // Midnight blue
    
    painter.fillRect(rect(), gradient);
    
    // Draw ZoraPerl logo/text in center
    painter.setPen(QPen(Qt::white, 2));
    painter.setFont(QFont("Segoe UI", 48, QFont::Light));
    
    QRect textRect = rect();
    textRect.setBottom(textRect.bottom() - 100); // Account for taskbar
    
    painter.drawText(textRect, Qt::AlignCenter, "ZoraPerl");
    
    // Draw welcome message
    painter.setFont(QFont("Segoe UI", 16, QFont::Normal));
    painter.setPen(QPen(Qt::white, 1));
    
    QRect welcomeRect = textRect;
    welcomeRect.setTop(welcomeRect.center().y() + 30);
    
    painter.drawText(welcomeRect, Qt::AlignCenter | Qt::AlignTop, "Right-click for options");
}

void DesktopEnvironment::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Left click - could be used for desktop icons in the future
        qDebug() << "Desktop left-clicked at:" << event->pos();
    }
    QWidget::mousePressEvent(event);
}

void DesktopEnvironment::contextMenuEvent(QContextMenuEvent *event) {
    m_desktopMenu->exec(event->globalPos());
}

void DesktopEnvironment::showDesktopMenu() {
    // This could be triggered by a button or key combination
    m_desktopMenu->exec(QCursor::pos());
}

void DesktopEnvironment::openTerminal() {
    qDebug() << "Opening terminal...";
    
    // Try to open Windows Terminal, then Command Prompt
    QProcess *process = new QProcess(this);
    
    // Try Windows Terminal first
    process->start("wt.exe");
    if (!process->waitForStarted(1000)) {
        // Fall back to Command Prompt
        process->start("cmd.exe");
        if (!process->waitForStarted(1000)) {
            QMessageBox::warning(this, "Error", "Could not open terminal");
            return;
        }
    }
    
    qDebug() << "Terminal opened";
}

void DesktopEnvironment::openFileManager() {
    qDebug() << "Opening file manager...";
    
    QProcess *process = new QProcess(this);
    process->start("explorer.exe");
    
    if (!process->waitForStarted(1000)) {
        QMessageBox::warning(this, "Error", "Could not open file manager");
        return;
    }
    
    qDebug() << "File manager opened";
}

void DesktopEnvironment::openSettings() {
    qDebug() << "Opening settings...";
    QMessageBox::information(this, "Settings", "Settings panel coming soon!");
}

void DesktopEnvironment::showAbout() {
    QMessageBox::about(this, "About ZoraPerl", 
                       "ZoraPerl Desktop Environment\n"
                       "Version 1.0\n\n"
                       "A custom desktop environment built with Qt.\n"
                       "© 2024 ZoraPerl Project");
}

// TaskBar implementation
TaskBar::TaskBar(QWidget *parent) : QWidget(parent) {
    setupTaskBar();
}

void TaskBar::setupTaskBar() {
    setFixedHeight(50);
    setStyleSheet("TaskBar { background-color: rgba(0, 0, 0, 180); border-top: 1px solid rgba(255, 255, 255, 50); }");
    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(10);
    
    // Start button
    m_startButton = new QPushButton("Start");
    m_startButton->setStyleSheet(
        "QPushButton { "
        "background-color: rgba(0, 122, 255, 200); "
        "border: none; "
        "border-radius: 5px; "
        "color: white; "
        "font-weight: bold; "
        "font-size: 14px; "
        "padding: 8px 16px; "
        "} "
        "QPushButton:hover { "
        "background-color: rgba(0, 122, 255, 255); "
        "} "
        "QPushButton:pressed { "
        "background-color: rgba(0, 100, 200, 255); "
        "}"
    );
    
    connect(m_startButton, &QPushButton::clicked, this, &TaskBar::showStartMenu);
    
    // Clock
    m_clockLabel = new QLabel;
    m_clockLabel->setStyleSheet("QLabel { color: white; font-size: 14px; font-weight: bold; }");
    m_clockLabel->setAlignment(Qt::AlignCenter);
    
    // Update clock
    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &TaskBar::updateClock);
    m_clockTimer->start(1000);
    updateClock();
    
    // Layout
    layout->addWidget(m_startButton);
    layout->addStretch();
    layout->addWidget(m_clockLabel);
    
    // Create start menu
    m_startMenu = new QMenu(this);
    m_startMenu->addAction("Terminal", [this]() {
        if (DesktopEnvironment *desktop = qobject_cast<DesktopEnvironment*>(parent())) {
            desktop->openTerminal();
        }
    });
    m_startMenu->addAction("File Manager", [this]() {
        if (DesktopEnvironment *desktop = qobject_cast<DesktopEnvironment*>(parent())) {
            desktop->openFileManager();
        }
    });
    m_startMenu->addSeparator();
    m_startMenu->addAction("Settings", [this]() {
        if (DesktopEnvironment *desktop = qobject_cast<DesktopEnvironment*>(parent())) {
            desktop->openSettings();
        }
    });
    m_startMenu->addAction("About", [this]() {
        if (DesktopEnvironment *desktop = qobject_cast<DesktopEnvironment*>(parent())) {
            desktop->showAbout();
        }
    });
    m_startMenu->addSeparator();
    m_startMenu->addAction("Exit", qApp, &QApplication::quit);
}

void TaskBar::updateClock() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("hh:mm:ss");
    QString dateString = currentTime.toString("MMM dd, yyyy");
    
    m_clockLabel->setText(QString("%1\n%2").arg(timeString, dateString));
}

void TaskBar::showStartMenu() {
    QPoint buttonPos = m_startButton->mapToGlobal(QPoint(0, 0));
    QPoint menuPos = QPoint(buttonPos.x(), buttonPos.y() - m_startMenu->sizeHint().height());
    
    m_startMenu->exec(menuPos);
}