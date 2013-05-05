#ifndef RLPRECOMPILER_H
#define RLPRECOMPILER_H

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
extern std::ostream* logstream;

void Precompiler(const char* token_input);
void setPrecompilerOutput(const char* lo);

}
#endif
