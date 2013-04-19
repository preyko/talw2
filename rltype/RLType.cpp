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
		std::cout << "ERROR: ID collision.\n";

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
    if(iregister_==NULL) {
        // ERROR! Register of identifier has not been declared,\n\tso use static method RLType::setRegister(RLIdentifierRegister*)
        std::cout << "ERROR: Register of identifier has not been declared,\n\tso use static method RLType::setRegister(RLIdentifierRegister*)\n";

        return false;
    }
	return iregister_->add(id,this);
}

RLType* RLType::copy() const {
	return new RLType();
}

RLType* RLType::applyUnary(RLOperator) {
	// ERROR! Can't apply any operator for base RLType
    std::cout << "ERROR: Can't apply any operator for base RLType.\n";

    return new RLBool(false);
}

RLType* RLType::applyBinary(RLOperator, RLType*) {
	// ERROR! Can't apply any operator for base RLType
    std::cout << "ERROR: Can't apply any operator for base RLType.\n";

    return new RLBool(false);
}

void RLType::print() {
    std::cout << "RLType = Base." << std::endl;
}

void RLType::setRegister(RLIdentifierRegister* iregister) {
    iregister_ = iregister;
}

RLType::RLTypeQualifier RLType::getTypeQualifier() const {
    return meta_.typeName;
}

std::string RLType::typeName(RLType::RLTypeQualifier qualifier) {
    switch(qualifier) {
    case Base:
        return "Base";
    break;
    case Bool:
        return "Bool";
    break;
    case Number:
        return "Number";
    break;
    case Array:
        return "Array";
    break;
    default:
        return "Unknown";
    break;
    }
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
	meta_.typeName = Bool;
}

RLType* RLBool::copy() const {
	return new RLBool(getValue());
}

bool RLBool::getValue() const {
	return value_;
}
void RLBool::setValue(bool val) {
	value_ = val;
}

RLType* RLBool::applyUnary(RLOperator oper) {
	// ERROR! Not one unary operators not exist for RLBool
    std::cout << "ERROR: Not one unary operators not exist for RLBool.\n";

}

RLType* RLBool::applyBinary(RLOperator oper, RLType* val) {
	switch(oper) {
	case compare:
		if(val->getTypeQualifier()==Bool) {
			RLBool* valb = (RLBool*)val;
            return new RLBool(getValue() == valb->getValue());
		} else if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            return new RLBool(getValue() == (bool)valn->getValue());
		} else {
			// Can't compare RLBool with val->getTypeQualifier()
            std::cout << "ERROR: Can't compare RLBool with " << RLType::typeName(val->getTypeQualifier()) << ".\n";

		}
	break;
	case assign:
		if(val->getTypeQualifier()==Bool) {
			RLBool* valb = (RLBool*)val;
			setValue(valb->getValue());
            return new RLBool(true);
		} else {
			// Can't assign RLBool with val->getTypeQualifier()
            std::cout << "ERROR: Can't assign RLBool with " << RLType::typeName(val->getTypeQualifier()) << ".\n";
            return new RLBool(false);
		}
	break;
	default:
        // Unexpected binary operator for RLBool
        std::cout << "ERROR: Unexpected binary operator for RLBool.\n";
        return new RLBool(false);
	break;
    }
}

void RLBool::print() {
    std::cout << "RLType = Bool, Value = " << value_ << std::endl;
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
	meta_.typeName = Number;
}

RLType* RLNumber::copy() const {
	return new RLNumber(getValue());
}

int RLNumber::getValue() const {
	return value_;
}
void RLNumber::setValue(int val) {
	value_ = val;
}

RLType* RLNumber::applyUnary(RLOperator oper) {
	switch(oper) {
	case increment:
		value_++;
	break;
	case decrement:
		value_--;
	break;
	default:
		// Unexpected unary operator for RLNumb
		std::cout << "ERROR: Unexpected unary operator for RLNumb.\n";

        return new RLBool(false);
	break;
    }
    return new RLBool(true);
}

RLType* RLNumber::applyBinary(RLOperator oper, RLType* val) {
    switch(oper) {
	case compare:
		if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            return new RLBool(getValue() == valn->getValue());
		} else {
			// Can't compare RLNumb with val->getTypeQualifier()
            std::cout << "ERROR: Can't compare RLNumb with " << RLType::typeName(val->getTypeQualifier()) << ".\n";

            return new RLBool(false);
		}
	break;
	case assign:
		if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
			setValue(valn->getValue());			
            return new RLBool(true);
		} else {
			// Can't assign RLNumb with val->getTypeQualifier()
            std::cout << "ERROR: Can't assign RLNumb with " << RLType::typeName(val->getTypeQualifier()) << ".\n";

            return new RLBool(false);
		}
	break;
	default:
        // Unexpected binary operator for RLBool
        std::cout << "ERROR: Unexpected binary operator for RLBool.\n";
        return new RLBool(false);
	break;
    }
}

