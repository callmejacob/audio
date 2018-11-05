#/bin/sh

# config
platform=android-24
build_types=(debug)
android_abis=(armeabi-v7a arm64-v8a x86)
output=build

if [ $# -gt 0 ]; then
    if [ $1 = 'debug' ]; then
        build_types=(debug)
    elif [ $1 = 'release' ]; then
        build_types=(release)
    elif [ $1 = 'all' ]; then
        build_types=(debug release)
    fi
fi

# build
rm -fr ${output}
mkdir ${output}
cd ${output}

shopt -s extglob
for type in ${build_types[@]}
do

    # enter type
    mkdir ${type}
    cd ${type}

    # build libs
    for abi in ${android_abis[@]}
    do     
        cmake -DANDROID_PLATFORM=${platform} -DANDROID_NDK=$NDK_ROOT -DANDROID_ABI=${abi} -DCMAKE_BUILD_TYPE=${type} -DCMAKE_TOOLCHAIN_FILE=$NDK_ROOT/build/cmake/android.toolchain.cmake -DANDROID_STL=c++_static ../..
        make
        rm -fr !(libs)
    done

    # build header
    cp -r ../../include ./

    # exit type
    cd ..

done

cd ..
