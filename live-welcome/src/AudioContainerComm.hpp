// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

typedef struct _AudioContainerComm {
    int sem;
    int shm1, shm2;
    int padding;
    float peaks[4];
} AudioContainerComm;

AudioContainerComm* initAudioContainerComm();
bool waitForAudioContainerComm(AudioContainerComm*);
void cleanupAudioContainerComm(AudioContainerComm*);
