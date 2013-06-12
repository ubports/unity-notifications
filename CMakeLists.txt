project(notification-server C CXX)
cmake_minimum_required(VERSION 2.8.9)

set(SONAME 1)
set(VERSION 1.0)
set(SOVERSION 1.0.0)

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

option(basic_warnings "Basic compiler warnings." ON)
option(build_tools "Build developer tools." OFF)
option(private_dbus "Use private dbus namespace to prevent clashes during development." OFF)

include(FindPkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
add_definitions(-DQT_NO_KEYWORDS)

if(${basic_warnings})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

set(LIBDIR "lib" CACHE PATH "Destination install dir for the library")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
add_subdirectory(src)
if(${build_tools})
  add_subdirectory(tools)
endif()

find_program(qmltestrunner_exe qmltestrunner)

if(NOT qmltestrunner_exe)
  message(FATAL_ERROR "Could not locate qmltestrunner.")
endif()

macro(add_qml_test COMPONENT_NAME)
    set(options NO_ADD_TEST NO_TARGETS)

    cmake_parse_arguments(qmltest "${options}" "IMPORT_PATH" "TARGETS" ${ARGN})

    set(qmltest_TARGET test${COMPONENT_NAME})
    set(qmltest_FILE tst_${COMPONENT_NAME})

    if("${qmltest_IMPORT_PATH}" STREQUAL "")
        add_custom_target(${qmltest_TARGET}
            ${qmltestrunner_exe}  -input ${CMAKE_CURRENT_SOURCE_DIR}/${qmltest_FILE}.qml
                          -o ${CMAKE_BINARY_DIR}/${qmltest_TARGET}.xml,xunitxml
                          -o -,txt)
    else()
        add_custom_target(${qmltest_TARGET}
            ${qmltestrunner_exe} -input ${CMAKE_CURRENT_SOURCE_DIR}/${qmltest_FILE}.qml
                          -import ${qmltest_IMPORT_PATH}
                          -o ${CMAKE_BINARY_DIR}/${qmltest_TARGET}.xml,xunitxml
                          -o -,txt)
    endif()

    if(NOT "${qmltest_UNPARSED_ARGUMENTS}" STREQUAL "")
        set_target_properties(${qmltest_TARGET} ${qmltest_PROPERTIES})
    elseif(NOT "${qmltest_DEFAULT_PROPERTIES}" STREQUAL "")
        set_target_properties(${qmltest_TARGET} ${qmltest_DEFAULT_PROPERTIES})
    endif()

    if("${qmltest_NO_ADD_TEST}" STREQUAL FALSE AND NOT "${qmltest_DEFAULT_NO_ADD_TEST}" STREQUAL "TRUE")
        add_test(${qmltest_TARGET} make ${qmltest_TARGET})

        if(NOT "${qmltest_UNPARSED_ARGUMENTS}" STREQUAL "")
            set_tests_properties(${qmltest_TARGET} ${qmltest_PROPERTIES})
        elseif(NOT "${qmltest_DEFAULT_PROPERTIES}" STREQUAL "")
            set_tests_properties(${qmltest_TARGET} ${qmltest_DEFAULT_PROPERTIES})
        endif()
    endif("${qmltest_NO_ADD_TEST}" STREQUAL FALSE AND NOT "${qmltest_DEFAULT_NO_ADD_TEST}" STREQUAL "TRUE")

    if("${qmltest_NO_TARGETS}" STREQUAL "FALSE")
        if(NOT "${qmltest_TARGETS}" STREQUAL "")
            foreach(TARGET ${qmltest_TARGETS})
                add_dependencies(${TARGET} ${qmltest_TARGET})
            endforeach(TARGET)
        elseif(NOT "${qmltest_DEFAULT_TARGETS}" STREQUAL "")
            foreach(TARGET ${qmltest_DEFAULT_TARGETS})
                add_dependencies(${TARGET} ${qmltest_TARGET})
            endforeach(TARGET)
        endif()
    endif("${qmltest_NO_TARGETS}" STREQUAL "FALSE")
endmacro(add_qml_test)

enable_testing()
add_subdirectory(test)
