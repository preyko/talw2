#include "RLInterpreter.h"

RLProcedure* RLInterpreter::mainProc_ = NULL;
RLInterpreter::StackFunctions RLInterpreter::stack_ = RLInterpreter::StackFunctions();

void RLInterpreter::Initialization() {
    if(mainProc_!=NULL)
        delete mainProc_;

    mainProc_ = new RLProcedure(-1);

    stack_.push_back(mainProc_);
}

void RLInterpreter::Perform() {
    mainProc_->exec();
}

RLProcedure* RLInterpreter::getMainFunction() {
    return mainProc_;
}

RLProcedure* RLInterpreter::getCurrentFunction() {
    return stack_[stack_.size()-1];
}

void RLInterpreter::addCommand(RLCommandPrototype* c) {
    getCurrentFunction()->addCommand(c);
}

void RLInterpreter::upStack(RLProcedure* u) {
    stack_.push_back(u);
}

RLProcedure* RLInterpreter::downStack() {
    RLProcedure* res = stack_[stack_.size()-1];

    stack_.pop_back();

    return res;
}
