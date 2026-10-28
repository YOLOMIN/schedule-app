cmake_minimum_required(VERSION 3.19)
project(ScheduleApp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Network LinguistTools)

qt_standard_project_setup()

qt_add_executable(ScheduleApp
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
    TARGETS ScheduleApp
    TS_FILES ScheduleApp_zh_CN.ts
)

target_link_libraries(ScheduleApp
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::Network
)

install(TARGETS ScheduleApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ScheduleApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# 编译后自动把 config 目录复制到 exe 旁边
add_custom_command(TARGET ScheduleApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:ScheduleApp>/config
)
