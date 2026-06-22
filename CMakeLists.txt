cmake_minimum_required(VERSION 3.16)

# Name the overall project
project(FileCompressor)

# Bring in our components
add_subdirectory(compressor_lib) # The library
add_subdirectory(app) # The executable