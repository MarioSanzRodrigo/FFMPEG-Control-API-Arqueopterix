cmake_minimum_required(VERSION 2.6)
find_package(PkgConfig)
#functions
function(test_lib LIB_NAME)
  if (${LIB_NAME} MATCHES "^.*-NOTFOUND")
    message(FATAL_ERROR "lib not found: " ${LIB_NAME} " check README")
    return()
  endif(${LIB_NAME} MATCHES "^.*-NOTFOUND")
endfunction(test_lib)
project (PTLOADER)

set(CMAKE_MODULE_PATH "cmake/")
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fno-stack-protector")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/libs/")

#file(GLOB_RECURSE SOURCES_PROT1 "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/prototype1.cpp")
#add_executable(prototype1 ${SOURCES_PROT1})

#file(GLOB_RECURSE SOURCES_PROT2 "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/prototype2.cpp")
#add_executable(prototype2 ${SOURCES_PROT2})

#file(GLOB_RECURSE SOURCES_PROT3 "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/prototype3.cpp")
#add_executable(prototype3 ${SOURCES_PROT3})

file(GLOB_RECURSE SOURCES_PROT4SEND "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/prototype4_send.cpp")
add_executable(prototype4_send ${SOURCES_PROT4SEND})

file(GLOB_RECURSE SOURCES_PROT4REC "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/prototype4_rec.cpp")
add_executable(prototype4_rec ${SOURCES_PROT4REC})

# GLIB
find_package(Glib REQUIRED)
include_directories(${GLIB_INCLUDE_DIRS})

# BOOST
set (BOOST_LIBS thread regex system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

# GTHREAD
find_library(GTHREAD gthread-2.0 HINTS)
test_lib(${GTHREAD})

# OPENSSL
find_library(SSL ssl HINTS)
test_lib(${SSL})
find_library(CRYPTO crypto HINTS)
test_lib(${CRYPTO})

# Math and Zlib
find_library(MATH m)
test_lib(${MATH})
find_library(ZLIB z)
test_lib(${ZLIB})
find_library(LZMA lzma)
test_lib(${LZMA})

find_library(AVRESAMP swresample)
test_lib(${AVRESAMP})
find_library(BZ2 bz2)
test_lib(${BZ2})
find_library(SWSCALE swscale)
test_lib(${SWSCALE})

find_library(LVA va)
test_lib(${LVA})
find_library(LX11 X11)
test_lib(${LX11})
find_library(LVDPAU vdpau)
test_lib(${LVDPAU})
find_library(LVADRM va-drm)
test_lib(${LVADRM})
find_library(LVAx11 va-x11)
test_lib(${LVAx11})
find_library(XCB xcb)
test_lib(${XCB})
find_library(XEXT Xext)
test_lib(${XEXT})
find_library(ASOUND asound)
test_lib(${ASOUND})
find_library(XV Xv)
test_lib(${XV})
find_library(VDPAU vdpau)
test_lib(${VDPAU})
find_library(XCBFI xcb-xfixes)
test_lib(${XCBFI})
find_library(XCBSHAPE xcb-shape)
test_lib(${XCBSHAPE})
find_library(XCBSHM xcb-shm)
test_lib(${XCBSHM})
find_library(SDL SDL)
test_lib(${SDL})

# Libav
find_library(AVUTIL avutil)
test_lib(${AVUTIL})
set (LIBS ${AVUTIL})
find_library(AVCODEC avcodec)
test_lib(${AVCODEC})
set (LIBS ${LIBS} ${AVCODEC})
find_library(AVFORMAT avformat)
test_lib(${AVFORMAT})
set (LIBS ${LIBS} ${AVFORMAT})
find_library(AVDEVICE avdevice)
test_lib(${AVDEVICE})
set (LIBS ${LIBS} ${AVDEVICE})
find_library(AVFILTER avfilter)
test_lib(${AVFILTER})
set (LIBS ${LIBS} ${AVFILTER})

# LOG4CXX
find_library(LOG log4cxx)
test_lib(${LOG})

# OpenCV
set(OpenCV_DIR /usr/local/share/OpenCV)
FIND_PACKAGE(OpenCV REQUIRED)

message("Libs ${GTHREAD} ${SSL} ${CRYPTO} ${LIBS} ${LOG}")

#target_link_libraries(prototype1 ${GLIB_LIBRARIES})
#target_link_libraries(prototype1 ${Boost_LIBRARIES})
#target_link_libraries(prototype1 ${GTHREAD} ${SSL} ${CRYPTO} ${BZ2} ${AVRESAMP} ${LZMA} ${MATH} ${ZLIB} ${LIBS} ${LX11} ${LVA} ${LVADRM} ${LVAx11} ${LVDPAU} ${LOG})

#target_link_libraries(prototype2 ${GLIB_LIBRARIES})
#target_link_libraries(prototype2 ${Boost_LIBRARIES})
#target_link_libraries(prototype2 ${GTHREAD} ${SSL} ${CRYPTO} ${BZ2} ${AVRESAMP} ${LZMA} ${MATH} ${ZLIB} ${LIBS} ${LX11} ${LVA} ${LVADRM} ${LVAx11} ${LVDPAU}  ${LOG})

#target_link_libraries(prototype3 ${GLIB_LIBRARIES})
#target_link_libraries(prototype3 ${Boost_LIBRARIES})
#target_link_libraries(prototype3 ${GTHREAD} ${SSL} ${CRYPTO} ${BZ2} ${AVRESAMP} ${LZMA} ${MATH} ${ZLIB} ${LIBS} ${LX11} ${LVA} ${LVADRM} ${LVAx11} ${LVDPAU}  ${LOG})

target_link_libraries(prototype4_send ${GLIB_LIBRARIES})
target_link_libraries(prototype4_send ${Boost_LIBRARIES})
target_link_libraries(prototype4_send ${GTHREAD} ${SSL} ${CRYPTO} ${BZ2} ${AVRESAMP} ${LZMA} ${MATH} ${ZLIB} ${LIBS} ${LX11} ${LVA} ${LVADRM} ${LVAx11} ${LVDPAU} ${LOG} ${XCB} ${XEXT} ${ASOUND} ${XV} ${VDPAU} ${XCBFI} ${XCBSHAPE} ${XCBSHM} ${SDL} ${SWSCALE} ${OpenCV_LIBS})

target_link_libraries(prototype4_rec ${GLIB_LIBRARIES})
target_link_libraries(prototype4_rec ${Boost_LIBRARIES})
target_link_libraries(prototype4_rec ${GTHREAD} ${SSL} ${CRYPTO} ${BZ2} ${AVRESAMP} ${LZMA} ${MATH} ${ZLIB} ${LIBS} ${LX11} ${LVA} ${LVADRM} ${LVAx11} ${LVDPAU} ${LOG} ${XCB} ${XEXT} ${ASOUND} ${XV} ${VDPAU} ${XCBFI} ${XCBSHAPE} ${XCBSHM} ${SDL} ${SWSCALE} ${OpenCV_LIBS})

# check lib dependencies pkg-config --libs libavdevice