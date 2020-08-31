# Install script for directory: /Users/zarathustra/git/opencv-2.4.13.3/modules/java

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "java" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/OpenCV/java" TYPE FILE FILES "/Users/zarathustra/git/opencv-2.4.13.3/release/bin/opencv-2413.jar")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "java" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/OpenCV/java" TYPE SHARED_LIBRARY FILES "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_java2413.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java2413.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java2413.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "lib/libopencv_java2413.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_calib3d.2.4.dylib" "libopencv_calib3d.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_contrib.2.4.dylib" "libopencv_contrib.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_core.2.4.dylib" "libopencv_core.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_features2d.2.4.dylib" "libopencv_features2d.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_flann.2.4.dylib" "libopencv_flann.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_gpu.2.4.dylib" "libopencv_gpu.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_highgui.2.4.dylib" "libopencv_highgui.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_imgproc.2.4.dylib" "libopencv_imgproc.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_legacy.2.4.dylib" "libopencv_legacy.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_ml.2.4.dylib" "libopencv_ml.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_nonfree.2.4.dylib" "libopencv_nonfree.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_objdetect.2.4.dylib" "libopencv_objdetect.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_ocl.2.4.dylib" "libopencv_ocl.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_photo.2.4.dylib" "libopencv_photo.2.4.dylib"
      -change "/Users/zarathustra/git/opencv-2.4.13.3/release/lib/libopencv_video.2.4.dylib" "libopencv_video.2.4.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java2413.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java2413.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java2413.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/zarathustra/git/opencv-2.4.13.3/release/modules/java/test/cmake_install.cmake")

endif()

