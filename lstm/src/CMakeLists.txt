cmake_minimum_required (VERSION 3.1)

SET(CMAKE_INSTALL_PREFIX "../../libraries")

project(LSTM-PYNQ VERSION 0.1)

#Equivalent to -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#Are we cross compiling?
option(CROSS_COMPILE "Cross compile for PYNQ" OFF)
if(CROSS_COMPILE)
    message("Cross compile to ARM")
    set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
endif()

#Are we running on HW?
option(ON_BOARD "HW acceleration enabled" OFF)
if(ON_BOARD)
    add_definitions(-DON_BOARD=1)
    set(LIB_NAME_PREFIX "hw")
    set(CMAKE_CXX_FLAGS "-O0 -std=c++0x")
else()
    set(LIB_NAME_PREFIX "sw")
    set(CMAKE_CXX_FLAGS "-O2 -std=c++0x")
endif()

#HLS header-only lib 
add_library(hls INTERFACE)
target_include_directories(hls INTERFACE "library/hls")
target_include_directories(hls INTERFACE $ENV{VIVADOHLS_INCLUDE_PATH})

#Driver lib 
file(GLOB DRIVER_SOURCES "library/driver/*.cpp")
add_library(driver STATIC ${DRIVER_SOURCES})
target_include_directories(driver PUBLIC "library/driver")
target_include_directories(driver PUBLIC $ENV{VIVADOHLS_INCLUDE_PATH})

#Network hw lib
set(DATASET $ENV{DATASET})
set(NETWORK $ENV{NETWORK})
set(NET_LIB ${LIB_NAME_PREFIX}-${DATASET}-${NETWORK})
if(ON_BOARD)
    add_library(${NET_LIB} INTERFACE)
    target_include_directories(${NET_LIB} INTERFACE "network/${DATASET}")
    target_include_directories(${NET_LIB} INTERFACE "network/${DATASET}/${NETWORK}")
    target_link_libraries(${NET_LIB} INTERFACE hls)
else()
    add_library(${NET_LIB} STATIC "network/${DATASET}/top.cpp")
    target_include_directories(${NET_LIB} PUBLIC "network/${DATASET}") 
    target_include_directories(${NET_LIB} PUBLIC "network/${DATASET}/${NETWORK}")
    target_link_libraries(${NET_LIB} PUBLIC hls)    
endif()

#Host lib
set(HOST_LIB ${LIB_NAME_PREFIX}-${DATASET}-${NETWORK}-ocr-pynq)
file(GLOB HOST_SOURCES "library/host/*.cpp")
add_library(${HOST_LIB} SHARED ${HOST_SOURCES})
target_include_directories(${HOST_LIB} PUBLIC "library/host")
target_link_libraries(${HOST_LIB} driver)
target_link_libraries(${HOST_LIB} ${NET_LIB})
target_link_libraries(${HOST_LIB} $ENV{LIBSDS_ABS_FILE_PATH})

# Install step
install(TARGETS ${HOST_LIB} DESTINATION ${DATASET}/${NETWORK})
