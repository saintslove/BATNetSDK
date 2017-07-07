cmake_minimum_required(VERSION 3.6)
project(BATNetSDK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../muduo)

set(SOURCE_FILES
    Package/CommonPackage.cpp
    Package/CommonPackage.h
    Package/Encoder.cpp
    Package/Encoder.h
    Package/Header.cpp
    Package/Header.h
    common.h
    CommonServer.cpp
    CommonServer.h
    newSdk.cpp
    rawSdk.cpp
    sdk.cpp
    sdk.h
    SDKManager.cpp
    SDKManager.h
    ServerBase.cpp
    ServerBase.h)

add_executable(BATNetSDK ${SOURCE_FILES})