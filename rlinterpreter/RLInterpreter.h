#pragma once

#include <iostream>
#include <vector>

#include "RLType.h"

class RLInterpreter {
public:
    typedef std::vector<RLProcedure*> StackFunctions;

    static void Initialization();
    static void Perform();

    static RLProcedure* getMainFunction();
    static RLProcedure* getCurrentFunction();

    static void addCommand(RLCommandPrototype* c);

    static void upStack(RLProcedure* u = NULL);
    static RLProcedure* downStack();

private:
    static RLProcedure* mainProc_;

    static StackFunctions stack_;

};
