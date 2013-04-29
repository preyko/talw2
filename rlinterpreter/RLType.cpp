#include "RLType.h"
#include "RLCommand.h"


/*
 * class RLIdentifierRegister
 */
bool RLIdentRegister::add(int id, RLTypePrototype* identifier) {
	if(register_.find(id) == register_.end()) {
		register_.insert(IdentifierRegisterPair(id,identifier));
		
		return true;
	} else {
		// Bring an ERROR:
		// ID collision.
        std::cout << "ERROR: ID collision. Id " << id << " exist.\n";

		return false;
	}
}

RLTypePrototype* RLIdentRegister::get(int id) {
    IdentifierRegister::iterator it = register_.find(id);
    return it->second;
}

IdentifierRegister RLIdentRegister::register_ = IdentifierRegister();


/*
 * class RLType
 */
RLTypePrototype::RLTypePrototype() {
}

bool RLTypePrototype::reg(int id) {
    RLIdentRegister::add(id,this);
}

RLTypePrototype* RLTypePrototype::copy() const {
    return new RLTypePrototype();
}

RLTypePrototype* RLTypePrototype::applyUnary(RLOperator oper) {
    switch(oper) {
        case show:
            print();

            return NULL;
        break;
        default:
            return this;
        break;
    }
}

RLTypePrototype* RLTypePrototype::applyBinary(RLOperator, RLTypePrototype*) {
	// ERROR! Can't apply any operator for base RLType
    std::cout << "ERROR: Can't apply any operator for base RLType.\n";

    return new RLBool(false);
}

void RLTypePrototype::print() {
    std::cout << "RLType = Base." << std::endl;
}

RLTypePrototype::RLTypeQualifier RLTypePrototype::getTypeQualifier() const {
    return meta_.typeName;
}

std::string RLTypePrototype::typeName(RLTypePrototype::RLTypeQualifier qualifier) {
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
RLBool::RLBool(bool val) : RLTypePrototype() {
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

RLTypePrototype* RLBool::copy() const {
	return new RLBool(getValue());
}

bool RLBool::getValue() const {
	return value_;
}
void RLBool::setValue(bool val) {
	value_ = val;
}

RLTypePrototype* RLBool::applyUnary(RLOperator oper) {
    RLTypePrototype::applyUnary(oper);

    return this;
}

RLTypePrototype* RLBool::applyBinary(RLOperator oper, RLTypePrototype* val) {
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
            std::cout << "ERROR: Can't compare RLBool with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";
		}
	break;
	case assign:
		if(val->getTypeQualifier()==Bool) {
			RLBool* valb = (RLBool*)val;
            setValue(valb->getValue());
		} else {
			// Can't assign RLBool with val->getTypeQualifier()
            std::cout << "ERROR: Can't assign RLBool with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";
		}
	break;
	default:
        // Unexpected binary operator for RLBool
        std::cout << "ERROR: Unexpected binary operator for RLBool.\n";
	break;
    }

    return this;
}

void RLBool::print() {
    std::cout << "RLType = Bool, Value = " << value_ << std::endl;
}


/*
 * class RLNumber : public RLType
 */
RLNumber::RLNumber(int val) : RLTypePrototype() {
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

RLTypePrototype* RLNumber::copy() const {
	return new RLNumber(getValue());
}

int RLNumber::getValue() const {
	return value_;
}
void RLNumber::setValue(int val) {
	value_ = val;
}

RLTypePrototype* RLNumber::applyUnary(RLOperator oper) {
    if(RLTypePrototype::applyUnary(oper) == NULL) {
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
        break;
        }
    }

    return this;
}

RLTypePrototype* RLNumber::applyBinary(RLOperator oper, RLTypePrototype* val) {
    switch(oper) {
	case compare:
		if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            return new RLBool(getValue() == valn->getValue());
		} else {
			// Can't compare RLNumb with val->getTypeQualifier()
            std::cout << "ERROR: Can't compare RLNumb with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";

            return new RLBool(false);
		}
	break;
	case assign:
		if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            setValue(valn->getValue());
		} else {
			// Can't assign RLNumb with val->getTypeQualifier()
            std::cout << "ERROR: Can't assign RLNumb with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";
		}
	break;
	default:
        // Unexpected binary operator for RLBool
        std::cout << "ERROR: Unexpected binary operator for RLBool.\n";
	break;
    }

    return this;
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

