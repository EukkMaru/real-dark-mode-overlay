# REAL DARK MODE
 
A C++ program to enable "dark mode" even in applications that does not natively support dark mode.

---

## Prerequisites

- Winows 10 or higher
- Have GCC / G++ installed and have them in PATH

## Usage

- Run `make.bat`
- Press `Ctrl+Shift+X` or close the console window to exit the program
> If the build fails for some reason, you can manually build main.cpp with whatever c++ compiler you are comfortable with. In this case, make sure to link lgdi32, luser32, lgdiplus, lmsimg32 libraires.

## Known Issues

- This project is still WIP.
- The overlay does not extend over the taskbar.