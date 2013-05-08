#pragma once

#include <iostream>
#include <string>

#include <algorithm>
#include <vector>
#include <map>

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

/* Solution to the crosslinks problem */
class RLTypePrototype;

typedef std::map<int,RLTypePrototype*> IdentifierRegister;
typedef std::pair<int,RLTypePrototype*> IdentifierRegisterPair;

class RLIdentRegister {
public:
    static bool add(int id, RLTypePrototype* identifier);

    static RLTypePrototype* get(int id);

    static void clear();

private:
    static IdentifierRegister register_;
};


/*
 * RLTypePrototype: Base type for each other RLType.
 */
class RLProcedure;
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
    static void killThemAll(); // Clear IdentRegister and Constant variables.
    RLTypePrototype* markAsTemp();
    RLTypePrototype* markAsConst();

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

protected:
    RLTypeMeta meta_;

private:
    std::vector<RLProcedure*> linkedProcedures_;

    static LinkCounter tempVars_;
    static LinkCounter constVars_;
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

    RLArray(RLTypeQualifier qualifier);
    RLArray(RLTypeQualifier qualifier, int key);

    virtual RLTypePrototype* copy() const;

    RLTypePrototype* getElem(int pos) const;

    RLTypeQualifier getElemQualifier() const;

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

protected:
    RLTypeQualifier arrayType_;
    RLArrayStorage elements_;

private:
    void init_(RLTypeQualifier qualifier);
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


class RLCommandPrototype;
class RLProcedure : public RLTypePrototype {
    friend class RLTypePrototype;
public:
    typedef std::vector<RLCommandPrototype*> RLCommandChain;

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
    void printCommand(int line);

protected:
    int currentLinePointer_;

    RLCommandChain chain_;

private:
    void init_();
    void exec_();
    void clear_();

    RLProcedure* referenceTo_;
};
