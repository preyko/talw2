#include <iostream>
#include <string>
#include <map>

enum RLOperator { increment, decrement, assign, compare, arrayat };

/* Solution to the crosslinks problem */
class RLType;

typedef std::map<int,RLType*> IdentifierRegister;
typedef std::pair<int,RLType*> IdentifierRegisterPair;

class RLIdentifierRegister {
public:
	bool add(int id, RLType* identifier);

	RLType* get(int id);

private:
	IdentifierRegister register_;
};

class RLType {
public:
	enum RLTypeQualifier { Base, Bool, Number, Array };
	
	struct RLTypeMeta {
		RLTypeQualifier typeName;	
	};

    static void setRegister(RLIdentifierRegister* iregister);
    static std::string typeName(RLTypeQualifier qualifier);

	RLType();

	bool reg(int id);

	virtual RLType* copy() const;

    virtual RLType applyUnary(RLOperator);
    virtual RLType applyBinary(RLOperator, RLType*);

    RLTypeQualifier getTypeQualifier() const;

    virtual void print();

protected:
	RLTypeMeta meta_;

	static RLIdentifierRegister* iregister_;
};

class RLBool : public RLType {
public:
	RLBool(bool value);
	RLBool(bool value, int id);

	virtual RLType* copy() const;

	bool getValue() const;
	void setValue(bool val);

    virtual RLType applyUnary(RLOperator oper);
    virtual RLType applyBinary(RLOperator oper, RLType* val);

    virtual void print();

protected:
	bool value_;

private:
	void init_(bool val);
};

class RLNumber : public RLType {
public:
	RLNumber(int value);
	RLNumber(int value, int id);

	virtual RLType* copy() const;

	int getValue() const;
	void setValue(int val);

    virtual RLType applyUnary(RLOperator oper);
    virtual RLType applyBinary(RLOperator oper, RLType* val);

    virtual void print();

protected:
	int value_;

private:
	void init_(int val);
};

class RLArray : public RLType {
public:
    typedef std::map<int, RLType*> RLArrayStorage;
    typedef std::pair<int, RLType*> RLArrayStoragePair;

    RLArray(RLTypeQualifier qualifier);
    RLArray(RLTypeQualifier qualifier, int key);

	virtual RLType* copy() const;

    void setElem(int pos, RLType* element);
    RLType* getElem(int pos) const;

    RLTypeQualifier getElemQualifier() const;

    virtual RLType applyUnary(RLOperator oper);
    virtual RLType applyBinary(RLOperator oper, RLType* val);

    virtual void print();

protected:
    RLTypeQualifier arrayType_;
    RLArrayStorage elements_;

private:
    void init_(RLTypeQualifier qualifier);
};
