#pragma once

#include <iostream>
#include <fstream>

#include <vector>

//#include "RLCommand.h"

class RLRoboMaze {
public:
    enum CellState { Visited, Empty, Occupied, OOR };
    enum Action { mup, mdown, mleft, mright, tp, getrobopos, showmaze };

    typedef struct { int w; int h; } MazeSize;
    typedef struct { int x; int y; } RoboPosition;
    typedef std::vector<std::vector<bool> > Maze;

    static bool moveRobot(Action action);

    static RoboPosition getRoboPosition();
    static bool setRoboPosition(int x, int y);
    static CellState getCellState(int x, int y);

    static void setSize(int w, int h);
    static MazeSize getSize();

    static void loadMaze(std::istream& file);
    static void loadMaze(std::string filename);
    static void clearMaze();

    static void reset();

    static void showMaze();

private:
    static Maze mazeScheme_; // True if cell occupied, else - false
    static Maze mazeVisited_; // True if cell has been visited before, else - false

    static MazeSize mazeSize_;

    static RoboPosition roboPos_;
    static RoboPosition startRoboPos_;

};

#include "RLCommand.h"
