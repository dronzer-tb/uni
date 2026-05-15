cmake_minimum_required(VERSION 3.21)
project(uni-ui VERSION 0.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets Concurrent)

set(UNI_UI_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/SearchBar.cpp
    src/SearchBar.h
    src/ResultsModel.cpp
    src/ResultsModel.h
    src/ResultsDelegate.cpp
    src/ResultsDelegate.h
    src/ProcessManager.cpp
    src/ProcessManager.h
    src/PackageDetailPane.cpp
    src/PackageDetailPane.h
    src/DownloadQueue.cpp
    src/DownloadQueue.h
    src/IndexWatcher.cpp
    src/IndexWatcher.h
    src/SourceBenchmark.cpp
    src/SourceBenchmark.h
    src/SettingsPane.cpp
    src/SettingsPane.h
    src/Theme.cpp
    src/Theme.h
)

qt_add_executable(uni-ui ${UNI_UI_SOURCES})
qt_add_resources(uni-ui "app_resources"
    PREFIX "/"
    FILES
        resources/styles/uni-ui.qss
)

target_link_libraries(uni-ui PRIVATE
    Qt6::Widgets
    Qt6::Concurrent
)

target_compile_definitions(uni-ui PRIVATE
    UNI_UI_VERSION="${PROJECT_VERSION}"
)

# Default-install layout: user-local, matches the existing bootstrap-installer.
include(GNUInstallDirs)
install(TARGETS uni-ui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/../packaging/uni.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications OPTIONAL)
