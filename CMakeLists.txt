cmake_minimum_required(VERSION 3.0)

project(builtinizer)

add_executable(${PROJECT_NAME} src/main.cpp src/Tokenizer.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
