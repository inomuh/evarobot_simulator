cmake_minimum_required(VERSION 2.8.3)
project(evarobot_gazebo)
SET(CMAKE_CXX_FLAGS "-std=c++0x")
find_package(catkin REQUIRED COMPONENTS
  urdf
  message_generation 
  gazebo_msgs 
  roscpp 
  rospy 
  nodelet 
  angles 
  std_srvs 
  geometry_msgs 
  sensor_msgs 
  nav_msgs 
  urdf 
  tf 
  tf2_ros 
  dynamic_reconfigure 
  driver_base 
  rosgraph_msgs 
  trajectory_msgs 
  image_transport 
  rosconsole
  cv_bridge
  polled_camera
  camera_info_manager
  im_msgs
  interactive_marker_twist_server  
)

find_package(Qt4 REQUIRED)
find_package(Protobuf REQUIRED)

include (${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

qt4_wrap_cpp(QT_MOC_HPP include/plugins/evarobot_widget.h)


include_directories(
  ${catkin_INCLUDE_DIRS}
)
include_directories(${Qt4Widgets_INCLUDE_DIRS})



include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(XML libxml-2.0)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(OGRE-Terrain OGRE-Terrain)
  pkg_check_modules(OGRE-Paging OGRE-Paging)
else()
  message(FATAL_ERROR "pkg-config is required; please install it")
endif()

# Depend on system install of Gazebo and SDFormat
find_package(gazebo REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

execute_process(COMMAND
  pkg-config --variable=plugindir OGRE
  OUTPUT_VARIABLE OGRE_PLUGIN_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)



generate_dynamic_reconfigure_options(
  cfg/CameraSynchronizer.cfg
  cfg/GazeboRosCamera.cfg
  cfg/GazeboRosOpenniKinect.cfg
  cfg/Hokuyo.cfg
  cfg/SensorModel.cfg
)

include_directories(include
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIRS}
  ${OGRE-Terrain_INCLUDE_DIRS}
  ${OGRE-Paging_INCLUDE_DIRS}
  include
  hector_gazebo_plugins
)

link_directories(
  ${GAZEBO_LIBRARY_DIRS}
  ${OGRE_LIBRARY_DIRS}
  ${OGRE-Terrain_LIBRARY_DIRS}
  ${OGRE-Paging_LIBRARY_DIRS}
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES 
  vision_reconfigure 
	im_gazebo_ros_sonar
  im_gazebo_ros_bumper
  evarobot_ros_bumper 
  
  CATKIN_DEPENDS 
  message_generation 
  gazebo_msgs 
  roscpp 
  rospy 
  nodelet 
  angles 
  std_srvs 
  geometry_msgs 
  sensor_msgs 
  nav_msgs 
  urdf 
  tf 
  tf2_ros
  dynamic_reconfigure 
  driver_base 
  rosgraph_msgs 
  trajectory_msgs 
  image_transport 
  rosconsole
  camera_info_manager
  DEPENDS 
    gazebo 
    SDF
  )

add_dependencies(${PROJECT_NAME}_gencfg ${catkin_EXPORTED_TARGETS})


add_library(im_gazebo_ros_bumper src/im_gazebo_ros_bumper.cpp)
add_dependencies(im_gazebo_ros_bumper gazebo_msgs_gencpp)
target_link_libraries(im_gazebo_ros_bumper ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} ContactPlugin ${catkin_LIBRARIES})

add_library(evarobot_ros_bumper src/evarobot_ros_bumper.cpp)
add_dependencies(evarobot_ros_bumper im_msgs_gencpp)
target_link_libraries(evarobot_ros_bumper  ${catkin_LIBRARIES}  ${GAZEBO_LIBRARIES})

add_library(im_gazebo_ros_sonar src/im_gazebo_ros_sonar.cpp)
add_dependencies(im_gazebo_ros_sonar ${PROJECT_NAME}_gencfg)
target_link_libraries(im_gazebo_ros_sonar ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES})

add_library(evarobot_widget SHARED ${QT_MOC_HPP} src/evarobot_widget.cpp)
add_dependencies(evarobot_widget ${PROJECT_NAME}_gencfg)
target_link_libraries(evarobot_widget ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES} ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES})

add_library(evarobot_objects src/evarobot_objects.cpp)
add_dependencies(evarobot_objects ${PROJECT_NAME}_gencfg)
target_link_libraries(evarobot_objects ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES})

#add_library(mazefactory src/MazeFactory.cc)
#add_dependencies(mazefactory ${PROJECT_NAME}_gencfg)
#target_link_libraries(mazefactory ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES})

install(TARGETS 
	im_gazebo_ros_sonar
  im_gazebo_ros_bumper
  evarobot_ros_bumper
  evarobot_widget
  evarobot_objects
	
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


install(DIRECTORY include/
   DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
   FILES_MATCHING PATTERN "*.h"
)

foreach(dir cfg launch map worlds)
    install(DIRECTORY ${dir}/
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach()
