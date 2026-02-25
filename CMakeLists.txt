cmake_minimum_required(VERSION 3.16)

project(LogPulse VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        codeeditor.cpp
        codeeditor.h
)

set(PROJECT_RESOURCES
        resources.qrc
)

set(TS_FILES
    translations/LogPulse_zh_CN.ts
    translations/LogPulse_en_US.ts
)

set_source_files_properties(${TS_FILES} PROPERTIES
    OUTPUT_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/translations"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_create_translation(QM_FILES ${PROJECT_SOURCES} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${PROJECT_SOURCES} ${TS_FILES})
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LogPulse
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
        ${QM_FILES}
    )
else()
    if(ANDROID)
        add_library(LogPulse SHARED
            ${PROJECT_SOURCES}
            ${PROJECT_RESOURCES}
            ${QM_FILES}
        )
    else()
        add_executable(LogPulse
            ${PROJECT_SOURCES}
            ${PROJECT_RESOURCES}
            ${QM_FILES}
        )
    endif()
endif()

target_link_libraries(LogPulse PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LogPulse)
endif()
set_target_properties(LogPulse PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LogPulse
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LogPulse)
endif()