RLTypePrototype* RLArray::copy() const {
    RLArray* temp = new RLArray(getElemQualifier());

    for(RLArrayStorage::const_iterator i=elements_.begin();i!=elements_.end();i++) {
        temp->elements_.insert(RLArrayStoragePair(i->first,i->second->copy()));
    }

    return temp;
}

RLTypePrototype* RLArray::getElem(int pos) const {
    if(elements_.find(pos)==elements_.end()) {
        // No such element in RLArray
        std::cout << "ERROR: No such element in RLArray with index " << pos << ".\n";

        return NULL;
    } else {
        return elements_.find(pos)->second;
    }
}

void RLArray::setElem(int pos, RLTypePrototype* element) {
    if(element->getTypeQualifier()!=getElemQualifier()) {
        // ERROR: Can't put different RLType in this Array
        std::cout << "ERROR: Can\'t put "
                  << RLTypePrototype::typeName(element->getTypeQualifier())
                  << " type in Array of "
                  << RLTypePrototype::typeName(getElemQualifier())
                  << ".\n";

        return;
    }
    elements_.insert(RLArrayStoragePair(pos,element));
}

RLTypePrototype::RLTypeQualifier RLArray::getElemQualifier() const {
    return arrayType_;
}

RLTypePrototype* RLArray::applyUnary(RLOperator oper) {
    RLTypePrototype::applyUnary(oper);
}

RLTypePrototype* RLArray::applyBinary(RLOperator oper, RLTypePrototype *val) {
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
                    RLTypePrototype* res = i1->second->applyBinary(compare,i2->second);

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
                std::cout << "ERROR: Can't compare RLArray with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";

                return new RLBool(false);
            }
        break;
        case assign:
            if(val->getTypeQualifier()==Array) {
                RLArray* vala = (RLArray*)val;

                if(vala->getElemQualifier()!= getElemQualifier()) {
                    // Different elements types
                    return new RLBool(false);
                }

                RLArrayStorage::const_iterator i;

                for(i = vala->elements_.begin(); i != vala->elements_.end(); i++) {
                    elements_.insert(RLArrayStoragePair(i->first,i->second->copy()));
                }
            } else {
                // Can't assign RLArray with val->getTypeQualifier()
                std::cout << "ERROR: Can't assign RLArray with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";
            }
        break;
        case arrayat:
            if(val->getTypeQualifier()==Number) {
                RLNumber* valn = (RLNumber*)val;

                RLTypePrototype* res = getElem(valn->getValue());

                if(res != NULL) {
                    return res;
                }
            } else {
                // Only RLNumber can be used in[]
                std::cout << "ERROR: Only RLNumber can be used in[].\n";
            }
        default:
            // Unexpected binary operator for RLArray
            std::cout << "ERROR: Unexpected binary operator for RLArray.\n";
        break;
    }

    return this;
}

void RLArray::print() {
    std::cout << "RLType = Array of " << RLTypePrototype::typeName(getElemQualifier()) << ".\n\tValues\n";

    RLArrayStorage::const_iterator i;
    for(i = elements_.begin(); i != elements_.end(); i++) {
        std::cout << "[" << i->first << "] ";

        i->second->print();
    }
}


/*
 * class RLLabel : public RLType
 */
RLMark::RLMark(RLTypePrototype* owner, int line) {
    init_(owner,line);
}

RLMark::RLMark(RLTypePrototype* owner, int line, int id) {
    init_(owner,line);
    reg(id);
}

void RLMark::init_(RLTypePrototype* owner, int line) {
    meta_.typeName = Mark;

    std::cout << "RLMark in development, it will come later.\n";

    if(owner->getTypeQualifier() != Procedure) {
        // Only RLProcedure can be owner of RLMark.
        std::cout << "ERROR: Only RLProcedure can be owner of RLMark.\n";
        return;
    }
    setOwner((RLProcedure*) owner);
    setLinePointer(line);
}

RLTypePrototype* RLMark::copy() const {
    return new RLMark(owner_,linePointer_);
}

void RLMark::setOwner(RLProcedure* nowner) {
    owner_ = nowner;
}

void RLMark::setLinePointer(int nline) {
    linePointer_ = nline;
}

