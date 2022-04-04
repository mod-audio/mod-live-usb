// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <QtCore/QProcess>
#include <QtWidgets/QMainWindow>

class DigitalPeakMeter;
class PeakMeterThread;
class QPushButton;
class QSlider;
class KioskSettingsPopup;
class KioskTabs;

#include "Utils.hpp"

class KioskWindow : public QMainWindow
{
    Q_OBJECT

    KioskSettingsPopup* const settingsPopup;
    KioskTabs* const tabWidget;

    QFont clockFont;
    QRect clockRect;
    int clockTimer;

    QSlider* const gainSlider;
    DigitalPeakMeter* const peakMeterIn;
    DigitalPeakMeter* const peakMeterOut;
    QPushButton* const settingsButton;
    QPushButton* const powerButton;

    const QString program;
    QProcess audioContainerProcess;

    PeakMeterThread* const peakMeterThread;

public:
    KioskWindow(bool desktop);
    ~KioskWindow() override;

    void stopAudioContainer();

public Q_SLOTS:
    void openPower();
    void openSettings(bool cancellable = true);
    void tryConnectingToWebServer();
    void setGain(int gain);

protected:
    void keyPressEvent(QKeyEvent* const event) override;
    void paintEvent(QPaintEvent* const event) override;
    void closeEvent(QCloseEvent* const event) override;
    void resizeEvent(QResizeEvent* const event) override;
    void timerEvent(QTimerEvent* const event) override;

private:
    void repositionTabBarWidgets();
};
