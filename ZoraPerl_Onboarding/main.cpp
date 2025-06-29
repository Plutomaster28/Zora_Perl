// main.cpp
#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "softlanding.h"
#include "setupwizard.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/music/The_Day_of_Night.mp3"));
    audioOutput->setVolume(1.0); // Set volume (0.0 - 1.0)
    player->play();

    SoftLanding window;
    window.setFixedSize(1280, 720); // Set window size to 1280x720 (16:9)
    QObject::connect(&window, &SoftLanding::finished, [&]() {
        window.hide();
        SetupWizard *wizard = new SetupWizard();
        wizard->setFixedSize(1280, 720); // Also set wizard size
        wizard->show();
    });
    window.show(); // Apple-style fullscreen welcome screen

    return app.exec();
}
