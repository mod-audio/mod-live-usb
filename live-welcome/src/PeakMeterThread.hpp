// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <QtCore/QThread>

#define SERVER_MODE
#include "../sys_host/sys_host.h"

class DigitalPeakMeter;
typedef struct _AudioContainerComm AudioContainerComm;

class PeakMeterThread : public QThread
{
    AudioContainerComm* containerComm;
    DigitalPeakMeter* const peakMeterIn;
    DigitalPeakMeter* const peakMeterOut;

    int sys_host_shmfd;
    sys_serial_shm_data* sys_host_data;

public:
    explicit PeakMeterThread(QObject* parent, DigitalPeakMeter* in, DigitalPeakMeter* out);

    // start communication with audio container, and start thread if that succeeds
    void init();

    // stop communication with audio container, waits maximum 2s for thread to stop
    void stop();

    // send a value to audio container side
    void send(sys_serial_event_type etype, int value);

private:
    // listening thread
    void run() override;
};
