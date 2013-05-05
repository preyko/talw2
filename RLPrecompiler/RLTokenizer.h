#ifndef RLTOKENIZER_H
#define RLTOKENIZER_H

#include <stdio.h>
#include <iostream>

namespace RLTokenizer {
class Exception {
public:
    Exception(std::string descr) {
        description = descr;
    }

    std::string description;
};

extern FILE* token_output;

void Tokenize(FILE* input, FILE* output);
void Tokenize(const char* input, const char* output);
}

#endif
