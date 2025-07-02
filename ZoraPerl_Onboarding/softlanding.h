#ifndef SOFTLANDING_H
#define SOFTLANDING_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class SoftLanding : public QWidget {
    Q_OBJECT

public:
    SoftLanding(QWidget *parent = nullptr);

signals:
    void finished();

private slots:
    void setupDirectories();
    void goToSetup();

private:
    QLabel *greeting;
};

#endif // SOFTLANDING_H
