#pragma once

#include <iostream>
#include <fstream>

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

extern FILE* yyin;
extern std::ofstream* logstream;

void Precompiler(const char* token_input);

std::ostream& getPrecompilerOutput();
void setPrecompilerOutput(std::string lo);

}
