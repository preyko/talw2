#include "RLCommand.h"


/*
 * class RLDereference : public RLCommandBase
 */
RLDereference::RLDereference(int ref) {
    RLTypePrototype* res = RLIdentRegister::get(ref);

    if(res == NULL) {
        // Identifier ref does not declared before.
        std::cout << "ERROR: Identifier " << ref << " does not declared before.\n";

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
    return value_;
}

void RLDereference::print() const {
    std::cout << "Container for:\n";
    value_->print();
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

void RLCommand::print() const {

}

RLTypePrototype* RLCommand::exec_() const {
    if(first_==NULL && second_==NULL) {
        return new RLBool(false);
    } else if(first_!=NULL && second_==NULL) {
        return first_->exec()->applyUnary(operator_);
    } else {
        return first_->exec()->applyBinary(operator_,second_->exec());
    }
}


/*
 * class RLConditional : public RLCommandBase
 */
RLConditional::RLConditional(RLCommandPrototype* effectcode, RLCommandPrototype* condition) {
    Truth_ = new RLBool(true);

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

    return new RLBool(accept);
}

void RLConditional::print() const {

}

bool RLConditional::isAccept_() const {
    RLTypePrototype* res = condition_->exec();

    if(res->getTypeQualifier() == RLTypePrototype::Bool)
        return ((RLBool*)res)->getValue();
    else
        std::cout << "Unexpected comparsion error. Try to inspect your \'compare\' block in RLTypePrototype::applyBinary().\n";
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
            return new RLBool(false);
        }
    }

    return new RLBool(true);
}

RLCommandPrototype* RLCycle::copy() const {
    return new RLCycle(effect_,condition_);
}

void RLCycle::print() const {

}
