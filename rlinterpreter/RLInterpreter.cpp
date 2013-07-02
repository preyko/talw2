#include "RLInterpreter.h"

bool RLInterpreter::outputStreamOpenedHere_ = false;
RLChainCommands* RLInterpreter::mainProc_ = NULL;
std::ostream* RLInterpreter::outputStream_ = NULL;
RLInterpreter::StackFunctions RLInterpreter::stack_ = RLInterpreter::StackFunctions();

void RLInterpreter::Initialization() {
    RLTypePrototype::killThemAll();

    mainProc_ = new RLChainCommands();

    stack_.push_back(mainProc_);

    RLRoboMaze::reset();
}

void RLInterpreter::Perform() {
    unlockLinkProcedurePerforming();

    if(mainProc_==NULL)
        throw RLPerformException(std::string("You should call RLInterpreter::Initialization() before."),0);
    else
        mainProc_->exec();
    
    RLTypePrototype::clearTempVars();
    RLTypePrototype::clearConstVars();
    RLTypePrototype::clearDynamicallyAllocatedVars();
    
    if(outputStreamOpenedHere_) {
        std::ofstream* outfstream = dynamic_cast<std::ofstream*>(outputStream_);
        if(outfstream != NULL) {
            outfstream->close();
            delete outfstream;
            outfstream = NULL;
        }
    }
}

RLChainCommands* RLInterpreter::getMainFunction() {
    return mainProc_;
}

RLChainCommands* RLInterpreter::getCurrentFunction() {
    return stack_[stack_.size()-1];
}

void RLInterpreter::addCommand(RLCommandPrototype* c) {
    if(c!=NULL)
        getCurrentFunction()->addCommand(c);
}

void RLInterpreter::upStack(RLChainCommands* u) {
    if(u==NULL)
        u = new RLChainCommands();

    stack_.push_back(u);
}

RLChainCommands* RLInterpreter::downStack() {
    RLChainCommands* res = stack_[stack_.size()-1];

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
