#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "RLType.h"
#include "RLCommand.h"

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

    static void setApplicationOutput(std::string file_name);
    static std::ostream& getApplicationOutput();


    static bool isPerformingLinkProcedureLocked();
    static void lockLinkProcedurePerforming();
    static void unlockLinkProcedurePerforming();

private:
    static RLProcedure* mainProc_;

    static StackFunctions stack_;

    static std::ofstream* outputStream_;

    static bool performingLinkProcedureLocked_;

};
