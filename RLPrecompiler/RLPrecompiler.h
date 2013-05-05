#ifndef RLPRECOMPILER_H
#define RLPRECOMPILER_H

#include <iostream>
#include <fstream>

namespace RLPrecompiler {

class Exception {
public:
    Exception(std::string descr) {
        description = descr;
    }

    std::string description;
};

extern FILE* yyin;
extern std::ostream* logstream;

void Precompiler(const char* token_input);
}

#endif
