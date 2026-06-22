# Set the minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Define the library target
add_library(CompressorLib
	src/huffmanTree.cpp
	src/fileHandler.cpp
)

# Allow any target linking this library to see the include folder
target_include_directories(CompressorLib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(CompressorLib PUBLIC cxx_std_20)