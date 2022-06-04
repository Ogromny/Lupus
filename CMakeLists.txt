cmake_minimum_required(VERSION 3.22)
project(Lupus)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(SOURCES
    src/main.c
    src/accman_app.c
    src/gtk_helpers.c
    src/helpers.c
    src/tox_errors.c
    src/utils_passworddialog.c
)
set(HEADERS
    include/accman_app.h
    include/gtk_helpers.h
    include/helpers.h
    include/tox_errors.h
    include/utils_passworddialog.h
)
set(COMPILE_FLAGS "-Wall -Wextra -Werror -g")

set_source_files_properties(${SOURCES} ${HEADERS} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
add_executable(Lupus ${SOURCES})
target_include_directories(Lupus PUBLIC include)

# gtk4
find_package(PkgConfig)
pkg_check_modules(GTK4 REQUIRED gtk4)
add_definitions(${GTK4_CFLAGS_OTHER})
target_include_directories(Lupus PUBLIC ${GTK4_INCLUDE_DIRS})
target_link_directories(Lupus PUBLIC ${GTK4_LIBRARY_DIRS})
target_link_libraries(Lupus ${GTK4_LIBRARIES})

# toxcore
add_subdirectory(c-toxcore)
target_include_directories(Lupus PUBLIC c-toxcore)
target_link_libraries(Lupus toxcore_static)
