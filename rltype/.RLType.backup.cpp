#include <iostream>
#include <string>
#include <map>

enum RLOperator { increment, decrement, assign, compare };

struct RLTypeMeta {
	std::string typeName;	
};

class RLType;
class RLIdentifierRegister {
public:
	bool add(int id, RLType* identifier) {
		if(!register_.find(id)) {
			register_.insert(identifier->copy());
			
			return true;
		} else {
			// Bring an ERROR:
			// ID collision.
			return false;
		}
	}

	RLType* get(int id) {
		return register_.find(id);
	}

private:
	std::map<int,RLType*> register_;
};

class RLType {
public:
	RLType() {
		if(iregister_==NULL) {
			register_ = new RLIdentifierRegister();
		}
	}

	bool reg(int id) {
		return register_->add(id,this);
	}

	virtual RLType* copy() const {
		return new RLType();
	}

	virtual bool applyUnary(RLOperator) {
		// ERROR! Can't apply any operator for base RLType
		return false;
	}
	
	virtual bool applyBinary(RLOperator, RLType*) {
		// ERROR! Can't apply any operator for base RLType
		return false;
	}

	std::string getTypeName() const {
		return meta_.typeName;

protected:
	RLTypeMeta meta_;

	static RLIdentifierRegister* iregister_;
};

class RLBool : public RLType {
public:
	RLBool(bool value) {
		value_ = value;
	}
	RLBool(bool value, int id) P
		RLBool(value);
		reg(id);
	}

	bool getValue() {
		return value_;
	}
	void setValue(bool val) {
		value_ = val;
	}

	virtual bool applyUnary(ELOperator oper) {
		// ERROR! Not one unary operators not exist for RLBool
	}
	
	virtual bool applyBinary(RLOperator oper, RLType* val) {
		switch(oper) {
		case compare:
			if(val->getTypeName()=="RLBool") {
				RLBool* valb = (RLBool*)val;
				return getValue() == valb.getValue();
			} else if(val->getTypeName()=="RLNumb") {
				RLNumb* valn = (RLNumb*)val;
				return getValue() == (bool)valn.getValue();
			} else {
				// Can't compare RLBool with val->getTypeName()
			}
		break;
		case assign:
			if(val->getTypeName()=="RLBool") {
				RLBool* valb = (RLBool*)val;
				return setValue(valb.getValue());
			} else {
				// Can't assign RLBool with val->getTypeName()
			}
		break;
		default:
			// Unexpected unary operator for RLBool
		break;
		}
	}

protected:
	bool value_;
};

class RLNumber : public RLType {
public:
	RLNumber(int value) {
		value_ = value;
	}
	RLNumber(int value, int id) {
		RLNumber(value);
		reg(id);
	}

	bool getValue() {
		return value_;
	}
	void setValue(bool val) {
		value_ = val;
	}

	virtual bool applyUnary(ELOperator oper) {
		switch(oper) {
		case increment:
			value_++;
		break;
		case decrement:
			value_--;
		break;
		default:
			// Unexpected unary operator for RLNumb
		break;
		}		
	}
	
	virtual bool applyBinary(RLOperator oper, RLType* val) {
		switch(oper) {
		case compare:
			if(val->getTypeName()=="RLNumb") {
				RLNumb* valn = (RLNumb*)val;
				return getValue() == valn.getValue();
			} else {
				// Can't compare RLNumb with val->getTypeName()
			}
		break;
		case assign:
			if(val->getTypeName()=="RLNumb") {
				RLNumb* valn = (RLNumb*)val;
				return setValue(valn.getValue());
			} else {
				// Can't assign RLNumb with val->getTypeName()
			}
		break;
		default:
			// Unexpected unary operator for RLBool
		break;
		}
	}

protected:
	int value_;
};
