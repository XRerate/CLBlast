cmake -Bbuild \
	-GNinja \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI=arm64-v8a \
	-DOPENCL_INCLUDE_DIRS=/home/david/third_party/include \
	.

cmake --build build