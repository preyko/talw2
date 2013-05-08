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
RLTypePrototype::LinkCounter RLTypePrototype::tempVars_ = RLTypePrototype::LinkCounter();
RLTypePrototype::LinkCounter RLTypePrototype::constVars_ = RLTypePrototype::LinkCounter();

void RLTypePrototype::clearTempVars() {
    for(int i = 0; i < tempVars_.size(); i++) {
        delete tempVars_[i];
    }
    tempVars_.clear();
}
void RLTypePrototype::clearConstVars() {
    for(int i = 0; i < constVars_.size(); i++) {
        delete constVars_[i];
    }
    constVars_.clear();
}

void RLTypePrototype::killThemAll() {
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
    //linkCounter_.push_back(this);
}

bool RLTypePrototype::reg(int id) {
    RLIdentRegister::add(id,this);
}

RLTypePrototype* RLTypePrototype::copy() const {
    return new RLTypePrototype();
}

RLTypePrototype* RLTypePrototype::markAsTemp() {
    tempVars_.push_back(this);
    return this;
}
RLTypePrototype* RLTypePrototype::markAsConst() {
    constVars_.push_back(this);
    return this;
}

RLTypePrototype* RLTypePrototype::applyUnary(RLOperator oper) {
    if(!RLInterpreter::isPerformingLinkProcedureLocked()) {
        RLInterpreter::lockLinkProcedurePerforming();
        for(int i = 0; i < linkedProcedures_.size(); i++)
            linkedProcedures_[i]->exec();
        RLInterpreter::unlockLinkProcedurePerforming();
    }

    switch(oper) {
        case np:
            // Do nothing.
            return this;
        break;
        case show:
            print();

            return this;
        break;
    }

    return NULL;
}

RLTypePrototype* RLTypePrototype::applyBinary(RLOperator oper, RLTypePrototype* val) {
    if(!RLInterpreter::isPerformingLinkProcedureLocked()) {
        RLInterpreter::lockLinkProcedurePerforming();
        for(int i = 0; i < linkedProcedures_.size(); i++)
            linkedProcedures_[i]->exec();
        RLInterpreter::unlockLinkProcedurePerforming();
    }

    switch(oper) {
        case bind:
            if(val->getTypeQualifier() == Procedure) {
                RLProcedure* valp = (RLProcedure*) val;
                return (new RLBool(linkWithProcedure(valp)))->markAsTemp();
            } else {
                // Identifier can be linked only with a RLPRocedure.
                throw RLTypeException(std::string("Identifier can be linked only with a RLPRocedure."));
            }
        break;
        case unbind:
            if(val->getTypeQualifier() == Procedure) {
                RLProcedure* valp = (RLProcedure*) val;
                return (new RLBool(breakLinkWithProcedure(valp)))->markAsTemp();
            } else {
                // Identifier can be linked only with a RLPRocedure.
                throw RLTypeException(std::string("Identifier can be delinked only with a RLPRocedure."));
            }
        break;
        default:
            return NULL;
        break;
    }
}

void RLTypePrototype::print() {
    RLInterpreter::getApplicationOutput() << "RLType = Base." << std::endl;
}

RLTypePrototype::RLTypeQualifier RLTypePrototype::getTypeQualifier() const {
    return meta_.typeName;
}

bool RLTypePrototype::linkWithProcedure(RLProcedure* proc) {
    if(this->getTypeQualifier()==Procedure && (this == proc || this == proc->referenceTo_))
        return false;
    else
        linkedProcedures_.push_back(proc);

    return true;
}

bool RLTypePrototype::breakLinkWithProcedure(RLProcedure* proc) {
    std::vector<RLProcedure*>::iterator it = std::find(linkedProcedures_.begin(),linkedProcedures_.end(),proc);

    if(!(it==linkedProcedures_.end()))
        linkedProcedures_.erase(it);

    return true;
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
    RLTypePrototype* temp = RLTypePrototype::applyUnary(oper);
    if(temp!=NULL)
        return temp;
}

