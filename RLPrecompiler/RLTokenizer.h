#pragma once

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


extern std::ostream* tokenOutput_;

std::ostream& getTokenizerOutput();
void setTokenizerOutput(std::string filename);
void setTokenizerOutput(std::ostream& ostr);

void Tokenize(const char* input);

}
