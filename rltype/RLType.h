#include <iostream>
#include <string>
#include <map>

enum RLOperator { increment, decrement, assign, compare };

struct RLTypeMeta {
	std::string typeName;	
};

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
	RLType();

	bool reg(int id);

	virtual RLType* copy() const;

	virtual bool applyUnary(RLOperator);
	virtual bool applyBinary(RLOperator, RLType*);

	std::string getTypeName() const;

	static void setRegister(RLIdentifierRegister* iregister);

protected:
	RLTypeMeta meta_;

	static RLIdentifierRegister* iregister_;
};

class RLBool : public RLType {
public:
	RLBool(bool value);
	RLBool(bool value, int id);

	bool getValue();
	void setValue(bool val);

	virtual bool applyUnary(RLOperator oper);
	virtual bool applyBinary(RLOperator oper, RLType* val);

protected:
	bool value_;

private:
	void init_(bool val);
};

class RLNumber : public RLType {
public:
	RLNumber(int value);
	RLNumber(int value, int id);

	bool getValue();
	void setValue(bool val);

	virtual bool applyUnary(RLOperator oper);
	virtual bool applyBinary(RLOperator oper, RLType* val);

protected:
	int value_;

private:
	void init_(int val);
};