void RLNumber::print() {
    std::cout << "RLType = Number, value = " << value_ << std::endl;
}


/*
 * class RLArray : public RLType
 */
RLArray::RLArray(RLTypeQualifier qualifier) {
    init_(qualifier);
}
RLArray::RLArray(RLTypeQualifier qualifier, int id) {
    init_(qualifier);
    reg(id);
}

void RLArray::init_(RLTypeQualifier qualifier) {
    meta_.typeName = Array;
    arrayType_ = qualifier;
}

RLType* RLArray::copy() const {
    RLArray* temp = new RLArray(getElemQualifier());

    for(RLArrayStorage::const_iterator i=elements_.begin();i!=elements_.end();i++) {
        temp->elements_.insert(RLArrayStoragePair(i->first,i->second->copy()));
    }

    return temp;
}

RLType* RLArray::getElem(int pos) const {
    if(elements_.find(pos)==elements_.end()) {
        // No such element in RLArray
        std::cout << "ERROR: No such element in RLArray with index " << pos << ".\n";

        return NULL;
    } else {
        return elements_.find(pos)->second;
    }
}

void RLArray::setElem(int pos, RLType* element) {
    if(element->getTypeQualifier()!=getElemQualifier()) {
        // ERROR: Can't put different RLType in this Array
        std::cout << "ERROR: Can\'t put "
                  << RLType::typeName(element->getTypeQualifier())
                  << " type in Array of "
                  << RLType::typeName(getElemQualifier())
                  << ".\n";

        return;
    }
    elements_.insert(RLArrayStoragePair(pos,element));
}

RLType::RLTypeQualifier RLArray::getElemQualifier() const {
    return arrayType_;
}

RLType* RLArray::applyUnary(RLOperator oper) {
    // ERROR! Not one unary operators not exist for RLArray
    std::cout << "ERROR: Not one unary operators not exist for RLArray.\n";

    return new RLBool(false);
}

RLType* RLArray::applyBinary(RLOperator oper, RLType *val) {
    switch(oper) {
    case compare:
        if(val->getTypeQualifier()==Array) {
            RLArray* vala = (RLArray*)val;

            if(vala->getElemQualifier() != getElemQualifier()) {
                // Different elements types
                return new RLBool(false);
            }

            if(elements_.size()!=vala->elements_.size()) {
                // Different arrays size
                return new RLBool(false);
            }

            RLArrayStorage::const_iterator i1 = elements_.begin();
            RLArrayStorage::const_iterator i2 = vala->elements_.begin();

            for(; i1 != elements_.end(); i1++, i2++) {
                RLType* res = i1->second->applyBinary(compare,i2->second);

                if(res->getTypeQualifier() == Bool) {
                    RLBool* resb = (RLBool*)res;
                    if(!resb->getValue()) {
                        return new RLBool(false);
                    }
                }
            }

            return new RLBool(true);
        } else {
            // Can't compare RLArray with val->getTypeQualifier()
            std::cout << "ERROR: Can't compare RLArray with " << RLType::typeName(val->getTypeQualifier()) << ".\n";

        }
    break;
    case assign:
        if(val->getTypeQualifier()==Array) {
            RLArray* vala = (RLArray*)val;

            if(vala->getElemQualifier() != getElemQualifier()) {
                // Different elements types
                return new RLBool(false);
            }

            RLArrayStorage::const_iterator i;

            for(i = vala->elements_.begin(); i != vala->elements_.end(); i++) {
                elements_.insert(RLArrayStoragePair(i->first,i->second->copy()));
            }

            return this;
        } else {
            // Can't assign RLArray with val->getTypeQualifier()
            std::cout << "ERROR: Can't assign RLBool with " << RLType::typeName(val->getTypeQualifier()) << ".\n";

            return new RLBool(false);
        }
    break;
    case arrayat:
        if(val->getTypeQualifier()==Number) {
            RLNumber* valn = (RLNumber*)val;

            RLType* res = getElem(valn->getValue());

            if(res != NULL) {
                return res;
            } else {
                return new RLBool(false);
            }
        } else {
            // Only RLNumber can be used in[]
            std::cout << "ERROR: Only RLNumber can be used in[].\n";

        }
    default:
        // Unexpected binary operator for RLArray
        std::cout << "ERROR: Unexpected binary operator for RLArray.\n";

        return new RLBool(false);
    break;
    }
}

void RLArray::print() {
    std::cout << "RLType = Array of " << RLType::typeName(getElemQualifier()) << ".\n\tValues\n";

    RLArrayStorage::const_iterator i;
    for(i = elements_.begin(); i != elements_.end(); i++) {
        std::cout << "[" << i->first << "] ";

        i->second->print();
    }
}