RLTypePrototype* RLMark::applyUnary(RLOperator oper) {
    if(RLTypePrototype::applyUnary(oper) == NULL) {
        switch(oper) {
            case maketransition:
                if(owner_!=NULL) {
                    owner_->setLinePointer(linePointer_);
                    return new RLBool(true);
                } else {
                    // RLMark transaction cannot be performed because procedure Owner is not declared.
                    std::cout << "ERROR: RLMark transaction cannot be performed because procedure Owner is not declared.\n";
                }
            break;
            default:
                // Can't perform not \'makeTransition\' operation for RLMark
                std::cout << "ERROR: Can't perform not \'makeTransition\' operation for RLMark\n";
            break;
        }
    }

    return new RLBool(false);
}

RLTypePrototype* RLMark::applyBinary(RLOperator oper, RLTypePrototype* val) {
    // No one binary operation declared for RLMark.
    std::cout << "No one binary operation declared for RLMark.\n";

    return this;
}

void RLMark::print() {
    if(owner_!=NULL) {
        std::cout << "RLMark for RLProcedure on line " << linePointer_ << "\n\t";
        owner_->printCommand(linePointer_);
    } else {
        // This RLMark does not have owner RLProcedure.
        std::cout << "This RLMark does not have owner RLProcedure.\n";
    }
}

/*
 * class RLProcedure : public RLType
 */
RLProcedure::RLProcedure() {
    init_();
}

RLProcedure::RLProcedure(int id) {
    init_();
    reg(id);
}

void RLProcedure::init_() {
    meta_.typeName = Procedure;

    currentLinePointer_ = 0;
}

RLProcedure::~RLProcedure() {
    clear_();
}

void RLProcedure::clear_() {
    for(int i = 0; i < chain_.size(); i++) {
        delete chain_.at(i);
    }
}

RLTypePrototype* RLProcedure::copy() const {
    RLProcedure* res = new RLProcedure();

    for(int i = 0; i < chain_.size(); i++) {
        //if(chain_.at(i)->first_->getTypeQualifier()==Mark) {
        //    RLMark* tmp = (RLMark*) chain_.at(i)->first_->copy();
        //    tmp->setOwner(res);
        //    res->chain_.push_back(tmp);
        //} else {
         res->chain_.push_back(chain_.at(i)->copy());
        //}
    }

    return res;
}

void RLProcedure::addCommand(RLCommandPrototype* c) {
    chain_.push_back(c);
}

void RLProcedure::setLinePointer(int nline) {
    currentLinePointer_ = nline;
}

void RLProcedure::exec_() {
    for(int i = 0; i < chain_.size(); i++) {
        (RLCommandPrototype*)(chain_.at(i))->exec();
    }
}

RLTypePrototype* RLProcedure::applyUnary(RLOperator oper) {
    if(RLTypePrototype::applyUnary(oper) == NULL) {
        switch(oper) {
            case perform:
                exec_();
            break;
            default:
                // Can't perform not \'perform\' operation for RLProcedure
                std::cout << "ERROR: Can't perform not \'perform\' operation for RLProcedure";
            break;
        }
    }

    return this;
}

RLTypePrototype* RLProcedure::applyBinary(RLOperator oper, RLTypePrototype* val) {
    // No one binary operation declared for RLProcedure.
    std::cout << "No one binary operation declared for RLProcedure.\n";

    switch(oper) {
        case assign:
            if(val->getTypeQualifier()==Procedure) {
                clear_();

                RLProcedure* valp = (RLProcedure*) val;
                for(int i = 0; i < valp->chain_.size(); i++) {
//                    if(valp->chain_.at(i)->first_->getTypeQualifier()==Mark) {
//                        RLMark* tmp = (RLMark*) valp->chain_.at(i)->first_->copy();
//                        val->setOwner(this);
//                        chain_.push_back(tmp);
//                    }
                    chain_.push_back(valp->chain_.at(i)->copy());
                }
            } else {
                // Can't assign RLProcedure with val->getTypeQualifier()
                std::cout << "ERROR: Can't assign RLProcedure with " << RLTypePrototype::typeName(val->getTypeQualifier()) << ".\n";
            }
        break;
        default:
            // Unexpected binary operator for RLProcedure
            std::cout << "ERROR: Unexpected binary operator for RLProcedure.\n";
        break;
    }

    return this;
}

void RLProcedure::print() {
    for(int i = 0; i < chain_.size(); i++)
        chain_.at(i)->print();
}

void RLProcedure::printCommand(int line) {
    if(line < chain_.size())
        chain_.at(line)->print();
}


void RLProcedure::exec() {
    exec_();
}
