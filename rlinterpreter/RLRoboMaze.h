#pragma once

#include <vector>

//#include "RLCommand.h"

class RLRoboMaze {
public:
    enum Action { mup, mdown, mleft, mright, tp };

    typedef struct { int w; int h; } MazeSize;
    typedef struct { int x; int y; } RoboPosition;
    typedef std::vector<std::vector<int> > Maze;

    static bool moveRobot(Action action);

    static RoboPosition getRoboPosition();

    static void setMazeSize(int w, int h);
    static bool setRoboPos(int x, int y);

    static void showMaze();

private:
    static Maze mazeScheme_;
    static Maze mazeVisited_;
    static MazeSize mazeSize_;

    static RoboPosition positionOfRobot_;

};

#include "RLCommand.h"
