cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(BackupUtility
    VERSION 1.0
    DESCRIPTION "Simple utility for file backups"
    LANGUAGES CXX
)
set(TARGET_NAME "BackupUtility")
file(GLOB SOURCES
    "src/*.cpp"
)
add_executable(${TARGET_NAME} ${SOURCES})
find_library(SQLITE3_LIBRARY sqlite3)
message("SQLite3 Library Path: ${SQLITE3_LIBRARY}")
target_link_libraries(${TARGET_NAME} PUBLIC ${SQLITE3_LIBRARY})
