cmake_minimum_required(VERSION 3.8)
project(manipulator_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(rclcpp REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SERIAL libserial)

# Arduino Serial version (legacy - keep for reference/fallback)
add_library(${PROJECT_NAME}_serial SHARED 
  src/manipulator_interface.cpp
)
target_include_directories(${PROJECT_NAME}_serial PRIVATE
  include
  "${SERIAL_INCLUDE_DIRS}"
)
target_link_libraries(${PROJECT_NAME}_serial
  "${SERIAL_LDFLAGS}"
)
ament_target_dependencies(${PROJECT_NAME}_serial
  hardware_interface
  pluginlib
  rclcpp_lifecycle
  rclcpp
)

# Check if pigpio library is available (required for Raspberry Pi GPIO)
find_library(PIGPIO_LIB pigpio)

if(PIGPIO_LIB)
  message(STATUS "pigpio library found - building GPIO interface")
  
  # Raspberry Pi GPIO version (new default)
  add_library(${PROJECT_NAME} SHARED 
    src/manipulator_interface_gpio.cpp
  )
  target_include_directories(${PROJECT_NAME} PRIVATE
    include
  )
  target_link_libraries(${PROJECT_NAME}
    pigpio
  )
  ament_target_dependencies(${PROJECT_NAME}
    hardware_interface
    pluginlib
    rclcpp_lifecycle
    rclcpp
  )
  
  set(GPIO_BUILD_ENABLED TRUE)
else()
  message(WARNING "pigpio library not found - GPIO interface will not be built")
  message(WARNING "Install pigpio on Raspberry Pi with: sudo apt-get install pigpio")
  message(STATUS "Building serial interface as default")
  
  # Use serial version as main library if pigpio not available
  add_library(${PROJECT_NAME} SHARED 
    src/manipulator_interface.cpp
  )
  target_include_directories(${PROJECT_NAME} PRIVATE
    include
    "${SERIAL_INCLUDE_DIRS}"
  )
  target_link_libraries(${PROJECT_NAME}
    "${SERIAL_LDFLAGS}"
  )
  ament_target_dependencies(${PROJECT_NAME}
    hardware_interface
    pluginlib
    rclcpp_lifecycle
    rclcpp
  )
  
  set(GPIO_BUILD_ENABLED FALSE)
endif()

pluginlib_export_plugin_description_file(hardware_interface manipulator_controller_plugins.xml)

install(
  DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

# Install libraries based on what was built
if(GPIO_BUILD_ENABLED)
  install(
    TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_serial
    DESTINATION lib
  )
else()
  install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib
  )
endif()

install(
  DIRECTORY include
  DESTINATION include
)

ament_export_include_directories(
  include
)

ament_export_libraries(
  ${PROJECT_NAME}
)

ament_export_dependencies(
  hardware_interface
  pluginlib
  rclcpp_lifecycle
  rclcpp
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
