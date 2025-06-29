#pragma once
#include <QWidget>
#include <QLabel>
#include <QTimer>

class SoftLanding : public QWidget {
    Q_OBJECT

public:
    SoftLanding(QWidget *parent = nullptr);

private slots:
    void goToSetup();

signals:
    void finished();

private:
    QLabel *greeting;
};
