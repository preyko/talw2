#include "RLType.h"
#include "RLTools.h"
#include "RLCommand.h"
#include "RLInterpreter.h"

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
        throw RLTypeException(std::string("ID collision. Id ") +
                                 intToStr(id) +
                                 std::string(" exist."));

		return false;
	}
}

RLTypePrototype* RLIdentRegister::get(int id) {
    IdentifierRegister::iterator it = register_.find(id);
    if(it != register_.end())
        return it->second;
    else
        return NULL;
}

void RLIdentRegister::clear() {
    register_.clear();
}

IdentifierRegister RLIdentRegister::register_ = IdentifierRegister();


/*
 * class RLType
 */
RLTypePrototype::LinkCounter RLTypePrototype::linkCounter_ = RLTypePrototype::LinkCounter();

void RLTypePrototype::killThemAll() {
    for(int i = 0; i < linkCounter_.size(); i++) {
        delete linkCounter_[i];
    }

    linkCounter_.clear();
    RLIdentRegister::clear();
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

RLTypePrototype::RLTypePrototype() {
    linkCounter_.push_back(this);
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

            return this;
        break;
        default:
        break;
    }

    return NULL;
}

RLTypePrototype* RLTypePrototype::applyBinary(RLOperator, RLTypePrototype*) {
	// ERROR! Can't apply any operator for base RLType
    throw RLTypeException(std::string("Can't apply any operator for base RLType."));

    return new RLBool(false);
}

void RLTypePrototype::print() {
    RLInterpreter::getApplicationOutput() << "RLType = Base." << std::endl;
}

RLTypePrototype::RLTypeQualifier RLTypePrototype::getTypeQualifier() const {
    return meta_.typeName;
}


/*
 * class RLBool : public RLType
 */
RLBool::RLBool(bool val) : RLTypePrototype() {
	init_(val);
}
RLBool::RLBool(bool val, int id) : RLTypePrototype() {
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
            throw RLTypeException(std::string("Can't compare RLBool with ") +
                                     RLTypePrototype::typeName(val->getTypeQualifier()));
		}
	break;
	case assign:
		if(val->getTypeQualifier()==Bool) {
			RLBool* valb = (RLBool*)val;
            setValue(valb->getValue());
		} else {
			// Can't assign RLBool with val->getTypeQualifier()
            throw RLTypeException(std::string("Can't assign RLBool with ") +
                                     RLTypePrototype::typeName(val->getTypeQualifier()));
		}
	break;
	default:
        // Unexpected binary operator for RLBool
        throw RLTypeException(std::string("Unexpected binary operator for RLBool."));
	break;
    }

    return this;
}

void RLBool::print() {
    RLInterpreter::getApplicationOutput() << "RLType = Bool, Value = " << value_ << std::endl;
}


/*
 * class RLNumber : public RLType
 */
RLNumber::RLNumber(int val) : RLTypePrototype() {
	init_(val);
}
RLNumber::RLNumber(int val, int id) : RLTypePrototype() {
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
            throw RLTypeException(std::string("Unexpected unary operator for RLNumb."));
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
            throw RLTypeException(std::string("Can't compare RLNumb with ") +
                                     RLTypePrototype::typeName(val->getTypeQualifier()));

            return new RLBool(false);
		}
	break;
	case assign:
		if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            setValue(valn->getValue());
		} else {
			// Can't assign RLNumb with val->getTypeQualifier()
            throw RLTypeException(std::string("Can't assign RLNumb with ") +
                                     RLTypePrototype::typeName(val->getTypeQualifier()));
		}
	break;
	default:
        // Unexpected binary operator for RLBool
        throw RLTypeException(std::string("Unexpected binary operator for RLBool."));
	break;
    }

    return this;
}

void RLNumber::print() {
    RLInterpreter::getApplicationOutput() << "RLType = Number, value = " << value_ << std::endl;
}


/*
 * class RLArray : public RLType
 */
RLArray::RLArray(RLTypeQualifier qualifier) : RLTypePrototype() {
    init_(qualifier);
}
RLArray::RLArray(RLTypeQualifier qualifier, int id) : RLTypePrototype() {
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
        throw RLTypeException(std::string("No such element in RLArray with index ") +
                                 intToStr(pos));

        return NULL;
    } else {
        return elements_.find(pos)->second;
    }
}

