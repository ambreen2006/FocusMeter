# https://wiki.gnome.org/Projects/gtkmm/UsingCMake

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(FocusMeterApp
    Main.cpp
    MainWindow.cpp
    MainWindow.hpp)

target_link_libraries(FocusMeterApp
    ${GTKMM_LIBRARIES}  )
