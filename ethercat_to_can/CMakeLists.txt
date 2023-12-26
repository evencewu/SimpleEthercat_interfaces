cmake_minimum_required(VERSION 3.5)
project(ethercat_to_can)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Woverloaded-virtual)
endif ()

# find dependencies
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

ament_auto_add_library(${PROJECT_NAME} SHARED
        src/ecat_can_base/ecat_base.cpp
        src/ecat_motor_dlc/ecat_DM4310.cpp
        src/ecat_motor_dlc/ecat_GM6020.cpp
)

ament_auto_add_executable(test_demo_gm6020
  src/ecat_can_base/ecat_base.cpp
  src/ecat_motor_dlc/ecat_DM4310.cpp
  src/ecat_motor_dlc/ecat_GM6020.cpp
  src/test_demo_GM6020.cpp
)

ament_auto_add_executable(test_demo_dm4310
  src/ecat_can_base/ecat_base.cpp
  src/ecat_motor_dlc/ecat_DM4310.cpp
  src/ecat_motor_dlc/ecat_GM6020.cpp
  src/test_demo_DM4310.cpp
)


#############
## Testing ##
#############

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


#############
## Install ##
#############

ament_auto_package(
  INSTALL_TO_SHARE
)


