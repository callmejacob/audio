/**
 * @author jacob
 * @date   2018.11.06
 */

#pragma once

#include <cstdio>
#include <functional>
#include <string>

#include "buffer.h"

namespace audio_al {

    typedef std::function<void(std::shared_ptr<data_buffer>)> RES_CALLBACK; 

    class audio_res_mgr final {
    public:
        audio_res_mgr();
        virtual ~audio_res_mgr();

    public:
        void get_audio_data(const std::string&, RES_CALLBACK);
    };
}