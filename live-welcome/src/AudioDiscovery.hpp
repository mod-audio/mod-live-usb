/*
 */

#pragma once

#include <string>
#include <vector>

namespace std {
typedef vector<string> stringlist;
}

void enumerateSoundcards(std::stringlist& inputNames,
                         std::stringlist& outputNames,
                         std::stringlist& inputIds,
                         std::stringlist& outputIds);

void getDeviceProperties(const char* const deviceID,
                         unsigned& minChansOut,
                         unsigned& maxChansOut,
                         unsigned& minChansIn,
                         unsigned& maxChansIn,
                         std::vector<unsigned>& bufSizes,
                         std::vector<unsigned>& rates,
                         bool testOutput,
                         bool testInput);
