cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME Payroll)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
	main.cpp)


#Include(FetchContent)

#FetchContent_Declare(
#        Catch2
#        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#        GIT_TAG        v2.13.1)

#FetchContent_MakeAvailable(Catch2)

#add_executable(tests)

#target_link_libraries(tests PRIVATE Catch2::Catch2)
