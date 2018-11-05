#include "lib.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

using namespace audio_al;

int feature::get_result(int argc, char** argv) {
    return argc + 1;
}

