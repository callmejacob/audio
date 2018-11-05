* openal编译(android): 

(https://github.com/kcat/openal-soft)

git clone https://github.com/kcat/openal-soft.git

cd openal-soft/build

cmake -DANDROID_PLATFORM=android-19 -DANDROID_NDK=$NDK_ROOT -DANDROID_ABI=armeabi-v7a -DCMAKE_BUILD_TYPE=debug -DCMAKE_TOOLCHAIN_FILE=$NDK_ROOT/build/cmake/android.toolchain.cmake -DANDROID_STL=c++_static -DANDROID_ARM_MODE=arm -DLIBTYPE=STATIC ..

make

上述命令需要注意，NDK_ROOT一定要r14以下的，否则mac上android studio中编译容易出现链接错误。

