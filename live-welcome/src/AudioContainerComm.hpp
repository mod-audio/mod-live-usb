/*
 */

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
