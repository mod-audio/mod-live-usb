/*
 */

#pragma once

#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabBar>

#include "KioskTabs.hpp"
#include "KioskSettingsPopup.hpp"

#include "Utils.hpp"

class KioskWindow : public QMainWindow
{
    Q_OBJECT

    KioskTabs tabWidget;
    KioskSettingsPopup* settingsPopup;

    QFont clockFont;
    QRect clockRect;
    int clockTimer;

    QPushButton settingsButton;

    QProcess audioContainer;
    const QString program;

public:
    KioskWindow()
      : QMainWindow(),
        settingsPopup(nullptr),
        tabWidget(this),
        clockFont(font()),
        clockRect(),
        clockTimer(-1),
        settingsButton(this),
        audioContainer(),
        program(findStartScript())
    {
        setCentralWidget(&tabWidget);
        setWindowTitle("MOD Live USB");

        // audioContainer.setProcessChannelMode(QProcess::ForwardedChannels);

        clockFont.setFamily("Monospace");
        clockFont.setPixelSize(20);

        QFontMetrics metrics(clockFont);
        const int height = tabWidget.tabBar()->height();
        const int fontheight = metrics.height();

        clockRect = QRect(0, height/2 - fontheight/2, metrics.horizontalAdvance("00:00:00"), fontheight);
        clockTimer = startTimer(1000);

        if (program.isEmpty())
        {
            settingsButton.hide();
            return;
        }

        settingsButton.setFixedWidth(32);
        settingsButton.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        settingsButton.setText("(S)");
        settingsButton.move(width() - clockRect.width() - settingsButton.width() - 8, 0);
        connect(&settingsButton, SIGNAL(clicked()), this, SLOT(openSettings()));
    }

    ~KioskWindow() override
    {
        stopAudioContainer();
    }

    void stopAudioContainer()
    {
        if (audioContainer.state() != QProcess::NotRunning)
        {
            audioContainer.terminate();
            audioContainer.waitForFinished(5000);
        }

        QProcess mctl;
        mctl.setProcessChannelMode(QProcess::ForwardedChannels);
        mctl.start("machinectl", {"stop", "mod-live-usb"}, QIODevice::ReadWrite | QIODevice::Unbuffered);
        mctl.waitForFinished();

        if (audioContainer.state() != QProcess::NotRunning)
            audioContainer.kill();
    }

public Q_SLOTS:
    void openSettings()
    {
        if (program.isEmpty())
            return;

        if (settingsPopup == nullptr)
            settingsPopup = new KioskSettingsPopup();

        settingsPopup->exec();

        QString device;
        unsigned rate;
        unsigned bufsize;

        if (! settingsPopup->getSelected(device, rate, bufsize))
            return openSettings();
        printf("Selected %s %u %u\n", device.toUtf8().constData(), rate, bufsize);

        stopAudioContainer();

        const QStringList arguments = { device, QString::number(rate), QString::number(bufsize) };
        // audioContainer.start(program, arguments, QIODevice::ReadWrite | QIODevice::Unbuffered);
        audioContainer.startDetached(program, arguments);

        QTimer::singleShot(500, this, SLOT(tryConnectingToWebServer()));
    }

    void tryConnectingToWebServer()
    {
        QTcpSocket socket;
        socket.connectToHost("localhost", 8000);

        if (! socket.waitForConnected(10))
        {
            QTimer::singleShot(500, this, SLOT(tryConnectingToWebServer()));
            return;
        }

        tabWidget.reloadPage();
    }

protected:
    void keyPressEvent(QKeyEvent* const event) override
    {
        QMainWindow::keyPressEvent(event);

        const Qt::KeyboardModifiers modifiers = event->modifiers();

        if ((modifiers & Qt::Modifier::ALT) == 0x0)
            return;
        if ((modifiers & Qt::Modifier::CTRL) == 0x0)
            return;

        if (event->key() == Qt::Key::Key_R)
            tabWidget.reloadPage();
        if (event->key() == Qt::Key::Key_T)
            tabWidget.openKonsole();
    }

    void paintEvent(QPaintEvent* const event) override
    {
        QMainWindow::paintEvent(event);

        if (clockRect.x() == 0)
            return;

        QPainter painter(this);
        painter.setFont(clockFont);
        painter.drawText(clockRect, QTime::currentTime().toString("hh:mm:ss"));
    }

    void resizeEvent(QResizeEvent* const event) override
    {
        QMainWindow::resizeEvent(event);

        clockRect.moveTo(width() - clockRect.width(), clockRect.y());
        settingsButton.move(width() - clockRect.width() - settingsButton.width() - 8, 0);

        update(clockRect);
    }

    void timerEvent(QTimerEvent* const event) override
    {
        QMainWindow::timerEvent(event);

        if (event->timerId() != clockTimer)
            return;
        if (clockRect.x() == 0)
            return;

        update(clockRect);
    }
};
