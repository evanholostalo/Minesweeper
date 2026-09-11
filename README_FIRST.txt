OPEN THIS FOLDER (Minesweeper_fixed) IN CLION, not the src folder.

IMPORTANT: The ZIP you uploaded contained only the C++ source files. It did NOT contain the images/ or boards/ assets required by the game.

Before running, copy your original folders into this project so it looks like:

Minesweeper_fixed/
  CMakeLists.txt
  src/
    main.cpp
    Tile.cpp
    Tile.h
    TextureManager.cpp
    TextureManager.h
  images/
    ... PNG files ...
  boards/
    config.cfg
    testboard1.brd
    testboard2.brd
    testboard3.brd
    ...

Then reload CMake in CLion and run the Minesweeper target.
