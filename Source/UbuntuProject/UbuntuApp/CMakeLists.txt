# https://wiki.gnome.org/Projects/gtkmm/UsingCMake

link_directories(
    ${GTKMM_LIBRARY_DIRS})

include_directories(
    ${GTKMM_INCLUDE_DIRS})

add_executable(FocusMeterApp
	Main.cpp
	../../CrossPlatform/Database/SQLDatabase.hpp
	../../CrossPlatform/Database/SQLDatabase.cpp
	../../CrossPlatform/Controller/FMController.hpp
	../../CrossPlatform/Controller/FMController.cpp
	GUI/MainWindow.cpp
	GUI/MainWindow.hpp)

target_link_libraries(FocusMeterApp ${GTKMM_LIBRARIES} sqlite3)
