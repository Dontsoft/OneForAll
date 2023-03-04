cmake_minimum_required(VERSION 3.5)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  find_package(Git)
  if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif()
endif()

set(BASE_VERSION 1.0.0)
set(BASE_VERSION_META -alpha)
set(APP_NAME "Devtools by dontsoft")
set(APP_AUTHOR "Artem Dontsov")
set(APP_AUTHOR_MAIL "artem@dontsoft.com")
set(APP_AUTHOR_WEBPAGE "https://dontsoft.com")
set(APP_AUTHOR_GITHUB "https://github.com/dontsoft")
set(APP_COMMENT "")
set(APP_DESCRIPTION "")
set(APP_YEAR "2023")
set(APP_PORTABLE false)

if(DEFINED GIT_HASH)
    message("-- Git hash = ${GIT_HASH}")
    list(APPEND DEFINITIONS -DGITHASH="${GIT_HASH}")
    set(APP_VERSION ${BASE_VERSION}${BASE_VERSION_META}+sha.${GIT_HASH})
else()
    set(GIT_HASH "Unknown")
    message("-- Git hash = N/A")
    set(APP_VERSION ${BASE_VERSION}${BASE_VERSION_META}+sha.unknown)
endif()

if(BASE_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
  set(VERSION_MAJOR "${CMAKE_MATCH_1}")
  set(VERSION_MINOR "${CMAKE_MATCH_2}")
  set(VERSION_PATCH "${CMAKE_MATCH_3}")
  set(WINDOWS_VERSION ${VERSION_MAJOR},${VERSION_MINOR},${VERSION_PATCH})
else()
  message(FATAL_ERROR "Failed to parse BASE_VERSION='${BASE_VERSION}'")
endif()

project(devtools VERSION 0.1 LANGUAGES CXX)

configure_file(build.h.in build.hpp @ONLY)
configure_file(version.rc.in version.rc @ONLY)
configure_file(${PROJECT_NAME}.rc.in ${PROJECT_NAME}.rc @ONLY)

message("-- Version = ${PROJECT_VERSION}")


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Gui LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Gui LinguistTools)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

add_compile_definitions(UUID_SYSTEM_GENERATOR UUID_TIME_GENERATOR)

set(TS_FILES devtools_de_DE.ts)

set(PROJECT_SOURCES
        main.cpp

        resources.qrc

        devtoolsmainwindow.cpp
        devtoolsmainwindow.h
        devtoolsmainwindow.ui

        # Models
        models/modulenavigationmodel.hpp
        models/modulenavigationmodel.cpp
        models/modulenavigationfiltermodel.hpp
        models/modulenavigationfiltermodel.cpp

        # Modules
        modules/moduleregistry.hpp
        modules/moduleregistry.cpp
        modules/module.hpp
        modules/module.cpp
        modules/categories.hpp
        modules/dummymodule.hpp
        modules/dummymodule.cpp

        ## Generators
        modules/generator/uuidgeneratormodule.hpp
        modules/generator/uuidgeneratormodule.cpp
        modules/generator/uuidgeneratormodulewidget.hpp
        modules/generator/uuidgeneratormodulewidget.cpp
        modules/generator/uuidgeneratormodulewidget.ui
        modules/generator/passwordgeneratormodule.hpp
        modules/generator/passwordgeneratormodule.cpp
        modules/generator/passwordgeneratormodulewidget.hpp
        modules/generator/passwordgeneratormodulewidget.cpp
        modules/generator/passwordgeneratormodulewidget.ui

        ## Decorders
        modules/decoder/uuiddecodermodule.hpp
        modules/decoder/uuiddecodermodule.cpp
        modules/decoder/uuiddecodermodulewidget.hpp
        modules/decoder/uuiddecodermodulewidget.cpp
        modules/decoder/uuiddecodermodulewidget.ui

        # Core
        core/dependant.hpp
        core/observer.hpp
        core/observable.hpp

        # Log
        log/loggable.hpp
        log/loggable.cpp
        log/qtfmt.hpp

        # Config
        config/configengine.cpp
        config/configengine.hpp
        config/configlistener.cpp
        config/configlistener.hpp
        config/engine/configfileengine.cpp
        config/engine/configfileengine.hpp
        config/engine/configjsonfileengine.cpp
        config/engine/configjsonfileengine.hpp
        config/engine/confignativefileengine.cpp
        config/engine/confignativefileengine.hpp
        config/engine/configyamlfileengine.cpp
        config/engine/configyamlfileengine.hpp
        config/engine/multiconfigengine.cpp
        config/engine/multiconfigengine.hpp

        # Utility
        utility/appinformation.cpp
        utility/appinformation.hpp
        utility/pathregistry.cpp
        utility/pathregistry.hpp

        # random
        random/random.hpp
        random/random.cpp

        ${TS_FILES}

        # Third Party
        external/stduuid/uuid.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(devtools
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(devtools
        ${PROJECT_SOURCES}
    )
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

set(YAML_BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "")
set(YAML_CPP_INSTALL OFF CACHE INTERNAL "")
add_subdirectory(external/yaml-cpp)

target_link_libraries(devtools PRIVATE Qt6::Widgets Qt6::Network Qt6::Gui)
target_link_libraries(devtools PRIVATE spdlog::spdlog)
target_link_libraries(devtools PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(devtools PUBLIC yaml-cpp)

set_target_properties(devtools PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER devtools.dontsoft.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_BUNDLE_NAME ${APP_NAME}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS devtools
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(devtools)
endif()
