// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "AudioContainerComm.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <linux/futex.h>
#include <sys/mman.h>
#include <syscall.h>
#include <unistd.h>

#if !defined(SYS_futex) && defined(SYS_futex_time64)
#define SYS_futex SYS_futex_time64
#endif

AudioContainerComm* initAudioContainerComm()
{
    int fd;
    void* ptr;
    AudioContainerComm* comm;

    shm_unlink("/ac");

    if ((fd = shm_open("/ac", O_CREAT|O_EXCL|O_RDWR, 0600)) < 0)
    {
        fprintf(stderr, "initAudioContainerComm: shm_open failed\n");
        return nullptr;
    }

    if (ftruncate(fd, sizeof(AudioContainerComm)) != 0)
    {
        fprintf(stderr, "initAudioContainerComm: ftruncate failed\n");
        close(fd);
        return nullptr;
    }

    if ((ptr = mmap(nullptr,
                    sizeof(AudioContainerComm),
                    PROT_READ|PROT_WRITE,
                    MAP_SHARED|MAP_LOCKED, fd, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "initAudioContainerComm: mmap failed\n");
        close(fd);
        return nullptr;
    }

    comm = (AudioContainerComm*)ptr;
    memset(comm, 0, sizeof(AudioContainerComm));

    comm->shm1 = fd;
    return comm;
}

bool waitForAudioContainerComm(AudioContainerComm* const comm)
{
    if (comm == nullptr)
        return false;

    // 1 second
    const timespec timeout = { 1, 0 };

    for (;;)
    {
        if (__sync_bool_compare_and_swap(&comm->sem, 1, 0))
            return true;

        if (::syscall(SYS_futex, &comm->sem, FUTEX_WAIT, 0, &timeout, nullptr, 0) != 0)
            if (errno != EAGAIN && errno != EINTR)
                return false;
    }
}

void cleanupAudioContainerComm(AudioContainerComm* const comm)
{
    if (comm == nullptr)
        return;

    const int fd = comm->shm1;
    comm->shm1 = -1;

    munmap(comm, sizeof(AudioContainerComm));
    close(fd);
    shm_unlink("/ac");
}
