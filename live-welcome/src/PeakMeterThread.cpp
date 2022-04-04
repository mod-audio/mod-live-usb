// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "PeakMeterThread.hpp"

#include "AudioContainerComm.hpp"
#include "../widgets/digitalpeakmeter.hpp"

PeakMeterThread::PeakMeterThread(QObject* const parent, DigitalPeakMeter* const in, DigitalPeakMeter* const out)
  : QThread(parent),
    containerComm(nullptr),
    peakMeterIn(in),
    peakMeterOut(out),
    sys_host_shmfd(-1),
    sys_host_data(nullptr) {}

void PeakMeterThread::init()
{
    if ((containerComm = initAudioContainerComm()) != nullptr)
    {
        if (sys_serial_open(&sys_host_shmfd, &sys_host_data))
            fprintf(stdout, "sys_host shared memory ok!\n");
        else
            fprintf(stderr, "sys_host shared memory failed\n");

        start(HighPriority);
    }
}

void PeakMeterThread::stop()
{
    cleanupAudioContainerComm(containerComm);
    sys_serial_close(sys_host_shmfd, sys_host_data);
    sys_host_shmfd = -1;
    sys_host_data = nullptr;
    wait(2000);
}

void PeakMeterThread::send(const sys_serial_event_type etype, const int value)
{
    if (sys_host_data == nullptr)
        return;

    char str[24];
    snprintf(str, sizeof(str), "%i", value);
    str[sizeof(str)-1] = '\0';

    if (! sys_serial_write(&sys_host_data->client, etype, str))
        return;

    sem_post(&sys_host_data->client.sem);
}

void PeakMeterThread::run()
{
    float peaks[4];
    char msg[SYS_SERIAL_SHM_DATA_SIZE];
    sys_serial_event_type etype;
    uint8_t page, subpage;

    sys_serial_shm_data_channel* const sysdata = sys_host_data != nullptr ? &sys_host_data->server : nullptr;

    while (containerComm != nullptr && ! isInterruptionRequested())
    {
        // flush all incoming host events
        if (sys_host_data != nullptr && sem_trywait(&sysdata->sem) == 0)
        {
            while (sysdata->head != sysdata->tail)
                sys_serial_read(sysdata, &etype, &page, &subpage, msg);
        }

        if (! waitForAudioContainerComm(containerComm))
            continue;

        memcpy(peaks, containerComm->peaks, sizeof(peaks));
        peakMeterIn->displayMeter(1, peaks[0]);
        peakMeterIn->displayMeter(2, peaks[1]);
        peakMeterOut->displayMeter(1, peaks[2]);
        peakMeterOut->displayMeter(2, peaks[3]);
    }
}
