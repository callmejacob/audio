/**
 * @author jacob
 * @date   2018.11.06
 */

#pragma once

#include <cstdio>
#include <functional>

#include "buffer.h"

namespace audio_al {

    enum audio_format {
        FORMAT_MONO8    = 0x1100,
        FORMAT_MONO16   = 0x1101,
        FORMAT_STEREO8  = 0x1102,
        FORMAT_STEREO16 = 0x1103
    };

    typedef std::function<
        void(audio_format, std::shared_ptr<data_buffer>, size_t, size_t)> DECODE_CALLBACK; 

    class audio_decoder_mgr final {
    public:
        audio_decoder_mgr();
        virtual ~audio_decoder_mgr();

    public:
        void decode(std::shared_ptr<data_buffer>, DECODE_CALLBACK);
    };
}