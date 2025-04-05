cmake_minimum_required(VERSION 3.16)
project(untitled)  # ← この名前が重要！

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp)  # ← ここも「untitled」に合わせる

# Win32 APIを使う場合
target_link_libraries(untitled user32 gdi32)
