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
                )
  target_link_libraries(update msi rpcrt4)
endif()
