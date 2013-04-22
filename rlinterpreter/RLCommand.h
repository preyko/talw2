#pragma once

#include "RLType.h"


class RLCommandPrototype {
    friend class RLProcedure;

public:
    enum RLCommandType { Base, Dereference, Usual, Conditional };

    virtual RLCommandPrototype* copy() const = 0;

    virtual RLTypePrototype* exec() const = 0;

    virtual void print() const = 0;

protected:
    RLCommandType commandType_;

};

class RLDereference : public RLCommandPrototype {
public:
    RLDereference(int ref);
    RLDereference(RLTypePrototype* ref);
    ~RLDereference();

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;

    virtual void print() const;

private:
    RLTypePrototype* value_;

};

class RLCommand : public RLCommandPrototype {
public:
    RLCommand(RLOperator oper, RLCommandPrototype* f, RLCommandPrototype* s = NULL);
    RLCommand(RLOperator oper, RLTypePrototype* f, RLTypePrototype* s = NULL);
    RLCommand(RLOperator oper, int idf, int ids = -1);
    ~RLCommand();

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;

    virtual void print() const;

protected:
    virtual RLTypePrototype* exec_();

private:
    RLCommandPrototype* first_;
    RLCommandPrototype* second_;

    RLOperator operator_;

    void init_(RLOperator oper, int f, int s);
    void init_(RLOperator oper, RLCommandPrototype* f, RLCommandPrototype* s);
};

class RLConditional : public RLCommandPrototype {
public:
    RLConditional(RLTypePrototype* effectcode, RLProcedure* condition);
    ~RLConditional();

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec();

    virtual void print();

protected:
    void exec_();
    bool isAccept_();

    RLCommandPrototype* condition_;
    RLProcedure* effect_;

};

