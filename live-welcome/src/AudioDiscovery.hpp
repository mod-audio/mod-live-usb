// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <string>
#include <vector>

namespace std {
typedef vector<string> stringlist;
}

typedef struct {
    unsigned minChansOut = 0;
    unsigned maxChansOut = 0;
    unsigned minChansIn = 0;
    unsigned maxChansIn = 0;
    std::vector<unsigned> bufsizes;
    std::vector<unsigned> rates;
} DeviceProperties;

void enumerateSoundcards(std::stringlist& inputNames,
                         std::stringlist& outputNames,
                         std::stringlist& inputIds,
                         std::stringlist& outputIds);

void getDeviceProperties(const char* const deviceID,
                         bool testOutput,
                         bool testInput,
                         DeviceProperties& props);
