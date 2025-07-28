cmake_minimum_required(VERSION 3.16)


project(IndexTab VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick WebEngineQuick WebEngineCore)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.7.3)

set(CMAKE_AUTORCC ON)

qt_add_executable(IndexTabApp
    main.cpp
)

add_subdirectory(quickjs)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(FILE_URL "file:///")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(FILE_URL "file://")
endif()
target_compile_definitions(IndexTabApp PRIVATE FILE_URL="${FILE_URL}")

add_subdirectory(src)
add_subdirectory(include)

set_source_files_properties(LastProjectModel.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(IndexTabApp
    URI IndexTab
    VERSION 1.0
    QML_FILES
        Main.qml
        Menubar.qml
        HomeScreen.qml
        OpenFolderDialog.qml
        CreateFileDialog.qml
        HomeToolbar.qml
        ToolbarLinks.qml
        ProjectButtons.qml
        ClickableTextImage.qml
        LastProjectModel.qml
        TaskFlashcard.qml
        Scene.qml
        TaskSceneLink.qml
        TaskTable.qml
        TaskText.qml



    RESOURCES
        resource.qrc
)


set_target_properties(IndexTabApp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(IndexTabApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/quickjs)
target_link_libraries(IndexTabApp PRIVATE Qt6::Quick Qt6::WebEngineQuick Qt6::WebEngineCore quickjs)
target_link_libraries(IndexTabApp PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS IndexTabApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
