/**
 * @author jacob
 * @date   2018.11.06
 */

#pragma once

#include <cstdio>
#include <functional>
#include <vector>
#include <map>
#include <string>

#include "buffer.h"
#include "audio_decoder.h"
#include "audio_res.h"

namespace audio_al {
    class audio_mgr : std::enable_shared_from_this<audio_mgr> {
    public:
        audio_mgr();
        virtual ~audio_mgr();

    public:
        void play(const std::string& url);
        void pause(const std::string& url);
        void stop(const std::string& url);
        void destroy(const std::string& url);
        void seek(const std::string& url, float pos);

    private:
        uint32_t get_buffer(const std::string&);
        uint32_t get_source(const std::string&);
        void remove_buffer(uint32_t buffer);
        void remove_source(uint32_t source);
        void decode(const std::string& url, std::shared_ptr<data_buffer> buffer);

    private:
        void* al_device_;
        void* al_context_;
        std::vector<uint32_t> buffers_;
        std::vector<uint32_t> sources_;
        std::map<std::string, uint32_t> buffer_map_;
        std::map<std::string, uint32_t> source_map_;

    private:
        //! audio decode manager
        std::shared_ptr<audio_decoder_mgr> decoder_mgr_;
        //! audio res manager
        std::shared_ptr<audio_res_mgr>     res_mgr_;
    };

}
