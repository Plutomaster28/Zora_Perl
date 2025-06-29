#include "softlanding.h"
#include <QVBoxLayout>
#include <QFont>
#include <QPalette>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

SoftLanding::SoftLanding(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #101010;");  // Dark clean bg

    greeting = new QLabel("Welcome to Zora Perl", this);
    greeting->setAlignment(Qt::AlignCenter);

    QFont font("Segoe UI", 32, QFont::Medium);
    greeting->setFont(font);
    greeting->setStyleSheet("color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(greeting);
    layout->addStretch();
    setLayout(layout);

    // Fade in animation (like macOS Setup)
    auto *opacity = new QGraphicsOpacityEffect(greeting);
    greeting->setGraphicsEffect(opacity);

    auto *fade = new QPropertyAnimation(opacity, "opacity");
    fade->setDuration(1500);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->start(QPropertyAnimation::DeleteWhenStopped);

    // After 3.5s, go to setup
    QTimer::singleShot(3500, this, SLOT(goToSetup()));
}

void SoftLanding::goToSetup() {
    // TODO: Replace this with real setup page
    greeting->setText("Starting Setup...");
    QFont font("Segoe UI", 24, QFont::Normal);
    greeting->setFont(font);
    emit finished();
}
