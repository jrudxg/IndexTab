cmake_minimum_required(VERSION 3.16)


project(IndexTab VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick WebEngineQuick WebEngineCore)

qt_standard_project_setup(REQUIRES 6.7.3)

set(CMAKE_AUTORCC ON)

add_subdirectory(quickjs)

qt_add_executable(IndexTab
    main.cpp
)

set_source_files_properties(LastProjectModel.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(IndexTab
    URI IndexTab
    VERSION 1.0
    QML_FILES
        Main.qml
	FileFolder.qml
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
    SOURCES
    SOURCES filemanager.h filemanager.cpp
    SOURCES filemodel.h filemodel.cpp
    SOURCES flashcardUtility.h flashcardUtility.cpp
    SOURCES projectreader.h projectreader.cpp
    SOURCES scenedatamodel.h scenedatamodel.cpp
    SOURCES scenelinkutility.h scenelinkutility.cpp
    SOURCES generaltaskutility.h generaltaskutility.cpp
    SOURCES tablemodel.h tablemodel.cpp
    SOURCES tableutility.h tableutility.cpp
    SOURCES
    SOURCES textutility.h textutility.cpp
    SOURCES


    RESOURCES
        resource.qrc
	QML_FILES TaskText.qml
	QML_FILES
)

set_target_properties(IndexTab PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(IndexTab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/quickjs)
target_link_libraries(IndexTab PRIVATE Qt6::Quick Qt6::WebEngineQuick Qt6::WebEngineCore ${CMAKE_CURRENT_SOURCE_DIR}/quickjs/build/Debug/quickjs.lib)

include(GNUInstallDirs)
install(TARGETS IndexTab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
