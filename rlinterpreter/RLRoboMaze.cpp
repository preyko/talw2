#include "RLRoboMaze.h"

RLRoboMaze::RoboPosition RLRoboMaze::positionOfRobot_ = RLRoboMaze::RoboPosition();
RLRoboMaze::Maze RLRoboMaze::mazeScheme_ = RLRoboMaze::Maze();
RLRoboMaze::Maze RLRoboMaze::mazeVisited_ = RLRoboMaze::Maze();
RLRoboMaze::MazeSize RLRoboMaze::mazeSize_ = RLRoboMaze::MazeSize();


bool RLRoboMaze::moveRobot(RLRoboMaze::Action action) {
    switch(action) {
        case mdown:
        if(mazeScheme_.at(positionOfRobot_.x).at(positionOfRobot_.y+1) == 0)
            positionOfRobot_.y++;
            return true;
        break;
        case mup:
        if(mazeScheme_.at(positionOfRobot_.x).at(positionOfRobot_.y-1) == 0)
            positionOfRobot_.y--;
            return true;
        break;
        case mleft:
        if(mazeScheme_.at(positionOfRobot_.x-1).at(positionOfRobot_.y) == 0)
            positionOfRobot_.x--;
            return true;
        break;
        case mright:
        if(mazeScheme_.at(positionOfRobot_.x+1).at(positionOfRobot_.y) == 0)
            positionOfRobot_.x++;
            return true;
        break;
        case tp:
            for(int i = 0; i< 255; i++) {
                int x = rand() % mazeSize_.w;
                int y = rand() % mazeSize_.h;

                if(mazeScheme_.at(x).at(y) == 0) {
                    positionOfRobot_.x = x;
                    positionOfRobot_.y = y;
                    return true;
                }
            }
        default:
            return false;
        break;
    }
}

RLRoboMaze::RoboPosition RLRoboMaze::getRoboPosition() {
    return positionOfRobot_;
}

void RLRoboMaze::setMazeSize(int w, int h) {
    mazeScheme_.resize(w);
    mazeVisited_.resize(w);
    for(int i = 0; i < mazeScheme_.size(); i++) {
        mazeScheme_[i].resize(h);
        mazeVisited_[i].resize(h);
    }
}

bool RLRoboMaze::setRoboPos(int x, int y) {
    if(x > mazeSize_.w || y > mazeSize_.h || x < 0 || y < 0)
        return false;

    positionOfRobot_.x = x;
    positionOfRobot_.y = y;

    return true;
}

void RLRoboMaze::showMaze() {
    for(int x = 0; x < mazeScheme_.size(); x++) {
        for(int y = 0; y < mazeScheme_.at(x).size(); y++) {
            if(positionOfRobot_.x == x && positionOfRobot_.y == y)
                std::cout << "A";
            else if(mazeScheme_.at(x).at(y) == 0)
                std::cout << "0";
            else if(mazeScheme_.at(x).at(y) == 1)
                std::cout << "#";
        }
        std::cout << std::endl;
    }
}
