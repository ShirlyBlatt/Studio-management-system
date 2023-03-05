cmake_minimum_required(VERSION 3.20)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_SOURCE_DIR src)
add_executable(Project1 include/Action.h include/Customer.h include/Studio.h include/Trainer.h include/Workout.h src/main.cpp src/Action.cpp src/Customer.cpp src/Studio.cpp src/Trainer.cpp src/Workout.cpp)
