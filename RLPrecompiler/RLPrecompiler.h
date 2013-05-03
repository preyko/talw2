#pragma once

namespace RLPrecompiler {
    int Precompiler(char** token_input) {
        yyin = fopen(token_input,"r");

        do {
            yyparse();
        } while(!feof(yyin));

        return 0;
    }
}
