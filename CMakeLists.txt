project(notification-server C CXX)
cmake_minimum_required(VERSION 2.8.9)

set(SONAME 1)
set(VERSION 1.0)
set(SOVERSION 1.0.0)

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

option(private_dbus "Use private dbus namespace to prevent clashes during development." OFF)

include(cmake/coverage.cmake)
include(FindPkgConfig)
include (GNUInstallDirs)

add_definitions(-DQT_NO_KEYWORDS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Werror -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -pedantic -Werror -Wextra")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})

find_package(Qt5DBus REQUIRED)
include_directories(${Qt5DBus_INCLUDE_DIRS})

find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Workaround for https://bugreports.qt-project.org/browse/QTBUG-29987
set(QT_IMPORTS_DIR "${CMAKE_INSTALL_LIBDIR}/qt5/qml")

include_directories("include")
include_directories("${CMAKE_BINARY_DIR}/include")
include_directories("src")
include_directories("${CMAKE_BINARY_DIR}/src")

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tools)

# Disabled this until trunks of both Unity and notifications
# have been updated to work together.

##find_program(qmltestrunner_exe qmltestrunner)
##
##if(NOT qmltestrunner_exe)
##  message(FATAL_ERROR "Could not locate qmltestrunner.")
##endif()
##
##macro(add_qml_test COMPONENT_NAME)
##    set(options NO_ADD_TEST NO_TARGETS)
##
##    cmake_parse_arguments(qmltest "${options}" "IMPORT_PATH" "TARGETS" ${ARGN})
##
##    set(qmltest_TARGET test${COMPONENT_NAME})
##    set(qmltest_FILE tst_${COMPONENT_NAME})
##
##    if("${qmltest_IMPORT_PATH}" STREQUAL "")
##        add_custom_target(${qmltest_TARGET}
##            ${qmltestrunner_exe}  -input ${CMAKE_CURRENT_SOURCE_DIR}/${qmltest_FILE}.qml
##                          -o ${CMAKE_BINARY_DIR}/${qmltest_TARGET}.xml,xunitxml
##                          -o -,txt)
##    else()
##        add_custom_target(${qmltest_TARGET}
##            ${qmltestrunner_exe} -input ${CMAKE_CURRENT_SOURCE_DIR}/${qmltest_FILE}.qml
##                          -import ${qmltest_IMPORT_PATH}
##                          -o ${CMAKE_BINARY_DIR}/${qmltest_TARGET}.xml,xunitxml
##                          -o -,txt)
##    endif()
##
##    if(NOT "${qmltest_UNPARSED_ARGUMENTS}" STREQUAL "")
##        set_target_properties(${qmltest_TARGET} ${qmltest_PROPERTIES})
##    elseif(NOT "${qmltest_DEFAULT_PROPERTIES}" STREQUAL "")
##        set_target_properties(${qmltest_TARGET} ${qmltest_DEFAULT_PROPERTIES})
##    endif()
##
##    if("${qmltest_NO_ADD_TEST}" STREQUAL FALSE AND NOT "${qmltest_DEFAULT_NO_ADD_TEST}" STREQUAL "TRUE")
##        add_test(${qmltest_TARGET} make ${qmltest_TARGET})
##
##        if(NOT "${qmltest_UNPARSED_ARGUMENTS}" STREQUAL "")
##            set_tests_properties(${qmltest_TARGET} ${qmltest_PROPERTIES})
##        elseif(NOT "${qmltest_DEFAULT_PROPERTIES}" STREQUAL "")
##            set_tests_properties(${qmltest_TARGET} ${qmltest_DEFAULT_PROPERTIES})
##        endif()
##    endif("${qmltest_NO_ADD_TEST}" STREQUAL FALSE AND NOT "${qmltest_DEFAULT_NO_ADD_TEST}" STREQUAL "TRUE")
##
##    if("${qmltest_NO_TARGETS}" STREQUAL "FALSE")
##        if(NOT "${qmltest_TARGETS}" STREQUAL "")
##            foreach(TARGET ${qmltest_TARGETS})
##                add_dependencies(${TARGET} ${qmltest_TARGET})
##            endforeach(TARGET)
##        elseif(NOT "${qmltest_DEFAULT_TARGETS}" STREQUAL "")
##            foreach(TARGET ${qmltest_DEFAULT_TARGETS})
##                add_dependencies(${TARGET} ${qmltest_TARGET})
##            endforeach(TARGET)
##        endif()
##    endif("${qmltest_NO_TARGETS}" STREQUAL "FALSE")
##endmacro()

enable_testing()
add_subdirectory(test)
