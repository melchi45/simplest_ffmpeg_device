cmake_minimum_required(VERSION 2.8)

set ( PROJECT_NAME simplest_ffmpeg_device )
set ( FFMPEG_READCAMERA simplest_ffmpeg_readcamera )
set ( FFMPEG_GRABDESKTOP simplest_ffmpeg_grabdesktop )

project( ${PROJECT_NAME} )
set ( ${PROJECT_NAME}_HOME ${CMAKE_CURRENT_SOURCE_DIR} )
message("SourceDir: " ${${PROJECT_NAME}_HOME} )

# set where to find additional cmake modules if any
# comment it out if not required
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
message ( "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )

if ( NOT DEFINED BUILD_SHARED_LIBS )
   set ( BUILD_SHARED_LIBS FALSE )
endif ()

if ( CMAKE_COMPILER_IS_GNUCXX )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive" )
endif ()

# set compiler support for C++11 standard
option(USE_CXX11_STD "Product should be build with C++11 compiler option enabled" ON)

if(USE_CXX11_STD)
    set(CMAKE_CXX_STANDARD 11)
endif()

if(MSVC)
	add_definitions(-D_CONSOLE)
else()
    # GCC or Clang
endif()

#set ( THREADS_USE_PTHREADS_WIN32 TRUE )
#find_package(Threads REQUIRED)

#message ( "THREADS_PTHREADS_INCLUDE_DIR = ${THREADS_PTHREADS_INCLUDE_DIR}" )
#message ( "THREADS_PTHREADS_WIN32_LIBRARY = ${THREADS_PTHREADS_WIN32_LIBRARY}" )
#message ( "CMAKE_THREAD_LIBS_INIT = ${CMAKE_THREAD_LIBS_INIT}" )


#find_package(OpenGL REQUIRED)
#if(OPENGL_FOUND)
#message("OPENGL_LIBRARIES = ${OPENGL_LIBRARIES} ")
#message("OPENGL_INCLUDE_DIRS = ${OPENGL_INCLUDE_DIRS} ")
#else( OPENGL_FOUND )
#    message(FATAL_ERROR "OpenGL not found")
#endif( OPENGL_FOUND )

#message("CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}")

#find_package(glfw3 REQUIRED) # First try to find using the find_package method (works with vcpkg)
#if(GLFW_FOUND) # Works with some other methods
#	message(STATUS "GLFW3_INCLUDE_PATH=${GLFW3_INCLUDE_PATH}")
#	message(STATUS "GLFW_LIBRARIES=${GLFW_LIBRARIES}")
#else( GLFW_FOUND )
#    message(FATAL_ERROR "GLFW3 not found")
#endif( GLFW_FOUND )

find_package(FFmpeg REQUIRED)
if(FFMPEG_FOUND)
  message("Found FFMPEG/LibAV libraries")
  message("FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS} ")
  message("FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES} ")
  message("FFMPEG_DEFINITIONS = ${FFMPEG_DEFINITIONS} ")
  
  if( SWSCALE_FOUND )
	message("Found SWSCALE_LIBRARIES = ${SWSCALE_LIBRARIES}")
  endif()
  
  if( SWRESAMPLE_FOUND )
	message("Found SWRESAMPLE_LIBRARIES = ${SWRESAMPLE_LIBRARIES}")
  endif()
  
  if( AVDEVICE_FOUND )
	message("Found AVDEVICE_LIBRARIES = ${AVDEVICE_LIBRARIES}")
  endif()
  
  if( AVFILTER_FOUND )
	message("Found AVFILTER_LIBRARIES = ${AVFILTER_LIBRARIES}")
  endif()
  
  if( POSTPROC_FOUND )
	message("Found POSTPROC_LIBRARIES = ${POSTPROC_LIBRARIES}")
  endif()
else( FFMPEG_FOUND )
    message(FATAL_ERROR "FFMPEG not found")
endif( FFMPEG_FOUND )

find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
  #include_directories(${SDL2_INCLUDE_DIR})
  message("Found SDL2 libraries")
  message("SDL2_INCLUDE_DIR = ${SDL2_INCLUDE_DIR} ")
  message("SDL2_LIBRARY = ${SDL2_LIBRARY} ")
else ( SDL2_FOUND )
  message(FATAL_ERROR "SDL2 not found")
endif (SDL2_FOUND)


# use standard file endings - rename to cxx if needed
set(CXX_SRC_EXT_PATTERN ".cpp")
set(C_SRC_EXT_PATTERN ".c")
set(HDR_EXT_PATTERN ".h")

# get all *.cpp files from src to build target
file(GLOB FFMPEG_READCAMERA_FILES ${PROJECT_SOURCE_DIR}/src/${FFMPEG_READCAMERA}.cpp)
file(GLOB FFMPEG_GRABDESKTOP_FILES ${PROJECT_SOURCE_DIR}/src/${FFMPEG_GRABDESKTOP}.cpp)
file(GLOB HDR_FILES ${PROJECT_SOURCE_DIR}/src/*${HDR_EXT_PATTERN})
# makes source and header files group
source_group("Source Files" FILES ${SRC_FILES})
source_group("Header Files" FILES ${HDR_FILES})

# build target
#add_executable( ${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES} )
add_executable(${FFMPEG_READCAMERA} ${FFMPEG_READCAMERA_FILES} ${HDR_FILES})
add_executable(${FFMPEG_GRABDESKTOP} ${FFMPEG_GRABDESKTOP_FILES} ${HDR_FILES})

target_include_directories( ${FFMPEG_READCAMERA} 
	PRIVATE 
		${FFMPEG_INCLUDE_DIRS} 
)

target_link_libraries( ${FFMPEG_READCAMERA} 
	PRIVATE
		${FFMPEG_LIBRARIES} ${SWSCALE_LIBRARIES} ${AVDEVICE_LIBRARIES} ${AVFILTER_LIBRARIES} ${SDL2_LIBRARY}
)

target_include_directories( ${FFMPEG_GRABDESKTOP} 
	PRIVATE 
		${FFMPEG_INCLUDE_DIRS} 
)

target_link_libraries( ${FFMPEG_GRABDESKTOP} 
	PRIVATE
		${FFMPEG_LIBRARIES} ${SWSCALE_LIBRARIES} ${AVDEVICE_LIBRARIES} ${AVFILTER_LIBRARIES} ${SDL2_LIBRARY}
)


add_custom_command (
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${PROJECT_NAME}
    COMMENT "Execute ${PROJECT_NAME}"
)