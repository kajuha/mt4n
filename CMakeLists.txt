################################################################################
# CMake
################################################################################
cmake_minimum_required(VERSION 2.8.3)
project(mt4n)

################################################################################
# Packages
################################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  tf
)

################################################################################
# Declare ROS messages, services and actions
################################################################################

################################################################################
# Declare ROS dynamic reconfigure parameters
################################################################################

################################################################################
# Catkin specific configuration
################################################################################
catkin_package(  
  LIBRARIES mt4n
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs tf
#   DEPENDS system_lib
)

################################################################################
# Build
################################################################################
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS libmodbus REQUIRED)

include_directories(  
  ${catkin_INCLUDE_DIRS}
  ${LIBMODBUS_INCLUDE_DIRS}
)

add_executable(mt4n src/mt4n.cpp)
add_dependencies(mt4n ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(mt4n ${catkin_LIBRARIES} ${LIBMODBUS_LIBRARIES})

# I knew the REP0003(http://www.ros.org/reps/rep-0003.html#c),
# but the code of "mt4n" must compile with C++11.
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


################################################################################
# Install
################################################################################
install(TARGETS mt4n
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(DIRECTORY launch
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY rviz_cfg
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

################################################################################
# Test
#############################################################################