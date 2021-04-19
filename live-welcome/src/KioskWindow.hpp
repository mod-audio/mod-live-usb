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

#include "../widgets/digitalpeakmeter.hpp"

// TESTING
#include <QtCore/QRandomGenerator>

class KioskWindow : public QMainWindow
{
    Q_OBJECT

    KioskTabs tabWidget;
    KioskSettingsPopup* settingsPopup;

    QFont clockFont;
    QRect clockRect;
    int clockTimer;

    DigitalPeakMeter peakMeterIn;
    DigitalPeakMeter peakMeterOut;
    QPushButton settingsButton;
    QPushButton powerButton;

    QProcess audioContainer;
    const QString program;

    // TESTING
    QRandomGenerator random;

public:
    KioskWindow()
      : QMainWindow(),
        settingsPopup(nullptr),
        tabWidget(this),
        clockFont(font()),
        clockRect(),
        clockTimer(-1),
        peakMeterIn(this),
        peakMeterOut(this),
        settingsButton(this),
        powerButton(this),
        audioContainer(),
        program(findStartScript())
    {
        setCentralWidget(&tabWidget);
        setWindowTitle("MOD Live USB");

        // audioContainer.setProcessChannelMode(QProcess::ForwardedChannels);

        const int height = tabWidget.tabBar()->height();

        clockFont.setFamily("Monospace");
        clockFont.setPixelSize(20);

        peakMeterIn.setChannelCount(2);
        peakMeterIn.setMeterColor(DigitalPeakMeter::COLOR_BLUE);
        peakMeterIn.setMeterLinesEnabled(false);
        peakMeterIn.setMeterOrientation(DigitalPeakMeter::HORIZONTAL);
        peakMeterIn.setMeterStyle(DigitalPeakMeter::STYLE_RNCBC);
        peakMeterIn.setFixedSize(150, height);

        peakMeterOut.setChannelCount(2);
        peakMeterOut.setMeterColor(DigitalPeakMeter::COLOR_GREEN);
        peakMeterOut.setMeterLinesEnabled(false);
        peakMeterOut.setMeterOrientation(DigitalPeakMeter::HORIZONTAL);
        peakMeterOut.setMeterStyle(DigitalPeakMeter::STYLE_OPENAV);
        peakMeterOut.setFixedSize(150, height);

        settingsButton.setFixedSize(height, height);
        settingsButton.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        settingsButton.setText("(S)");

        powerButton.setFixedSize(height, height);
        powerButton.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        powerButton.setText("(P)");

        repositionTabBarWidgets();

        clockTimer = startTimer(1000);

        connect(&powerButton, SIGNAL(clicked()), this, SLOT(openPower()));
        connect(&settingsButton, SIGNAL(clicked()), this, SLOT(openSettings()));

        if (program.isEmpty())
            settingsButton.hide();
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
    void openPower()
    {
    }

    void openSettings(const bool cancellable = true)
    {
        if (program.isEmpty())
            return;

        if (settingsPopup == nullptr)
            settingsPopup = new KioskSettingsPopup();

        settingsPopup->setCancellable(cancellable);
        settingsPopup->exec();

        QString device;
        unsigned rate;
        unsigned bufsize;

        if (! settingsPopup->getSelected(device, rate, bufsize))
        {
            if (cancellable)
                return;

            return openSettings(cancellable);
        }
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
        repositionTabBarWidgets();
    }

    void timerEvent(QTimerEvent* const event) override
    {
        QMainWindow::timerEvent(event);

        if (event->timerId() != clockTimer)
            return;
        if (clockRect.x() == 0)
            return;

        update(clockRect);

        // TESTING
        peakMeterIn.displayMeter(1, random.generateDouble(), true);
        peakMeterIn.displayMeter(2, random.generateDouble(), true);
        peakMeterOut.displayMeter(1, random.generateDouble(), true);
        peakMeterOut.displayMeter(2, random.generateDouble(), true);
    }

private:
    void repositionTabBarWidgets()
    {
        const int width = this->width();
        const int height = tabWidget.tabBar()->height();

        // clock at center
        QFontMetrics clockmetrics(clockFont);
        const int clockwidth = clockmetrics.horizontalAdvance("00:00:00");
        const int clockheight = clockmetrics.height();
        clockRect = QRect(width/2 - clockwidth/2, height/2 - clockheight/2, clockwidth, clockheight);

        // going from the corner..
        const int padding = 4;
        int x = width;

        x -= powerButton.width() + padding;
        powerButton.move(x, 0);

        x -= settingsButton.width() + padding;
        settingsButton.move(x, 0);

        x -= peakMeterOut.width() + padding;
        peakMeterOut.move(x, 0);

        x -= peakMeterIn.width() + padding;
        peakMeterIn.move(x, 0);
    }
};
