cmake_minimum_required(VERSION 3.19)
project(personal-plan LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Network LinguistTools)

qt_standard_project_setup()

qt_add_executable(personal-plan
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    task.h
    database.h
    database.cpp
    openclawclient.h
    openclawclient.cpp

)

qt_add_translations(
    TARGETS personal-plan
    TS_FILES personal-plan_zh_CN.ts
)

target_link_libraries(personal-plan
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::Network
)

install(TARGETS personal-plan
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET personal-plan
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
