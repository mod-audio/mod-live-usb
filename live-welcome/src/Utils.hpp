/*
 */

#pragma once

#include <QtCore/QString>

typedef struct {
    int sem;
    int shm1, shm2;
    int padding;
    float peaks[4];
} AudioContainerComm;

QString findDocumentation();
QString findStartScript();

AudioContainerComm* initAudioContainerComm();
bool waitForAudioContainerComm(AudioContainerComm*);
void cleanupAudioContainerComm(AudioContainerComm*);
