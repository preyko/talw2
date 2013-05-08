#pragma once

#include <iostream>
#include <fstream>

#include <vector>

namespace RLPrecompiler {

class Exception {
public:
    Exception(std::string descr,int line = -1) {
        description_ = descr;
        linePosition_ = line;
    }

    int whatLine() {
        return linePosition_;
    }

    std::string what() {
        return description_;
    }

private:
    int linePosition_;
    std::string description_;

};

class Exceptions {
public:
    typedef std::vector<Exception> Collectioner;

    void add(Exception exc) {
        collectionerException_.push_back(exc);
    }

    void clear() {
        collectionerException_.clear();
    }

    bool isEmpty() {
        return collectionerException_.size() == 0;
    }

    Collectioner& getCollectioner() {
        return collectionerException_;
    }

private:
    std::vector<Exception> collectionerException_;

};

extern FILE* yyin;
extern std::ofstream* logstream;

void Precompiler(const char* token_input);

std::ostream& getPrecompilerOutput();
void setPrecompilerOutput(std::string lo);

}
