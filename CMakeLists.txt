cmake_minimum_required(VERSION 3.8)
project(cam_test)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# export COMPILE_COMMANDS for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)


include_directories(include)

# add the armor_detector executable
add_library(${PROJECT_NAME} SHARED 
  src/cam_pub.cpp
)
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  cv_bridge
  image_transport
  OpenCV
)
target_link_libraries(${PROJECT_NAME} 
  ${InferenceEngine_LIBRARIES}
)
rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "image_pub"
  EXECUTABLE armor_detector_node
)

add_executable(cam_pub src/cam_pub.cpp)
ament_target_dependencies(cam_pub rclcpp std_msgs cv_bridge image_transport OpenCV)

# Install binaries
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install headers
install(
  DIRECTORY include/
  DESTINATION include
)

# Install launch files
install(DIRECTORY
    launch
    config
    model
    DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
