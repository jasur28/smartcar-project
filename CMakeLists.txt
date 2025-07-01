cmake_minimum_required(VERSION 3.16)

project(SmartCar VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core
    Gui
    Qml
    Quick
    QuickControls2
    Bluetooth)

# Автоматически включить MOC для QObject
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appSmartCar
    main.cpp
)

qt_add_qml_module(appSmartCar
    URI SmartCar
    VERSION 1.0
    QML_FILES
        Main.qml
        RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml
        SOURCES BluetoothManager.h
        SOURCES BluetoothManager.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSmartCar PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSmartCar
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSmartCar
    PRIVATE Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Bluetooth)

include(GNUInstallDirs)
install(TARGETS appSmartCar
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
