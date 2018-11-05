#include "audio_mgr.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

using namespace audio_al;

audio_mgr::audio_mgr() {
    decoder_mgr_ = std::make_shared<audio_decoder_mgr>();
    res_mgr_ = std::make_shared<audio_res_mgr>();

    // init al device
    char* default_device_name = nullptr;
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) {
        default_device_name = (char*) alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    }
    al_device_ = alcOpenDevice(default_device_name);
    if (al_device_) {
        al_context_ = alcCreateContext((ALCdevice*)al_device_, nullptr);
        if (al_context_) {
            alcMakeContextCurrent((ALCcontext*)al_context_);
        } else {
            alcCloseDevice((ALCdevice*)al_device_);
            al_device_ = nullptr;
        }
    }
}

audio_mgr::~audio_mgr() {
    if (al_device_) {
        for (size_t i = 0; i < buffers_.size(); i++) {
            if (buffers_[i]) alDeleteBuffers(1, &buffers_[i]);
        }

        for (size_t i = 0; i < sources_.size(); i++) {
            if (sources_[i]) alDeleteSources(1, &sources_[i]);
        }

        alcMakeContextCurrent(nullptr);
        alcDestroyContext((ALCcontext*)al_context_);
        alcCloseDevice((ALCdevice*)al_device_);        
    }

    decoder_mgr_ = nullptr;
    res_mgr_ = nullptr;
    buffer_map_.clear();
    source_map_.clear();
}

void audio_mgr::play(const std::string& url) {
    if (!al_device_) return;

    ALuint source = (ALuint) get_source(url);
    if (source) {
        alSourcePlay(source);
        return;
    }

    std::weak_ptr<audio_mgr> mgr = shared_from_this();
    res_mgr_->get_audio_data(url, [mgr, url](
            const std::shared_ptr<data_buffer> buffer) {
        auto mgr_ = mgr.lock();
        if (!mgr_) return;

        mgr_->decode(url, buffer);
    });
}

void audio_mgr::pause(const std::string& url) {
    if (!al_device_) return;

    ALuint source = (ALuint) get_source(url);
    if (source) alSourcePause(source);
}

void audio_mgr::stop(const std::string& url) {
    if (!al_device_) return;

    ALuint source = (ALuint) get_source(url);
    if (source) alSourceStop(source);
}

void audio_mgr::destroy(const std::string& url) {
    if (!al_device_) return;

    ALuint source = (ALuint) get_source(url);
    if (source) {
        alDeleteSources(1, &source);

        for (size_t i = 0; i < sources_.size(); i++) {
            if (sources_[i] == source) sources_[i] = 0;
        }

        remove_source(source);
    }

    ALuint buffer = (ALuint) get_buffer(url);
    if (buffer) {
        alDeleteBuffers(1, &buffer);

        for (size_t i = 0; i < buffers_.size(); i++) {
            if (buffers_[i] == buffer) buffers_[i] = 0;
        }

        remove_buffer(buffer);
    }
}

void audio_mgr::seek(const std::string& url, float pos) {
    if (!al_device_) return;

    ALuint buffer = (ALuint) get_buffer(url);
    ALuint source = (ALuint) get_source(url);
    if (buffer && source) {
        ALint tot = 0;
        alGetBufferi(buffer, AL_SIZE, &tot);
        alSourcei(source, AL_BYTE_OFFSET, (int) (pos * tot));
        alSourcePlay(source);
    }
}

uint32_t audio_mgr::get_buffer(const std::string& url) {
    auto iter = buffer_map_.find(url);
    if (iter != buffer_map_.end()) {
        return iter->second;
    }
    return 0;
}

uint32_t audio_mgr::get_source(const std::string& url) {
    auto iter = source_map_.find(url);
    if (iter != source_map_.end()) {
        return iter->second;
    }
    return 0;
}

void audio_mgr::remove_buffer(uint32_t buffer) {
    for (size_t i = 0; i < buffers_.size(); i++) {
        if (buffers_[i] == buffer) {
            buffers_[i] = 0;
            break;
        }
    }
}

void audio_mgr::remove_source(uint32_t source) {
    for (size_t i = 0; i < sources_.size(); i++) {
        if (sources_[i] == source) {
            sources_[i] = 0;
            break;
        }
    }
}

void audio_mgr::decode(const std::string& url,
                       std::shared_ptr<data_buffer> buffer) {
    std::weak_ptr<audio_mgr> mgr = shared_from_this();
    decoder_mgr_->decode(buffer, [mgr, url](
            audio_format format, std::shared_ptr<data_buffer> audio_buffer, size_t size, size_t freq) {
        auto mgr_ = mgr.lock();
        if (!mgr_) return;

        ALuint buffer;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, (ALenum) format,
                     audio_buffer->data(), (ALsizei) size, (ALsizei) freq);

        ALuint source;
        alGenSources(1, &source);
        alSourceQueueBuffers(source, 1, &buffer);

        alSourcePlay(source);

        mgr_->buffers_.push_back(buffer);
        mgr_->buffer_map_[url] = buffer;
        mgr_->sources_.push_back(source);
        mgr_->source_map_[url] = source;
    });
}