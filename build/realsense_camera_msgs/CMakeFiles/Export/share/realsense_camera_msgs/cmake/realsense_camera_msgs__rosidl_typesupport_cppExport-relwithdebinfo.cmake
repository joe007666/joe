#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "realsense_camera_msgs::realsense_camera_msgs__rosidl_typesupport_cpp" for configuration "RelWithDebInfo"
set_property(TARGET realsense_camera_msgs::realsense_camera_msgs__rosidl_typesupport_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(realsense_camera_msgs::realsense_camera_msgs__rosidl_typesupport_cpp PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/librealsense_camera_msgs__rosidl_typesupport_cpp.so"
  IMPORTED_SONAME_RELWITHDEBINFO "librealsense_camera_msgs__rosidl_typesupport_cpp.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS realsense_camera_msgs::realsense_camera_msgs__rosidl_typesupport_cpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_realsense_camera_msgs::realsense_camera_msgs__rosidl_typesupport_cpp "${_IMPORT_PREFIX}/lib/librealsense_camera_msgs__rosidl_typesupport_cpp.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