void RLArray::setElem(int pos, RLTypePrototype* element) {
    if(element->getTypeQualifier()!=getElemQualifier()) {
        // Can't put different RLType in this Array
        throw RLTypeException(std::string("Can\'t put ") +
                                 RLTypePrototype::typeName(element->getTypeQualifier()) +
                                 std::string(" type in Array of ") +
                                 RLTypePrototype::typeName(getElemQualifier()));

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
                throw RLTypeException(std::string("Can't compare RLArray with ") +
                                         RLTypePrototype::typeName(val->getTypeQualifier()));

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
                throw RLTypeException(std::string("Can't assign RLArray with ") +
                                         RLTypePrototype::typeName(val->getTypeQualifier()));
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
                throw RLTypeException(std::string("Only RLNumber can be used in[]."));
            }
        default:
            // Unexpected binary operator for RLArray
            throw RLTypeException(std::string("Unexpected binary operator for RLArray."));
        break;
    }

    return this;
}

void RLArray::print() {
    RLInterpreter::getApplicationOutput() << "RLType = Array of " << RLTypePrototype::typeName(getElemQualifier()) << ".\n\tValues\n";

    RLArrayStorage::const_iterator i;
    for(i = elements_.begin(); i != elements_.end(); i++) {
        RLInterpreter::getApplicationOutput() << "[" << i->first << "] ";

        i->second->print();
    }
}


/*
 * class RLLabel : public RLType
 */
RLMark::RLMark(RLTypePrototype* owner, int line) : RLTypePrototype() {
    init_(owner,line);
}

RLMark::RLMark(RLTypePrototype* owner, int line, int id) : RLTypePrototype() {
    init_(owner,line);
    reg(id);
}

void RLMark::init_(RLTypePrototype* owner, int line) {
    meta_.typeName = Mark;

    RLInterpreter::getApplicationOutput() << "RLMark in development, it will come later.\n";

    if(owner->getTypeQualifier() != Procedure) {
        // Only RLProcedure can be owner of RLMark.
        throw RLTypeException(std::string("Only RLProcedure can be owner of RLMark."));
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
                    throw RLTypeException(std::string("RLMark transaction cannot be performed because procedure Owner is not declared."));
                }
            break;
            default:
                // Can't perform not \'makeTransition\' operation for RLMark
                throw RLTypeException(std::string("Can't perform not \'makeTransition\' operation for RLMark"));
            break;
        }
    }

    return new RLBool(false);
}

RLTypePrototype* RLMark::applyBinary(RLOperator oper, RLTypePrototype* val) {
    // No one binary operation declared for RLMark.
    throw RLTypeException(std::string("No one binary operation declared for RLMark.\n"));

    return this;
}

void RLMark::print() {
    if(owner_!=NULL) {
        RLInterpreter::getApplicationOutput() << "RLMark for RLProcedure on line " << linePointer_ << "\n\t";
        owner_->printCommand(linePointer_);
    }
}

/*
 * class RLProcedure : public RLType
 */
RLProcedure::RLProcedure() : RLTypePrototype() {
    init_();
}

RLProcedure::RLProcedure(int id) : RLTypePrototype() {
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
        try {
            (RLCommandPrototype*)(chain_.at(i))->exec();
        } catch(RLTypeException exc) {
            throw RLPerformException(exc.what(),chain_.at(i)->getLinePosition());
        }
    }
}

RLTypePrototype* RLProcedure::applyUnary(RLOperator oper) {
    if(RLTypePrototype::applyUnary(oper) == NULL) {
        switch(oper) {
            case perform:
                exec();
            break;
            default:
                // Can't perform not \'perform\' operation for RLProcedure
                throw RLTypeException(std::string("Can't perform not \'perform\' operation for RLProcedure"));
            break;
        }
    }

    return this;
}

RLTypePrototype* RLProcedure::applyBinary(RLOperator oper, RLTypePrototype* val) {
    // No one binary operation declared for RLProcedure.
    throw RLTypeException(std::string("No one binary operation declared for RLProcedure."));

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
                throw RLTypeException(std::string("Can't assign RLProcedure with ") +
                                         RLTypePrototype::typeName(val->getTypeQualifier()));
            }
        break;
        default:
            // Unexpected binary operator for RLProcedure
            throw RLTypeException(std::string("Unexpected binary operator for RLProcedure."));
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
