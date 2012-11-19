cmake_minimum_required (VERSION 2.6)
project (wavutils)

set(CMAKE_BUILD_TYPE Release)

add_executable(wavcut wavcut.cpp)
target_link_libraries(wavcut sndfile)

add_executable(wavlen wavlen.cpp)
target_link_libraries(wavlen sndfile)

add_executable(wavcat wavcat.cpp)
target_link_libraries(wavcat sndfile)

add_executable(wav2mono wav2mono.cpp)
target_link_libraries(wav2mono sndfile)


