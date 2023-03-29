cmake_minimum_required(VERSION 3.16)
project(checker VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_subdirectory(lib)

add_executable(checker main.cpp
        UI/UI/addgroup.cpp UI/UI/addgroup.h UI/UI/addgroup.ui
        UI/UI/WelcomeWindow.cpp UI/UI/WelcomeWindow.h UI/UI/WelcomeWindow.ui
        src/MainWindowInterface.h
        src/constDeclarations.h src/Settings.h
        UI/UI/AddNewMainGroup.cpp UI/UI/AddNewMainGroup.h UI/UI/AddNewMainGroup.ui
        src/MainGroupSettings.cpp src/MainGroupSettings.h
        src/TreeModel.cpp src/TreeModel.h
        src/MainWindowManager.cpp src/MainWindowManager.h
        UI/UI/AssigmentMW.cpp UI/UI/AssigmentMW.h UI/UI/AssigmentsMainWindow.ui
        src/GroupSettings.cpp src/GroupSettings.h UI/UI/addstudent.cpp UI/UI/addstudent.h UI/UI/addstudent.ui)

target_link_libraries(checker
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

target_include_directories(checker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include ${CMAKE_CURRENT_SOURCE_DIR}/lib/CSVLib/include)
target_include_directories(checker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/UI)
target_link_libraries(checker nlohmann_json::nlohmann_json CSVlib)

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/styles/qwindowsvistastyle${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/styles/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/styles/qwindowsvistastyle${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/styles/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
