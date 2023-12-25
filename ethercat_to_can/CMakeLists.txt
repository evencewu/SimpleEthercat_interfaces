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
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(soem_ros2 REQUIRED)

add_library(ethercat_to_can SHARED
  include/ecat_can_base/ecat_typedef.hpp
  include/ecat_can_base/ecat_base.hpp
  include/ecat_motor_dlc/ecat_DM4310.hpp
  include/ecat_motor_dlc/ecat_GM6020.hpp
  src/ecat_can_base/ecat_base.cpp
  src/ecat_motor_dlc/ecat_DM4310.cpp
  src/ecat_motor_dlc/ecat_GM6020.cpp
  src/test_demo_THD.cpp
)

ament_target_dependencies(ethercat_to_can
        PUBLIC
        rclcpp
        soem_ros2
        )

target_include_directories(ethercat_to_can PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )


ament_export_targets(ethercat_to_can HAS_LIBRARY_TARGET)
ament_export_dependencies(
        rclcpp
        soem_ros2
)

# 注册 导出头文件
install(
        DIRECTORY include/
        DESTINATION include
)

# 注册 导出库文件
install(
        TARGETS ethercat_to_can
        EXPORT ethercat_to_can
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

ament_package()


