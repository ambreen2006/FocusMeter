set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(FocusMeterApp
	main.cpp
	../../CrossPlatform/Database/SQLDatabase.hpp
	../../CrossPlatform/Database/SQLDatabase.cpp
	../../CrossPlatform/Controller/FMController.hpp
	../../CrossPlatform/Controller/FMController.cpp
	)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_link_libraries(FocusMeterApp sqlite3 stdc++fs)
