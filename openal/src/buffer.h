/**
 * @author jacob
 * @date   2018.11.06
 */

#pragma once

#include <string>

namespace audio_al {
    class data_buffer final {
    public:
        data_buffer(uint8_t* data, uint32_t size) {
            if (data) {
                buffer_.append((char*)data, size);
            } else {
                buffer_.resize(size);
            }
        }

    public:
        uint8_t* data() { 
            return (uint8_t*)buffer_.c_str(); 
        }

        uint32_t size() {
            return (uint32_t)buffer_.size();
        }

    private:
        std::string buffer_;
    };
}