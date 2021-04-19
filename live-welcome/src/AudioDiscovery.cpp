/*
 */

#include "AudioDiscovery.hpp"

//#define ALSA_PCM_NEW_HW_PARAMS_API
//#define ALSA_PCM_NEW_SW_PARAMS_API
#include <alsa/asoundlib.h>
#include <map>

static int nextPowerOfTwo(int size) noexcept
{
    // http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    --size;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return ++size;
}

static void getDeviceBufferSizes(snd_pcm_t* const pcm, std::vector<unsigned>& bufSizes)
{
    snd_pcm_hw_params_t* params;
    snd_pcm_hw_params_alloca(&params);

    if (snd_pcm_hw_params_any(pcm, params) >= 0)
    {
        int dir = 0;
        snd_pcm_uframes_t minSize = 0, maxSize = 0;
        snd_pcm_hw_params_get_period_size_min(params, &minSize, &dir);
        snd_pcm_hw_params_get_period_size_max(params, &maxSize, &dir);

        minSize = std::max(nextPowerOfTwo(minSize), 16);
        maxSize = std::min(maxSize, 8192LU);

        std::map<unsigned,bool> used;

        for (snd_pcm_uframes_t s = minSize; s <= maxSize; s = nextPowerOfTwo(s+1))
        {
            if (snd_pcm_hw_params_test_period_size(pcm, params, minSize, dir) == 0)
            {
                if (used.count(s) == 0)
                {
                    used[s] = true;
                    bufSizes.push_back(s);
                }
            }

            if (s == 8192)
                break;
        }
    }

    // snd_pcm_hw_params_free(params);
}

static void getDeviceSampleRates(snd_pcm_t* const pcm, std::vector<unsigned>& rates)
{
    const int ratesToTry[] = { 22050, 32000, 44100, 48000, 88200, 96000, 176400, 192000, 0 };

    snd_pcm_hw_params_t* params;
    snd_pcm_hw_params_alloca(&params);

    if (snd_pcm_hw_params_any(pcm, params) >= 0)
    {
        for (int i = 0; ratesToTry[i] != 0; ++i)
        {
            if (snd_pcm_hw_params_test_rate(pcm, params, ratesToTry[i], 0) == 0)
                rates.push_back(ratesToTry[i]);
        }
    }

    // snd_pcm_hw_params_free(params);
}

static void getDeviceNumChannels(snd_pcm_t* const pcm, unsigned& minChans, unsigned& maxChans)
{
    snd_pcm_hw_params_t *params;
    snd_pcm_hw_params_alloca(&params);

    if (snd_pcm_hw_params_any(pcm, params) >= 0)
    {
        snd_pcm_hw_params_get_channels_min(params, &minChans);
        snd_pcm_hw_params_get_channels_max(params, &maxChans);

        // put some sane limits
        maxChans = std::min(maxChans, 32U);
        minChans = std::min(minChans, maxChans);
    }

    // snd_pcm_hw_params_free(params);
}

static bool isdigit(const char* const s)
{
    const size_t len = strlen(s);

    if (len == 0)
        return false;

    for (size_t i=0; i<len; ++i)
    {
        if (std::isdigit(s[i]))
            continue;
        return false;
    }

    return true;
}

void enumerateSoundcards(std::stringlist& inputNames,
                         std::stringlist& outputNames,
                         std::stringlist& inputIds,
                         std::stringlist& outputIds)
{
    snd_ctl_t* ctl = nullptr;
    snd_ctl_card_info_t* cardinfo = nullptr;
    snd_ctl_card_info_alloca(&cardinfo);

    int card = -1;
    char hwcard[32];
    char reserve[32];

    while (outputIds.size() + inputIds.size() <= 64)
    {
        snd_card_next(&card);

        if (card < 0)
            break;

        snprintf(hwcard, sizeof(hwcard), "hw:%i", card);

        if (snd_ctl_open(&ctl, hwcard, SND_CTL_NONBLOCK) < 0)
            continue;

        if (snd_ctl_card_info(ctl, cardinfo) >= 0)
        {
            const char* cardId = snd_ctl_card_info_get_id(cardinfo);
            const char* cardName = snd_ctl_card_info_get_name(cardinfo);

            if (cardId == nullptr || ::isdigit(cardId))
            {
                snprintf(reserve, sizeof(reserve), "%i", card);
                cardId = reserve;
            }

            if (cardName == nullptr || *cardName == '\0')
                cardName = cardId;

            int device = -1;

            snd_pcm_info_t* pcminfo;
            snd_pcm_info_alloca(&pcminfo);

            for (;;)
            {
                if (snd_ctl_pcm_next_device(ctl, &device) < 0 || device < 0)
                    break;

                snd_pcm_info_set_device(pcminfo, device);

                for (int subDevice = 0, nbSubDevice = 1; subDevice < nbSubDevice; ++subDevice)
                {
                    snd_pcm_info_set_subdevice(pcminfo, subDevice);

                    snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_CAPTURE);
                    const bool isInput = (snd_ctl_pcm_info(ctl, pcminfo) >= 0);

                    snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_PLAYBACK);
                    const bool isOutput = (snd_ctl_pcm_info(ctl, pcminfo) >= 0);

                    if (! (isInput || isOutput))
                        continue;

                    if (nbSubDevice == 1)
                        nbSubDevice = snd_pcm_info_get_subdevices_count(pcminfo);

                    std::string strid(hwcard);
                    std::string strname(cardName);

                    strid += ",";
                    strid += std::to_string(device);

                    strname += ", ";
                    strname += snd_pcm_info_get_name(pcminfo);

                    if (nbSubDevice != 1)
                    {
                        strid += ",";
                        strid += std::to_string(subDevice);
                        strname += " {";
                        strname += snd_pcm_info_get_subdevice_name(pcminfo);
                        strname += "}";
                    }

                    if (isInput)
                    {
                        inputNames.push_back(strname);
                        inputIds.push_back(strid);
                    }

                    if (isOutput)
                    {
                        outputNames.push_back(strname);
                        outputIds.push_back(strid);
                    }
                }
            }

            snd_ctl_close(ctl);
        }
    }

    // snd_ctl_card_info_free(cardinfo);
}

void getDeviceProperties(const char* const deviceID, bool testOutput, bool testInput, DeviceProperties& props)
{
    props.minChansOut = props.maxChansOut = props.minChansIn = props.maxChansIn = 0;
    props.bufsizes.clear();
    props.rates.clear();

    if (deviceID == nullptr || *deviceID == '\0')
        return;

    snd_pcm_info_t* info;
    snd_pcm_info_alloca(&info);

    if (testOutput)
    {
        snd_pcm_t* pcm;

        if (snd_pcm_open(&pcm, deviceID, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) >= 0)
        {
            getDeviceNumChannels(pcm, props.minChansOut, props.maxChansOut);
            getDeviceBufferSizes(pcm, props.bufsizes);
            getDeviceSampleRates(pcm, props.rates);

            snd_pcm_close(pcm);
        }
    }

    if (testInput)
    {
        snd_pcm_t* pcm;

        if (snd_pcm_open(&pcm, deviceID, SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK) >= 0)
        {
            getDeviceNumChannels(pcm, props.minChansIn, props.maxChansIn);

            if (props.bufsizes.size() == 0)
                getDeviceBufferSizes(pcm, props.bufsizes);

            if (props.rates.size() == 0)
                getDeviceSampleRates(pcm, props.rates);

            snd_pcm_close(pcm);
        }
    }

    // snd_pcm_info_free(info);
}
