#include <iostream>

#include "RLType.h"

/*
 * class RLIdentifierRegister
 */
bool RLIdentifierRegister::add(int id, RLType* identifier) {
	if(register_.find(id) == register_.end()) {
		register_.insert(IdentifierRegisterPair(id,identifier));
		
		return true;
	} else {
		// Bring an ERROR:
		// ID collision.
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: ID collision.\n";
#endif
		return false;
	}
}

RLType* RLIdentifierRegister::get(int id) {
	return register_.find(id)->second;
}


/*
 * class RLType
 */
RLIdentifierRegister* RLType::iregister_ = NULL; // Definition of static class member

RLType::RLType() {
	
}

bool RLType::reg(int id) {
	return iregister_->add(id,this);
}

RLType* RLType::copy() const {
	return new RLType();
}

bool RLType::applyUnary(RLOperator) {
	// ERROR! Can't apply any operator for base RLType
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Can't apply any operator for base RLType.\n";
#endif
	return false;
}

bool RLType::applyBinary(RLOperator, RLType*) {
	// ERROR! Can't apply any operator for base RLType
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Can't apply any operator for base RLType.\n";
#endif
	return false;
}

std::string RLType::getTypeName() const {
	return meta_.typeName;
}

void RLType::setRegister(RLIdentifierRegister* iregister) {
	iregister_ = iregister;
}


/*
 * class RLBool : public RLType
 */
RLBool::RLBool(bool val) : RLType() {
	init_(val);
}
RLBool::RLBool(bool val, int id) {
	init_(val);
	reg(id);
}
void RLBool::init_(bool val) {
	setValue(val);
	meta_.typeName = "RLBool";
}

bool RLBool::getValue() {
	return value_;
}
void RLBool::setValue(bool val) {
	value_ = val;
}

bool RLBool::applyUnary(RLOperator oper) {
	// ERROR! Not one unary operators not exist for RLBool
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Not one unary operators not exist for RLBool.\n";
#endif
}

bool RLBool::applyBinary(RLOperator oper, RLType* val) {
	switch(oper) {
	case compare:
		if(val->getTypeName()=="RLBool") {
			RLBool* valb = (RLBool*)val;
			return getValue() == valb->getValue();
		} else if(val->getTypeName()=="RLNumber") {
			RLNumber* valn = (RLNumber*)val;
			return getValue() == (bool)valn->getValue();
		} else {
			// Can't compare RLBool with val->getTypeName()
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Can't compare RLBool with " << val->getTypeName() << ".\n";
#endif
		}
	break;
	case assign:
		if(val->getTypeName()=="RLBool") {
			RLBool* valb = (RLBool*)val;
			setValue(valb->getValue());
			return true;
		} else {
			// Can't assign RLBool with val->getTypeName()
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Can't assign RLBool with " << val->getTypeName() << ".\n";
#endif
		}
	break;
	default:
		// Unexpected unary operator for RLBool
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Unexpected unary operator for RLBool.\n";
#endif
	break;
	}
}


/*
 * class RLNumber : public RLType
 */
RLNumber::RLNumber(int val) : RLType() {
	init_(val);
}
RLNumber::RLNumber(int val, int id) {
	init_(val);
	reg(id);
}
void RLNumber::init_(int val) {
	setValue(val);
	meta_.typeName = "RLNumber";
}

bool RLNumber::getValue() {
	return value_;
}
void RLNumber::setValue(bool val) {
	value_ = val;
}

bool RLNumber::applyUnary(RLOperator oper) {
	switch(oper) {
	case increment:
		value_++;
	break;
	case decrement:
		value_--;
	break;
	default:
		// Unexpected unary operator for RLNumb
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Unexpected unary operator for RLNumb.\n";
#endif
	break;
	}		
}

bool RLNumber::applyBinary(RLOperator oper, RLType* val) {
	switch(oper) {
	case compare:
		if(val->getTypeName()=="RLNumber") {
			RLNumber* valn = (RLNumber*)val;
			return getValue() == valn->getValue();
		} else {
			// Can't compare RLNumb with val->getTypeName()
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Can't compare RLNumb with " << val->getTypeName() << ".\n";
#endif
		}
	break;
	case assign:
		if(val->getTypeName()=="RLNumber") {
			RLNumber* valn = (RLNumber*)val;
			setValue(valn->getValue());			
			return true;
		} else {
			// Can't assign RLNumb with val->getTypeName()
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Can't assign RLNumb with " << val->getTypeName() << ".\n";
#endif
		}
	break;
	default:
		// Unexpected unary operator for RLBool
#ifdef RLINTERP_DEBUG
		std::cout << "ERROR: Unexpected unary operator for RLBool.\n";
#endif
	break;
	}
}

