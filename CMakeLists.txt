cmake_minimum_required(VERSION 3.7)

# TODO: ADD CURL LIBRARY 

file( GLOB HEADERS "*.h" "include/*.h" )

add_library( update
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
             
target_include_directories(    
                            update  PUBLIC   
                            include               
                            "${CMAKE_CURRENT_COURCE_DIR}"
                          )
if( WIN32 )
  target_sources(
              update  PUBLIC
              getversionex.cpp
               stdafx.cpp
               )
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yustdafx.h /FIstdafx.h")
  set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
  #target_link_libraries( )
else()
#  target_link_libraries( )
endif()
