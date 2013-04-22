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
    init_(oper,new RLDereference(f),new RLDereference(s));
}

RLCommand::RLCommand(RLOperator oper, int idf, int ids) {
    init_(oper,f,s);
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

RLTypePrototype* RLCommand::exec_() {
    if(first_==NULL && second_==NULL) {
        return RLBool(false);
    } else if(first_!=NULL && second_==NULL) {
        return first_->exec()->applyUnary(operator_);
    } else {
        return first_->exec()->applyBinary(operator_,second_->exec());
    }
}


/*
 * class RLConditional : public RLCommandBase
 */
RLConditional::RLConditional(RLTypePrototype* effectcode, RLProcedure* condition) {
}


