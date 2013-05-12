#pragma once

#include <iostream>
#include <string>

#include <algorithm>
#include <vector>
#include <map>

#include "RLTools.h"


/* Solution to the crosslinks problem */
class RLTypePrototype;
class RLProcedure;
class RLCommandPrototype;
class RLChainCommands;

/*
 * RLTypeException: Exception that describe errors with RLTypes in run time.
 */
class RLTypeException {
public:
    RLTypeException(std::string descr) {
        description_ = descr;
    }

    std::string what() {
        return description_;
    }

private:
    std::string description_;
};

/* Aviabale operators */
enum RLOperator { show, np, increment, decrement, assign, compare, arrayat, maketransition, perform, bind, unbind };


/*
 * RLIdentRegister: Reestr of identifier that keep variables.
 */
typedef std::map<int,RLTypePrototype*> IdentifierRegister;
typedef std::pair<int,RLTypePrototype*> IdentifierRegisterPair;

class RLIdentRegister {
public:
    static bool add(int id, RLTypePrototype* identifier);

    static RLTypePrototype* get(int id);

    static void clear();
    static void showAll();

private:
    static IdentifierRegister register_;
};


/*
 * RLTypePrototype: Base type for each other RLType.
 */
class RLTypePrototype {
public:
    typedef std::vector<RLTypePrototype*> LinkCounter;

    enum RLTypeQualifier { Base, Bool, Number, Array, Mark, Procedure };
	
	struct RLTypeMeta {
		RLTypeQualifier typeName;	
	};

    /* This members provide protection of memory leak */
    static void clearTempVars();
    static void clearConstVars();
    static void clearDynamicallyAllocatedVars();
    static void killThemAll(); // Clear IdentRegister and Constant variables.
    RLTypePrototype* markAsTemp();
    RLTypePrototype* markAsConst();
    RLTypePrototype* markAsDynamicallyAllocated();

    static std::string typeName(RLTypeQualifier qualifier);


    RLTypePrototype();

	bool reg(int id);

    virtual RLTypePrototype* copy() const;

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    RLTypeQualifier getTypeQualifier() const;

    virtual void print();

    bool linkWithProcedure(RLProcedure* proc);
    bool breakLinkWithProcedure(RLProcedure* proc);
    void performLinkedProcedures();

protected:
    RLTypeMeta meta_;

private:
    std::vector<RLProcedure*> linkedProcedures_;

    static LinkCounter tempVars_;
    static LinkCounter constVars_;
    static LinkCounter dynamicVars_;
};

class RLBool : public RLTypePrototype {
public:
	RLBool(bool value);
	RLBool(bool value, int id);

    virtual RLTypePrototype* copy() const;

	bool getValue() const;
	void setValue(bool val);

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

protected:
	bool value_;

private:
	void init_(bool val);
};

class RLNumber : public RLTypePrototype {
public:
	RLNumber(int value);
	RLNumber(int value, int id);

    virtual RLTypePrototype* copy() const;

	int getValue() const;
	void setValue(int val);

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

protected:
	int value_;

private:
	void init_(int val);
};

class RLArray : public RLTypePrototype {
public:
    typedef std::map<int, RLTypePrototype*> RLArrayStorage;
    typedef std::pair<int, RLTypePrototype*> RLArrayStoragePair;

    RLArray(int depth, RLTypeQualifier rootqualifier);
    RLArray(int depth, RLTypeQualifier rootqualifier,int id);
    ~RLArray();

    virtual RLTypePrototype* copy() const;

    void setElem(int pos,RLTypePrototype* elem);
    RLTypePrototype* getElem(int pos) const;
    
    int getRootDepth() const;
    void setRootDepth(int depth);
    
    RLTypeQualifier getRootQualifier() const;
    void setRootQualifier(RLTypeQualifier qual);
    
    void clear();

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

private:
    void init_(int depth, RLTypeQualifier rootqualifier);

    void print_(std::string delimetr = "");
    
    int currentRootDepth_;
    RLTypeQualifier rootType_;
    
    RLArrayStorage elements_;
};

class RLMark : public RLTypePrototype {
public:

    static const int MaxTransactionCount_ = 255;

    struct TransactionDescriptor {
        RLProcedure* dawnPerformStackTo;
        int goToLine;
    };

    RLMark(RLTypePrototype* owner, int line);
    RLMark(RLTypePrototype* owner, int line, int id);

    virtual RLTypePrototype* copy() const;

    void setOwner(RLProcedure* nowner);
    void setLinePointer(int nline);

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

protected:
    int transactionCount_;

    int linePointer_;
    RLProcedure* owner_;

private:
    void init_(RLTypePrototype* owner,int line);
};


class RLProcedure : public RLTypePrototype {
public:
    RLProcedure();
    RLProcedure(int id);
    ~RLProcedure();

    virtual RLTypePrototype* copy() const;

    void addCommand(RLCommandPrototype* c);

    void setLinePointer(int nline);

    int getSize() const;

    void exec();

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

protected:
    int currentLinePointer_;

private:
    void init_();

    RLChainCommands* chain_;
};