RLTypePrototype* RLBool::applyBinary(RLOperator oper, RLTypePrototype* val) {
    RLTypePrototype* temp = RLTypePrototype::applyBinary(oper,val);
    if(temp!=NULL)
        return temp;

	switch(oper) {
	case compare:
		if(val->getTypeQualifier()==Bool) {
			RLBool* valb = (RLBool*)val;
            return (new RLBool(getValue() == valb->getValue()))->markAsTemp();
		} else if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            return (new RLBool(getValue() == (bool)valn->getValue()))->markAsTemp();
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
    RLTypePrototype* temp = RLTypePrototype::applyUnary(oper);
    if(temp!=NULL)
        return temp;

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

    return this;
}

RLTypePrototype* RLNumber::applyBinary(RLOperator oper, RLTypePrototype* val) {
    RLTypePrototype* temp = RLTypePrototype::applyBinary(oper,val);
    if(temp!=NULL)
        return temp;

    switch(oper) {
	case compare:
		if(val->getTypeQualifier()==Number) {
			RLNumber* valn = (RLNumber*)val;
            return (new RLBool(getValue() == valn->getValue()))->markAsTemp();
		} else {
			// Can't compare RLNumb with val->getTypeQualifier()
            throw RLTypeException(std::string("Can't compare RLNumb with ") +
                                     RLTypePrototype::typeName(val->getTypeQualifier()));

            return (new RLBool(false))->markAsTemp();
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
        return NULL;
    } else {
        return elements_.find(pos)->second;
    }
}

RLTypePrototype::RLTypeQualifier RLArray::getElemQualifier() const {
    return arrayType_;
}

RLTypePrototype* RLArray::applyUnary(RLOperator oper) {
    RLTypePrototype* temp = RLTypePrototype::applyUnary(oper);
    if(temp!=NULL)
        return temp;

    // Unexpected unary operation for RLArray
    throw(RLTypeException("Unexpected unary operation for RLArray"));
}

RLTypePrototype* RLArray::applyBinary(RLOperator oper, RLTypePrototype *val) {
    RLTypePrototype* temp = RLTypePrototype::applyBinary(oper,val);
    if(temp!=NULL)
        return temp;

    switch(oper) {
        case compare:
            if(val->getTypeQualifier()==Array) {
                RLArray* vala = (RLArray*)val;

                if(vala->getElemQualifier() != getElemQualifier()) {
                    // Different elements types
                    return (new RLBool(false))->markAsTemp();
                }

                if(elements_.size()!=vala->elements_.size()) {
                    // Different arrays size
                    return (new RLBool(false))->markAsTemp();
                }

                RLArrayStorage::const_iterator i1 = elements_.begin();
                RLArrayStorage::const_iterator i2 = vala->elements_.begin();

                for(; i1 != elements_.end(); i1++, i2++) {
                    RLTypePrototype* res = i1->second->applyBinary(compare,i2->second);

                    if(res->getTypeQualifier() == Bool) {
                        RLBool* resb = (RLBool*)res;
                        if(!resb->getValue()) {
                            return (new RLBool(false))->markAsTemp();
                        }
                    }
                }

                return new RLBool(true);
            } else {
                // Can't compare RLArray with val->getTypeQualifier()
                throw RLTypeException(std::string("Can't compare RLArray with ") +
                                         RLTypePrototype::typeName(val->getTypeQualifier()));

                return (new RLBool(false))->markAsTemp();
            }
        break;
        case assign:
            if(val->getTypeQualifier()==Array) {
                RLArray* vala = (RLArray*)val;

                if(vala->getElemQualifier()!= getElemQualifier()) {
                    // Different elements types
                    return (new RLBool(false))->markAsTemp();
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
                } else {
                    // Index valn->getValue() is out of array bounds.
                    //throw RLTypeException(std::string("Index ") +
                    //                      intToStr(valn->getValue()) +
                    //                      std::string("is out of array bounds."));
                }
            } else {
                // Only RLNumbers can be used for access to array elements.
                throw RLTypeException(std::string("Only RLNumbers can be used for access to array elements"));
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
 * class RLMark : public RLType
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

    transactionCount_ = 0;

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
    RLTypePrototype* temp = RLTypePrototype::applyUnary(oper);
    if(temp!=NULL)
        return temp;

    switch(oper) {
        case maketransition:
            if(transactionCount_ <= MaxTransactionCount_) {
                TransactionDescriptor descriptor;
                descriptor.dawnPerformStackTo = owner_;
                descriptor.goToLine = linePointer_;


                RLInterpreter::getApplicationOutput() << "I'am catch this shit!" << std::endl;

                throw descriptor;
            } else {
                // RLMark transaction cannot be performed because it's look like infinite loop.
                throw RLTypeException(std::string("RLMark transaction cannot be performed because it's look like infinite loop."));
            }
        break;
        default:
            // Can't perform not \'makeTransition\' operation for RLMark
            throw RLTypeException(std::string("Can't perform not \'makeTransition\' operation for RLMark"));
        break;
    }

    return this;
}

RLTypePrototype* RLMark::applyBinary(RLOperator oper, RLTypePrototype* val) {
    RLTypePrototype* temp = RLTypePrototype::applyBinary(oper,val);
    if(temp!=NULL)
        return temp;

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

    referenceTo_ = NULL;
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

int RLProcedure::getSize() const {
    return chain_.size()-1;
}

void RLProcedure::exec_() {
    if(referenceTo_ != NULL) {
        referenceTo_->exec();
        return;
    }

    for(int i = 0; i < chain_.size(); i++) {
        try {
            (RLCommandPrototype*)(chain_.at(i))->exec();

            RLTypePrototype::clearTempVars(); // Cleaning temporary variables after execution line of code

        } catch(RLTypeException exc) {
            throw RLPerformException(exc.what(),chain_.at(i)->getLinePosition());

        } catch(RLMark::TransactionDescriptor descr) {
            if(descr.dawnPerformStackTo == this)
                i = descr.goToLine;
            else if(descr.dawnPerformStackTo==RLInterpreter::getMainFunction())
                throw RLPerformException("Can't make transition.",chain_.at(i)->getLinePosition());
            else
                throw descr;

        }
    }
}

RLTypePrototype* RLProcedure::applyUnary(RLOperator oper) {
    RLTypePrototype* temp = RLTypePrototype::applyUnary(oper);
    if(temp!=NULL)
        return temp;

    switch(oper) {
        case perform:
            exec();
        break;
        default:
            // Can't perform not \'perform\' operation for RLProcedure
            throw RLTypeException(std::string("Can't perform not \'perform\' operation for RLProcedure"));
        break;
    }

    return this;
}

RLTypePrototype* RLProcedure::applyBinary(RLOperator oper, RLTypePrototype* val) {
    RLTypePrototype* temp = RLTypePrototype::applyBinary(oper,val);
    if(temp!=NULL)
        return temp;

    switch(oper) {
        case assign:
            if(val->getTypeQualifier()==Procedure) {
                referenceTo_ = (RLProcedure*)(val);
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
    RLInterpreter::getApplicationOutput() << "RLProcedure.";
}

void RLProcedure::printCommand(int line) {
    if(line < chain_.size())
        chain_.at(line)->print();
}


void RLProcedure::exec() {
    exec_();
}
