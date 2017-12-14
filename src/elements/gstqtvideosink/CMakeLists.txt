find_program(GLIB2_GENMARSHAL_UTIL glib-genmarshal)
set(GSTREAMER_ABI_VERSION "1.0")

macro(glib2_genmarshal output_name)
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/genmarshal_tmp)
    foreach(_declaration ${ARGN})
        file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/genmarshal_tmp "${_declaration}\n")
    endforeach()
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${output_name}.h ${CMAKE_CURRENT_BINARY_DIR}/${output_name}.c
        COMMAND ${GLIB2_GENMARSHAL_UTIL} --header genmarshal_tmp > ${output_name}.h
        COMMAND ${GLIB2_GENMARSHAL_UTIL} --body genmarshal_tmp > ${output_name}.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endmacro()

glib2_genmarshal(gstqtvideosinkmarshal
    VOID:POINTER,FLOAT,FLOAT,FLOAT,FLOAT
    VOID:POINTER,DOUBLE,DOUBLE,DOUBLE,DOUBLE
    POINTER:POINTER,FLOAT,FLOAT,FLOAT,FLOAT
    POINTER:POINTER,DOUBLE,DOUBLE,DOUBLE,DOUBLE
)

add_definitions(-fexceptions)

set(GstQtVideoSink_SRCS
    utils/utils.cpp
    utils/bufferformat.cpp

    painters/genericsurfacepainter.cpp

    delegates/basedelegate.cpp
    delegates/qtvideosinkdelegate.cpp
    delegates/qwidgetvideosinkdelegate.cpp

    gstqtvideosinkplugin.cpp
    gstqtvideosinkbase.cpp
    gstqtvideosink.cpp
    gstqwidgetvideosink.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/gstqtvideosinkmarshal.c

    painters/videomaterial.cpp
    painters/videonode.cpp

    delegates/qtquick2videosinkdelegate.cpp

    gstqtquick2videosink.cpp

    painters/openglsurfacepainter.cpp
    gstqtglvideosinkbase.cpp
    gstqtglvideosink.cpp
)

add_definitions(
    -DQTVIDEOSINK_NAME=qt5videosink
    -DQTGLVIDEOSINK_NAME=${QTGLVIDEOSINK_NAME}
    -DQWIDGETVIDEOSINK_NAME=${QWIDGETVIDEOSINK_NAME}
)

find_package(OpenGL REQUIRED)
if (OPENGLES2_FOUND)
    set(GstQtVideoSink_GL_LIBS ${OPENGLES2_LIBRARY})
    include_directories(${OPENGLES2_INCLUDE_DIR})
else()
    set(GstQtVideoSink_GL_LIBS ${OPENGL_gl_LIBRARY})
    include_directories(${OPENGL_INCLUDE_DIR})
endif()

add_library(gstqt5videosink MODULE ${GstQtVideoSink_SRCS})
target_link_libraries(gstqt5videosink
    ${GOBJECT_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${GSTREAMER_BASE_LIBRARY}
    ${GSTREAMER_VIDEO_LIBRARY}
    ${GstQtVideoSink_GL_LIBS}
    Qt5::OpenGL
    Qt5::Quick
    Qt5::Widgets
)

install(TARGETS gstqt5videosink DESTINATION ${CMAKE_INSTALL_LIBDIR}/gstreamer-${GSTREAMER_ABI_VERSION})

add_executable(qtvideosink_autotest
    autotest.cpp
    utils/utils.cpp
    utils/bufferformat.cpp
    painters/genericsurfacepainter.cpp
    painters/openglsurfacepainter.cpp
    ${GstQtVideoSink_test_GL_SRCS}
)
target_link_libraries(qtvideosink_autotest
    ${GOBJECT_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${GSTREAMER_BASE_LIBRARY}
    ${GSTREAMER_VIDEO_LIBRARY}
    ${GstQtVideoSink_GL_LIBS}
    Qt5::Test
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::Quick
)
