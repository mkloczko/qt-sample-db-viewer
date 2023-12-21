cmake_minimum_required(VERSION 3.16)

project(qt-sample-db-viewer VERSION 0.1 LANGUAGES C CXX)
option(GEN_COVERAGE "Generate coverage with help of gcovr" false)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing(true)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Location Network Positioning Concurrent Test Sql)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
find_package(GTest REQUIRED)

qt_standard_project_setup()

qt_add_library(sample-lib)
qt_add_qml_module(sample-lib
    URI test-project
    VERSION 1.0
    QML_FILES qml/Main.qml
    QML_FILES qml/UI/Table.qml
    QML_FILES qml/UI/Menu.qml
    QML_FILES qml/UI/Menu/ConnectionMenu.qml
    QML_FILES qml/UI/Menu/RecordMenu.qml
    QML_FILES qml/UI/Menu/IpStackMenu.qml
    SOURCES include/database.h src/database.cpp
    SOURCES include/extendedtablemodel.h src/extendedtablemodel.cpp
    SOURCES include/utils.h src/utils.cpp
    SOURCES include/ipstack.h src/ipstack.cpp
)

target_include_directories(sample-lib PUBLIC "include/")
target_link_libraries(sample-lib
    PUBLIC Qt6::Quick Qt6::Sql Qt6::Concurrent Qt6::Network Qt6::Positioning
)

qt_add_executable(sample-viewer
    src/main.cpp
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(sample-viewer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest-project
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(sample-viewer PRIVATE sample-lib)

qt_add_executable(sample-test tests/tst_utils.cpp
    tests/tst_ipstack.cpp
)

set_target_properties(sample-test PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest-project
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(sample-test PRIVATE sample-lib GTest::gtest_main GTest::gmock_main)


if (DEFINED GEN_COVERAGE) 
    include(CodeCoverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()
    # Got problems running gcovr
    setup_target_for_coverage_gcovr_html(NAME sample-coverage EXECUTABLE sample-test)
endif ()

include(GNUInstallDirs)
install(TARGETS sample-viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS sample-lib
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
