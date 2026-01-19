cmake_minimum_required(VERSION 3.16)

project(GraphSolver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Dependencies
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
find_package(spdlog REQUIRED)

# --- Sources Definition ---
# Logic sources shared or specific to logic
set(LOGIC_SOURCES
    algorithmeditor.cpp algorithmeditor.h algorithmeditor.ui
    coloringsolver.cpp coloringsolver.h
    connectivitysolver.cpp connectivitysolver.h
    edge.cpp edge.h
    edgeeditor.cpp edgeeditor.h edgeeditor.ui
    euleriansolver.cpp euleriansolver.h
    graph.cpp graph.h
    isomorphismsolver.cpp isomorphismsolver.h
    matchingsolver.cpp matchingsolver.h
    maxflowsolver.cpp maxflowsolver.h
    mincostflowsolver.cpp mincostflowsolver.h
    minspanningforestsolver.cpp minspanningforestsolver.h
    mygraphicsscene.cpp mygraphicsscene.h
    myview.cpp myview.h myview.ui
    shortestpathsolver.cpp shortestpathsolver.h
    vertex.cpp vertex.h
    vertexeditor.cpp vertexeditor.h vertexeditor.ui
    LoggerWrapper.cpp LoggerWrapper.h
    TemplateStack.h
    commands.cpp commands.h
    layoutsolver.cpp layoutsolver.h
)

# Removed: console_ui.cpp/.h, AbstractAlgorithm.h, algorithms_oop.h, AppConfig.h/.cpp

set(APP_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)

# --- Main Application Target ---
add_executable(GraphSolver
    ${APP_SOURCES}
    ${LOGIC_SOURCES}
)

target_link_libraries(GraphSolver PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    spdlog::spdlog
)

# --- Installation Rules ---
include(GNUInstallDirs)
install(TARGETS GraphSolver
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
)

# --- Testing Setup ---
enable_testing()

# Fetch GoogleTest automatically
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define Test Executable
add_executable(GraphSolverTests
    tests/main_test.cpp
    tests/tst_solvers.cpp
    tests/tst_commands.cpp
    ${LOGIC_SOURCES} # Link logic sources directly to test
)

target_include_directories(GraphSolverTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GraphSolverTests PRIVATE
    GTest::gtest_main
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    spdlog::spdlog
)

# Register tests
include(GoogleTest)
# Pass -platform offscreen to ensure tests don't fail if no display is available
gtest_discover_tests(GraphSolverTests EXTRA_ARGS "-platform" "offscreen")

# --- Documentation (Doxygen) ---
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Configure Doxygen input and recursion
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_FILE_PATTERNS *.cpp *.h *.hpp *.ui *.md)
    
    # Critical: Use README.md as the main page
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    # Add a target to generate API documentation with Doxygen
    # This automatically sets the INPUT to PROJECT_SOURCE_DIR
    doxygen_add_docs(doxygen_docs
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate HTML documentation"
    )
endif()