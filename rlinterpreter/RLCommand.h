#pragma once

#include "RLType.h"
#include "RLTools.h"
#include "RLInterpreter.h"

#include "RLRoboMaze.h"

class RLPerformException {
public:
    RLPerformException(std::string descr,int line) {
        errLine_ = line;
        description_ = descr;
    }

    int whatLine() {
        return errLine_;
    }

    std::string what() {
        return description_;
    }

private:
    int errLine_;
    std::string description_;
};


class RLCommandPrototype {
    friend class RLProcedure;

public:
    virtual RLCommandPrototype* copy() const = 0;

    virtual RLTypePrototype* exec() const = 0;

    virtual RLTypePrototype* getFinalLeft(); // Recursively return left type element  without  performing of command, placed in RLDereference.

    void setLinePosition(int line);
    int getLinePosition();

private:
    int linePosition_;

};

class RLDereference : public RLCommandPrototype {
public:
    RLDereference(int ref);
    RLDereference(RLTypePrototype* ref);
    ~RLDereference();

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;

    virtual RLTypePrototype* getFinalLeft();

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

    virtual RLTypePrototype* getFinalLeft();

protected:
    virtual RLTypePrototype* exec_() const;

private:
    RLCommandPrototype* first_;
    RLCommandPrototype* second_;

    RLOperator operator_;

    void init_(RLOperator oper, int f, int s);
    void init_(RLOperator oper, RLCommandPrototype* f, RLCommandPrototype* s);
};

class RLRoboCommands : public RLCommandPrototype {
public:
    RLRoboCommands(RLRoboMaze::Action action);

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;

private:
    RLRoboMaze::Action action_;

};

class RLChainCommands : public RLCommandPrototype  {
public:
    typedef std::vector<RLCommandPrototype*> ChainType;
    
    ~RLChainCommands();
    
    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;
    
    void addCommand(RLCommandPrototype* cmd);
    
    int getChainSize() const;
    
protected:
    virtual RLTypePrototype* exec_() const;
    
private:
    ChainType chain_;
    
};

class RLConditional : public RLCommandPrototype {
public:
    RLConditional(RLCommandPrototype* effectcode, RLCommandPrototype* condition);
    ~RLConditional();

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;

protected:
    void exec_() const;
    bool isAccept_() const;

    RLCommandPrototype* condition_;
    RLCommandPrototype* effect_;

private:
    RLTypePrototype* Truth_;
    RLCommandPrototype* TestOfFaith_;

};

class RLCycle : public RLConditional {
public:
    RLCycle(RLCommandPrototype* effectcode, RLCommandPrototype* condition);

    virtual RLCommandPrototype* copy() const;

    virtual RLTypePrototype* exec() const;

private:
    static const int MaxCycleIteration_ = 250;

};
