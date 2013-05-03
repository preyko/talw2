#pragma once

#include <stdio.h>

namespace RLTokenizer {
    FILE* token_output;

    void Tokenize(FILE* input, FILE* output);
}
