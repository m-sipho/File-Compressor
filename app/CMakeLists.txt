cmake_minimum_required(VERSION 3.16)

# Define the executable target
add_executable(CompressorApp src/main.cpp)

# Add usage requirements to the executable
target_compile_features(CompressorApp PUBLIC cxx_std_20)

# Link the library to the executable
target_link_libraries(CompressorApp CompressorLib)