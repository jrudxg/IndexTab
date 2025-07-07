cmake_minimum_required(VERSION 3.16)

project(IndexTab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC on)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appIndexTab
    main.cpp
)

set_source_files_properties(LastProjectModel.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)


qt_add_qml_module(appIndexTab
    URI IndexTab
    VERSION 1.0
    QML_FILES
        RESOURCES resource.qrc
        Main.qml
	QML_FILES FileFolder.qml
	QML_FILES Menubar.qml
	QML_FILES HomeScreen.qml
	QML_FILES OpenFileDialog.qml
	QML_FILES CreateFileDialog.qml
	QML_FILES HomeToolbar.qml
	QML_FILES ToolbarLinks.qml
	QML_FILES ProjectButtons.qml
	QML_FILES ClickableTextImage.qml
	QML_FILES LastProjectModel.qml
	SOURCES filemanager.h filemanager.cpp
	SOURCES filemodel.h filemodel.cpp
	SOURCES flashcardUtility.h flashcardUtility.cpp
	SOURCES projectreader.h projectreader.cpp
	SOURCES scenedatamodel.h scenedatamodel.cpp
	QML_FILES Flashcard.qml
	QML_FILES Scene.qml
	QML_FILES SceneLink.qml
	SOURCES scenelinkutility.h scenelinkutility.cpp
	SOURCES generaltaskutility.h generaltaskutility.cpp
	QML_FILES Table.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appIndexTab PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appIndexTab
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appIndexTab
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appIndexTab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
