#include "RLInterpreter.h"

bool RLInterpreter::outputStreamOpenedHere_ = false;
RLProcedure* RLInterpreter::mainProc_ = NULL;
std::ostream* RLInterpreter::outputStream_ = NULL;
RLInterpreter::StackFunctions RLInterpreter::stack_ = RLInterpreter::StackFunctions();

void RLInterpreter::Initialization() {
    RLTypePrototype::killThemAll();

    mainProc_ = (RLProcedure*)((new RLProcedure(-1))->markAsConst());

    stack_.push_back(mainProc_);
}

void RLInterpreter::Perform() {
    unlockLinkProcedurePerforming();

    if(mainProc_==NULL)
        throw RLPerformException(std::string("You should call RLInterpreter::Initialization() before."),0);
    else
        mainProc_->exec();
    
    if(outputStreamOpenedHere_) {
        std::ofstream* outfstream = dynamic_cast<std::ofstream*>(outputStream_);
        if(outfstream != NULL) {
            outfstream->close();
            delete outfstream;
            outfstream = NULL;
        }
    }
}

RLProcedure* RLInterpreter::getMainFunction() {
    return mainProc_;
}

RLProcedure* RLInterpreter::getCurrentFunction() {
    return stack_[stack_.size()-1];
}

void RLInterpreter::addCommand(RLCommandPrototype* c) {
    if(c!=NULL)
        getCurrentFunction()->addCommand(c);
}

void RLInterpreter::upStack(RLProcedure* u) {
    if(u==NULL)
        u = new RLProcedure();

    stack_.push_back(u);
}

RLProcedure* RLInterpreter::downStack() {
    RLProcedure* res = stack_[stack_.size()-1];

    stack_.pop_back();

    return res;
}

void RLInterpreter::setApplicationOutput(std::string fname) {
    outputStreamOpenedHere_ = true;
    outputStream_ = new std::ofstream(fname.c_str());
}

void RLInterpreter::setApplicationOutput(std::ostream& stream) {
    outputStreamOpenedHere_ = false;
    outputStream_ = &stream;
}

std::ostream& RLInterpreter::getApplicationOutput() {
    if(outputStream_!=NULL)
        return *outputStream_;
    else
        return std::cout;
}

bool RLInterpreter::performingLinkProcedureLocked_ = false;
bool RLInterpreter::isPerformingLinkProcedureLocked() {
    return performingLinkProcedureLocked_;
}
void RLInterpreter::lockLinkProcedurePerforming() {
    performingLinkProcedureLocked_ = true;
}
void RLInterpreter::unlockLinkProcedurePerforming() {
    performingLinkProcedureLocked_ = false;
}
