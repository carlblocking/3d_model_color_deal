cmake_minimum_required(VERSION 3.6.0)

set(OSG_INCLUDE_PATH "D:/osg_pre_build/OpenSceneGraph-3.6.5-VC2022-64-Release-2023-01/include")

set(OSG_DEBUG_LIB_PATH "D:/osg_pre_build/OpenSceneGraph-3.6.5-VC2022-64-Debug-2023-01/lib")
set(OSG_RELEASE_LIB_PATH "D:/osg_pre_build/OpenSceneGraph-3.6.5-VC2022-64-Release-2023-01/lib")

set(OPENCV_INCLUDE_PATH "D:/opencv/build/include")
set(OPENCV_LIB_PATH "D:/opencv/build/x64/vc15/lib")

set(QT_INCLUDE_PATH "D:/Qt/5.12.12/msvc2017_64/include")
set(QT_LIB_PATH "D:/Qt/5.12.12/msvc2017_64/lib")

include_directories(
    ${OSG_INCLUDE_PATH}
    ${OPENCV_INCLUDE_PATH}
    ${QT_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/include
)

link_libraries(
    optimized ${OPENCV_LIB_PATH}/opencv_world460d.lib
    debug ${OPENCV_LIB_PATH}/opencv_world460.lib

    optimized ${OSG_RELEASE_LIB_PATH}/osg.lib
    optimized ${OSG_RELEASE_LIB_PATH}/osgdb.lib

    debug ${OSG_DEBUG_LIB_PATH}/osgd.lib
    debug ${OSG_DEBUG_LIB_PATH}/osgdbd.lib
)

add_executable(
    main
    ${CMAKE_SOURCE_DIR}/include/NodeImageVisitor.cpp
    src/test_node_image_visitor.cpp
)