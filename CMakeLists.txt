cmake_minimum_required(VERSION 3.7)

find_package(CURL REQUIRED)

file( GLOB HEADERS "include/*.h" )

add_library(update
            common.cpp
            httpdownloader.cpp
            httprequest.cpp
            httpresponse.cpp
            httpsession.cpp
            iniline.cpp
            iniparser.cpp
            logic.cpp
            srvsettings.cpp
            update.cpp
            updatepathes.cpp
            updatethread.cpp
            version.cpp
             ${HEADERS}
             )

target_include_directories(update
                           PUBLIC include
                           PRIVATE ${CURL_INCLUDE_DIRS}
                          )

if( WIN32 )
  target_sources(update PRIVATE
                 getversionex.cpp
                 stdafx.cpp
                 stdafx.h
                )
  target_link_libraries(update msi)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yustdafx.h /FIstdafx.h")
  set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif()
