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
    set_property(SOURCE ${output_name}.c ${output_name}.h PROPERTY SKIP_AUTOMOC ON)
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

    delegates/basedelegate.cpp
    gstqtvideosinkplugin.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/gstqtvideosinkmarshal.c
    painters/videomaterial.cpp
    painters/videonode.cpp

    delegates/qtquick2videosinkdelegate.cpp
    gstqtquick2videosink.cpp
)

add_definitions(-DQTVIDEOSINK_NAME=${QTVIDEOSINK_NAME})

find_package(OpenGL)
if (OpenGL_FOUND)
    set(GstQtVideoSink_GL_LIBS ${OPENGL_gl_LIBRARY})
    include_directories(${OPENGL_INCLUDE_DIR})
else()
    find_package(OpenGLES2 REQUIRED)
    set(GstQtVideoSink_GL_LIBS ${OPENGLES2_LIBRARY})
    include_directories(${OPENGLES2_INCLUDE_DIR})
endif()

add_library(gst${QTVIDEOSINK_NAME} MODULE ${GstQtVideoSink_SRCS})
target_link_libraries(gst${QTVIDEOSINK_NAME}
    PkgConfig::GStreamerVideo
    ${GstQtVideoSink_GL_LIBS}
    Qt::Quick
)

install(TARGETS gst${QTVIDEOSINK_NAME} DESTINATION ${KDE_INSTALL_LIBDIR}/gstreamer-${GSTREAMER_ABI_VERSION})

if(BUILD_TESTING)
    find_package(Qt${QT_MAJOR_VERSION} NO_MODULE REQUIRED COMPONENTS OpenGL Test)

    ecm_add_test(
            autotest.cpp
            utils/utils.cpp
            utils/bufferformat.cpp
            painters/genericsurfacepainter.cpp
            painters/openglsurfacepainter.cpp
            ${GstQtVideoSink_test_GL_SRCS}
        LINK_LIBRARIES
            PkgConfig::GStreamerVideo
            ${GstQtVideoSink_GL_LIBS}
            Qt::Test
            Qt::OpenGL
            Qt::Quick
            Qt::Widgets
        TEST_NAME qtvideosink_autotest
    )
endif()
