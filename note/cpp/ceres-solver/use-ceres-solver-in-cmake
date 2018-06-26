# enable c++11 support
set(CMAKE_CXX_STANDARD 11)

find_package(Ceres REQUIRED)
#find_package(Ceres REQUIRED PATHS <directory-contains-CeresConfig.cmake>)

include_directories(${CERES_INCLUDE_DIRS})

add_executable(out main.cpp)
target_link_libraries(out ${CERES_LIBRARIES})
