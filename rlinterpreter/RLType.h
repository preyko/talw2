#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>


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

enum RLOperator { show, np, increment, decrement, assign, compare, arrayat, maketransition, perform };

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

class RLTypePrototype {
public:
    typedef std::vector<RLTypePrototype*> LinkCounter;

    enum RLTypeQualifier { Base, Bool, Number, Array, Mark, Procedure };
	
	struct RLTypeMeta {
		RLTypeQualifier typeName;	
	};

    static void killThemAll();
    static std::string typeName(RLTypeQualifier qualifier);

    RLTypePrototype();

	bool reg(int id);

    virtual RLTypePrototype* copy() const;

    virtual RLTypePrototype* applyUnary(RLOperator);
    virtual RLTypePrototype* applyBinary(RLOperator, RLTypePrototype*);

    RLTypeQualifier getTypeQualifier() const;

    virtual void print();

protected:
    RLTypeMeta meta_;

    static LinkCounter linkCounter_;
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

    void setElem(int pos, RLTypePrototype* element);
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

class RLProcedure;
class RLMark : public RLTypePrototype {
    RLMark(RLTypePrototype* owner, int line);
    RLMark(RLTypePrototype* owner, int line, int id);

    virtual RLTypePrototype* copy() const;

    void setOwner(RLProcedure* nowner);
    void setLinePointer(int nline);

    virtual RLTypePrototype* applyUnary(RLOperator oper);
    virtual RLTypePrototype* applyBinary(RLOperator oper, RLTypePrototype* val);

    virtual void print();

protected:
    int linePointer_;

    RLProcedure* owner_;

private:
    void init_(RLTypePrototype* owner,int line);
};


class RLCommandPrototype;
class RLProcedure : public RLTypePrototype {
public:
    typedef std::vector<RLCommandPrototype*> RLCommandChain;

    RLProcedure();
    RLProcedure(int id);
    ~RLProcedure();

    virtual RLTypePrototype* copy() const;

    void addCommand(RLCommandPrototype* c);

    void setLinePointer(int nline);

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
};

template<class T>
T RLcast(RLTypePrototype* type);
