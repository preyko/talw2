#include "RLCommand.h"

#include "RLType.h"
#include "RLTools.h"
#include "RLInterpreter.h"

/*
 * class RLCommandPrototype
 */
RLTypePrototype *RLCommandPrototype::getFinalLeft() {
    return NULL;
}

void RLCommandPrototype::setLinePosition(int line) {
    linePosition_ = line;
}

int RLCommandPrototype::getLinePosition() {
    return linePosition_;
}

/*
 * class RLDereference : public RLCommandBase
 */
RLDereference::RLDereference(int ref) {
    RLTypePrototype* res = RLIdentRegister::get(ref);

    if(res == NULL) {
        // Identifier ref does not declared before.
        RLPerformException(std::string("Identifier ") +
                                 intToStr(ref) +
                                 std::string(" does not declared before.\n"),getLinePosition());
        return;
    } else {
        value_ = res;
    }
}

RLDereference::RLDereference(RLTypePrototype* ref) {
    value_ = ref;
}

RLDereference::~RLDereference() {
}

RLCommandPrototype* RLDereference::copy() const {
    return new RLDereference(value_);
}

RLTypePrototype* RLDereference::exec() const {
    value_->performLinkedProcedures();
    return value_;
}

RLTypePrototype* RLDereference::getFinalLeft() {
    return value_;
}


/*
 * class RLCommand : public RLCommandBase
 */
RLCommand::RLCommand(RLOperator oper, RLCommandPrototype* f, RLCommandPrototype* s) {
    init_(oper,f,s);
}

RLCommand::RLCommand(RLOperator oper, RLTypePrototype* f, RLTypePrototype* s) {
    if(s == NULL)
        init_(oper,new RLDereference(f),NULL);
    else
        init_(oper,new RLDereference(f),new RLDereference(s));
}

RLCommand::RLCommand(RLOperator oper, int idf, int ids) {
    init_(oper,idf,ids);
}

RLCommand::~RLCommand() {
    delete first_;

    if(second_!=NULL)
        delete second_;
}

void RLCommand::init_(RLOperator oper, int f, int s) {
    if(s == -1)
        init_(oper,new RLDereference(f),NULL);
    else
        init_(oper,new RLDereference(f),new RLDereference(s));
}

void RLCommand::init_(RLOperator oper, RLCommandPrototype* f, RLCommandPrototype* s) {
    first_ = f;
    second_ = s;

    operator_ = oper;
}

RLCommandPrototype* RLCommand::copy() const {
    return new RLCommand(operator_,first_,second_);
}

RLTypePrototype* RLCommand::exec() const {
    return exec_();
}

RLTypePrototype* RLCommand::getFinalLeft() {
    return first_->getFinalLeft();
}

RLTypePrototype* RLCommand::exec_() const {
    if(first_==NULL && second_==NULL) {
        return (new RLBool(false))->markAsTemp();
    } else if(first_!=NULL && second_==NULL) {
        return first_->exec()->applyUnary(operator_);
    } else {
        return first_->exec()->applyBinary(operator_,second_->exec());
    }
}

/*
 * class RLRoboCommands : public RLCommandPrototype
 */
RLRoboCommands::RLRoboCommands(RLRoboMaze::Action action) {
    action_ = action;
}

RLCommandPrototype* RLRoboCommands::copy() const {
    return new RLRoboCommands(action_);
}

RLTypePrototype* RLRoboCommands::exec() const {
    return (new RLBool(RLRoboMaze::moveRobot(action_)))->markAsTemp();
}

/*
 * class RLChainCommands : public RLCommandPrototype
 */
RLChainCommands::~RLChainCommands() {
    for(int i = 0; i < chain_.size(); i++) {
        delete chain_[i];
    }
    
    chain_.clear();
}

RLCommandPrototype* RLChainCommands::copy() const {
    RLChainCommands* tmp = new RLChainCommands();
    
    for(int i = 0; i < chain_.size(); i++) {
        tmp->addCommand(chain_.at(i)->copy());
    }
    
    return tmp;
}

RLTypePrototype* RLChainCommands::exec() const {
    return exec_();
}

void RLChainCommands::addCommand(RLCommandPrototype* cmd) {
    if(cmd != NULL)
        chain_.push_back(cmd);
}

int RLChainCommands::getChainSize() const {
    return chain_.size() - 1;
}

RLTypePrototype* RLChainCommands::exec_() const {
    for(int i = 0; i < chain_.size(); i++) {
        try {
            (RLCommandPrototype*)(chain_.at(i))->exec();

            RLTypePrototype::clearTempVars(); // Cleaning temporary variables after execution line of code

        } catch(RLTypeException exc) {
            throw RLPerformException(exc.what(),chain_.at(i)->getLinePosition());

        } catch(RLMark::TransactionDescriptor descr) {
            /*if(descr.dawnPerformStackTo == this)
                i = descr.goToLine;
            else if(descr.dawnPerformStackTo==RLInterpreter::getMainFunction())
                throw RLPerformException("Can't make transition.",chain_.at(i)->getLinePosition());
            else
                throw descr;
            */

        }
    }
    
    return NULL;
}

/*
 * class RLConditional : public RLCommandBase
 */
RLConditional::RLConditional(RLCommandPrototype* effectcode, RLCommandPrototype* condition) {
    Truth_ = (new RLBool(true))->markAsConst();

    condition_ = new RLCommand(compare,condition,new RLDereference(Truth_));

    effect_ = effectcode;
}

RLConditional::~RLConditional() {
    delete condition_;
}

RLCommandPrototype* RLConditional::copy() const {
    return new RLConditional(effect_->copy(),condition_->copy());
}

RLTypePrototype* RLConditional::exec() const {
    bool accept = isAccept_();

    if(accept)
        exec_();

    return (new RLBool(accept))->markAsTemp();
}

bool RLConditional::isAccept_() const {
    RLTypePrototype* res = condition_->exec();

    if(res->getTypeQualifier() == RLTypePrototype::Bool)
        return ((RLBool*)res)->getValue();
    else
        RLTypeException("Unexpected comparison error. Try to inspect your \'compare\' block in RLTypePrototype::applyBinary().\n");
}

void RLConditional::exec_() const {
    effect_->exec();
}

/*
 * class RLCycle : public RLConditional
 */
RLCycle::RLCycle(RLCommandPrototype* effectcode, RLCommandPrototype* condition)
    : RLConditional(effectcode,condition) {
}

RLTypePrototype* RLCycle::exec() const {
    int i = 0;

    while(isAccept_()) {
        exec_();

        if(i++ > MaxCycleIteration_) {
            return (new RLBool(false))->markAsTemp();
        }
    }

    return (new RLBool(true))->markAsTemp();
}

RLCommandPrototype* RLCycle::copy() const {
    return new RLCycle(effect_,condition_);
}
