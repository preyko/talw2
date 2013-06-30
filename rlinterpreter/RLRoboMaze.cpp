#include "RLRoboMaze.h"


// RLRoboMaze static members initialization
RLRoboMaze::RoboPosition RLRoboMaze::roboPos_ = RLRoboMaze::RoboPosition();
RLRoboMaze::RoboPosition RLRoboMaze::startRoboPos_ = RLRoboMaze::RoboPosition();
RLRoboMaze::Maze RLRoboMaze::mazeScheme_ = RLRoboMaze::Maze();
RLRoboMaze::Maze RLRoboMaze::mazeVisited_ = RLRoboMaze::Maze();
RLRoboMaze::MazeSize RLRoboMaze::mazeSize_ = RLRoboMaze::MazeSize();


bool RLRoboMaze::moveRobot(RLRoboMaze::Action action) {
    switch(action) {
        case mdown:
            return setRoboPosition(roboPos_.x,roboPos_.y+1);
        break;
        case mup:
            return setRoboPosition(roboPos_.x,--roboPos_.y-1);
        break;
        case mleft:
            return setRoboPosition(roboPos_.x-1,roboPos_.y);
        break;
        case mright:
            return setRoboPosition(roboPos_.x+1,roboPos_.y);
        break;
        case tp:
            for(int i = 0; i< 255; i++) {
                int x = rand() % mazeSize_.w;
                int y = rand() % mazeSize_.h;

                if(x > mazeSize_.w || y > mazeSize_.h || x < 0 || y < 0)
                    return false;

                if(!mazeScheme_.at(x).at(y)) {
                    return setRoboPosition(x,y);
                }
            }
        default:
            return false;
        break;
    }
}

RLRoboMaze::RoboPosition RLRoboMaze::getRoboPosition() {
    return roboPos_;
}

void RLRoboMaze::setSize(int w, int h) {
    mazeScheme_.resize(w);
    mazeVisited_.resize(w);
    for(int i = 0; i < mazeScheme_.size(); i++) {
        mazeScheme_[i].resize(h,false);
        mazeVisited_[i].resize(h,false);
    }
    mazeSize_.w = w;
    mazeSize_.h = h;
}

RLRoboMaze::MazeSize RLRoboMaze::getSize() {
    return mazeSize_;
}

void RLRoboMaze::loadMaze(std::istream& file) {
    int wi, hi;
    int xp, yp;
    int cellstate;

    file >> wi;
    file >> hi;

    file >> xp;
    file >> yp;

    setSize(wi,hi);
    if(!setRoboPosition(xp,yp))
        setRoboPosition(0,0);

    startRoboPos_.x = xp;
    startRoboPos_.y = yp;

    for(int y = 0; y < hi; y++)
        for(int x = 0; x < wi; x++) {
            file >> cellstate;

            mazeScheme_.at(x).at(y) = (bool)cellstate;
            mazeVisited_.at(x).at(y) = false;
        }
}

void RLRoboMaze::loadMaze(std::string filename) {
    std::ifstream file;
    file.open(filename.c_str());

    loadMaze(file);
}

void RLRoboMaze::clearMaze() {
    setSize(mazeSize_.w,mazeSize_.h);

    for(int x = 0; x < mazeScheme_.size(); x++)
        for(int y = 0; y < mazeScheme_.at(x).size(); y++)
            mazeScheme_.at(x).at(y) = false;
}

void RLRoboMaze::reset() {
    for(int x = 0; x < mazeVisited_.size(); x++)
        for(int y = 0; y < mazeVisited_.at(x).size(); y++)
            mazeVisited_.at(x).at(y) = false;
}

RLRoboMaze::CellState RLRoboMaze::getCellState(int x, int y) {
    if(x >= mazeSize_.w || y >= mazeSize_.h || x < 0 || y < 0)
        return OOR;

    if(mazeScheme_.at(x).at(y))
        return Occupied;
    else {
        if(mazeVisited_.at(x).at(y))
            return Visited;
        else
            return Empty;
    }

}

bool RLRoboMaze::setRoboPosition(int x, int y) {
    if(x >= mazeSize_.w || y >= mazeSize_.h || x < 0 || y < 0 || mazeScheme_.at(x).at(y) != false)
        return false;

    roboPos_.x = x;
    roboPos_.y = y;

    mazeVisited_.at(x).at(y) = true;

    return true;
}

void RLRoboMaze::showMaze() {
    for(int x = 0; x < mazeScheme_.size(); x++) {
        for(int y = 0; y < mazeScheme_.at(x).size(); y++) {
            if(roboPos_.x == x && roboPos_.y == y)
                std::cout << "A";
            else if(mazeScheme_.at(x).at(y) == 0)
                std::cout << "0";
            else if(mazeScheme_.at(x).at(y) == 1)
                std::cout << "#";
        }
        std::cout << std::endl;
    }
}
