cmake_minimum_required(VERSION 3.28.0)

project(Gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)#c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/cfitsio/include)
include_directories(${CMAKE_SOURCE_DIR}/opencv/include)
include_directories(${CMAKE_SOURCE_DIR}/zlib/include)
include_directories(${CMAKE_SOURCE_DIR}/Hfile)

# 设置源文件
set(PROJECT_SOURCES
    src/main.cpp
    src/fitsmerger.cpp
    src/filehandler.cpp
    src/Smooth.cpp
    src/fitsaligner.cpp
    src/mainwindow.cpp
    src/mainwindow.ui
    src/mainwindow.h
    src/fitscalibration.cpp
    src/bilateral.cpp
    src/baryer.cpp
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR})
else()
    if(ANDROID)
        add_library(GUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GUI
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR})
endif()

# 添加可执行文件
add_executable(GUI ${SOURCE_FILES})

#链接动态库
# target_link_libraries(GUI
#     cfitsio.dll
#     zlibd.dll
#     opencv_world480d.dll
#     PRIVATE
#     Qt${QT_VERSION_MAJOR}::Widgets
# )

target_link_libraries(GUI PRIVATE cfitsio.dll)
target_link_libraries(GUI PRIVATE zlibd.dll)
target_link_libraries(GUI PRIVATE opencv_world480d.dll)
target_link_libraries(GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# #target_link_libraries(GUI opencv_videoio_ffmpeg480_64.dll)
# #target_link_libraries(GUI opencv_videoio_msmf480_64.dll)
# #target_link_libraries(GUI opencv_videoio_msmf480_64d.dll)
# #target_link_libraries(GUI opencv_world480.dll)

# 链接库
target_link_directories(GUI PUBLIC ${CMAKE_SOURCE_DIR}/cfitsio/lib)
target_link_directories(GUI PUBLIC ${CMAKE_SOURCE_DIR}/zlib/lib)
target_link_directories(GUI PUBLIC ${CMAKE_SOURCE_DIR}/opencv/lib)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GUI)
endif()

set_target_properties(GUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#将所用的dll文件复制到构建目录下以便使用
configure_file(${CMAKE_SOURCE_DIR}/cfitsio/bin/cfitsio.dll ${CMAKE_BINARY_DIR}/cfitsio.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/zlib/bin/zlibd.dll ${CMAKE_BINARY_DIR}/zlibd.dll COPYONLY)
#configure_file(${CMAKE_SOURCE_DIR}/opencv/bin/opencv_world480.dll ${CMAKE_BINARY_DIR}/opencv_world480.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/opencv/bin/opencv_world480d.dll ${CMAKE_BINARY_DIR}/opencv_world480d.dll COPYONLY)
#configure_file(${CMAKE_SOURCE_DIR}/opencv/bin/opencv_videoio_ffmpeg480_64.dll ${CMAKE_BINARY_DIR}/opencv_videoio_ffmpeg480_64.dll COPYONLY)
#configure_file(${CMAKE_SOURCE_DIR}/opencv/bin/opencv_videoio_msmf480_64.dll ${CMAKE_BINARY_DIR}/opencv_videoio_msmf480_64.dll COPYONLY)
#configure_file(${CMAKE_SOURCE_DIR}/opencv/bin/opencv_videoio_msmf480_64d.dll ${CMAKE_BINARY_DIR}/opencv_videoio_msmf480_64d.dll COPYONLY)

#将指定目录下的fits文件复制到构建目录
configure_file(${CMAKE_SOURCE_DIR}/debug/Capture_00001.fits ${CMAKE_BINARY_DIR}/Capture_00001.fits COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/debug/Capture_00002.fits ${CMAKE_BINARY_DIR}/Capture_00002.fits COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/debug/Capture_00003.fits ${CMAKE_BINARY_DIR}/Capture_00003.fits COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/debug/Capture_00004.fits ${CMAKE_BINARY_DIR}/Capture_00004.fits COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/debug/Capture_00005.fits ${CMAKE_BINARY_DIR}/Capture_00005.fits COPYONLY)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GUI)
endif()

