# FocusMeter
Loosely based on pomodoro technique to help focus in small time intervals.

Project is currently in development on Ubuntu platform and uses gtkmm for GUI.


#Debug Build

Change directory to 'Debug'

1. cmake -DCMAKE_BUILD_TYPE=Debug ../
2. make
3. ./Source/FocusMeterApp

#Valgrind
valgrind --tool=memcheck --leak-check=full --leak-resolution=high --num-callers=20 --log-file=vgdump 