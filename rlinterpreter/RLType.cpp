#include "RLType.h"


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

void RLIdentRegister::showAll() {
    IdentifierRegister::iterator it;
    for(it = register_.begin(); it != register_.end(); it++) {
        RLInterpreter::getApplicationOutput() << it->first << " ";
        it->second->print();
    }
}

IdentifierRegister RLIdentRegister::register_ = IdentifierRegister();


/*
 * class RLType
 */
RLTypePrototype::LinkCounter RLTypePrototype::tempVars_ = RLTypePrototype::LinkCounter();
RLTypePrototype::LinkCounter RLTypePrototype::constVars_ = RLTypePrototype::LinkCounter();
RLTypePrototype::LinkCounter RLTypePrototype::dynamicVars_ = RLTypePrototype::LinkCounter();

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

void RLTypePrototype::clearDynamicallyAllocatedVars() {
    for(int i = 0; i < dynamicVars_.size(); i++) {
        delete dynamicVars_[i];
    }
    dynamicVars_.clear();
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
    case Procedure:
        return "Procedure";
    break;
    case Mark:
        return "Mark";
    break;
    default:
        return "Unknown";
    break;
    }
}

RLTypePrototype::RLTypePrototype() {
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

RLTypePrototype *RLTypePrototype::markAsDynamicallyAllocated() {
    dynamicVars_.push_back(this);
    return this;
}

RLTypePrototype* RLTypePrototype::applyUnary(RLOperator oper) {
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
    RLInterpreter::getApplicationOutput() << "RLType = Base." << "\n";
}

RLTypePrototype::RLTypeQualifier RLTypePrototype::getTypeQualifier() const {
    return meta_.typeName;
}

bool RLTypePrototype::linkWithProcedure(RLProcedure* proc) {
    if(this->getTypeQualifier()==Procedure && this == proc)
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

void RLTypePrototype::performLinkedProcedures() {
    if(!RLInterpreter::isPerformingLinkProcedureLocked()) {
        RLInterpreter::lockLinkProcedurePerforming();
        for(int i = 0; i < linkedProcedures_.size(); i++)
            linkedProcedures_[i]->exec();
        RLInterpreter::unlockLinkProcedurePerforming();
    }
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
    return (new RLBool(getValue()))->markAsDynamicallyAllocated();
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
    RLInterpreter::getApplicationOutput() << "RLType = Bool, Value = " << value_ << "\n";
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
    return (new RLNumber(getValue()))->markAsDynamicallyAllocated();
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
        // Unexpected binary operator for RLNumb
        throw RLTypeException(std::string("Unexpected binary operator for RLNumb."));
	break;
    }

    return this;
}

void RLNumber::print() {
    RLInterpreter::getApplicationOutput() << "RLType = Number, Value = " << value_ << "\n";
}


/*
 * class RLArray : public RLType
 */
RLArray::RLArray(int depth, RLTypePrototype::RLTypeQualifier rootqualifier) {
    init_(depth,rootqualifier);
}

RLArray::RLArray(int depth, RLTypePrototype::RLTypeQualifier rootqualifier, int id) {
    init_(depth,rootqualifier);
    reg(id);
}

RLArray::~RLArray() {
    clear();
}

void RLArray::init_(int depth, RLTypeQualifier rootqualifier) {
    setRootDepth(depth);
    rootType_ = rootqualifier;
    meta_.typeName = Array;
}

RLTypePrototype* RLArray::copy() const {
    RLArray* temp = new RLArray(getRootDepth()+1,getRootQualifier());

    for(RLArrayStorage::const_iterator i=elements_.begin();i!=elements_.end();i++) {
        temp->elements_.insert(RLArrayStoragePair(i->first,(i->second->copy())));
    }

    return temp->markAsDynamicallyAllocated();
}

void RLArray::setElem(int pos, RLTypePrototype* elem) {
    elements_.insert(RLArrayStoragePair(pos,elem));
}

RLTypePrototype* RLArray::getElem(int pos) const {
    if(elements_.find(pos)==elements_.end()) {
        return NULL;
    } else {
        return elements_.find(pos)->second;
    }
}

RLTypePrototype::RLTypeQualifier RLArray::getRootQualifier() const {
    return rootType_;
}

void RLArray::setRootQualifier(RLTypeQualifier qual) {
    rootType_ = qual;
}

void RLArray::clear() {
    RLArrayStorage::const_iterator i;
    for(i = elements_.begin(); i != elements_.end(); i++) {
        i->second->markAsTemp();
    }
    elements_.clear();
}

int RLArray::getRootDepth() const {
    return currentRootDepth_;
}

void RLArray::setRootDepth(int depth) {
    currentRootDepth_ = depth-1;
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

                if(vala->getRootQualifier() != getRootQualifier()) {
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

                return (new RLBool(true))->markAsTemp();
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
                if(currentRootDepth_ == vala->currentRootDepth_ && rootType_ == vala->rootType_) {
                    RLArrayStorage::const_iterator i;
                    
                    clear();
    
                    for(i = vala->elements_.begin(); i != vala->elements_.end(); i++) {
                        elements_.insert(RLArrayStoragePair(i->first,(i->second->copy())));
                    }
                } else {
                    // Can't assign RLArray with different dimension and root types.
                    throw RLTypeException(std::string("Can't assign RLArray with different dimension and root types."));
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
                
                if(res==NULL) {
                    if(currentRootDepth_ > 1) {
                        RLArray* array = new RLArray(currentRootDepth_,rootType_);
                        setElem(valn->getValue(),array->markAsDynamicallyAllocated());
                        return array;
                    } else if(currentRootDepth_ == 1) {
                        RLTypePrototype* new_element;
                        
                        if(getRootQualifier() == Number)
                            new_element = new RLNumber(0);
                        else if(getRootQualifier() == Bool)
                            new_element = new RLBool(false);
                        else if(getRootQualifier() == Procedure)
                            new_element = new RLProcedure();
                        
                        setElem(valn->getValue(),new_element->markAsDynamicallyAllocated());
                        return new_element;
                    } else {
                        // Trying to get value in not existing  dimension of RLArray
                        throw RLTypeException(std::string("Trying to get value in not existing  dimension of RLArray"));
                    }
                }

                return res;
            } else {
                // Only RLNumbers can be used for access to array elements.
                throw RLTypeException(std::string("Only RLNumbers can be used for access to array elements"));
            }
        break;
        default:
            // Unexpected binary operator for RLArray
            throw RLTypeException(std::string("Unexpected binary operator for RLArray."));
        break;
    }

    return this;
}

void RLArray::print() {
    print_("");
}

void RLArray::print_(std::string delimetr) {
    RLInterpreter::getApplicationOutput() << "RLType = Array of " << RLTypePrototype::typeName(getRootQualifier()) << ". Values:\n";

    RLArrayStorage::const_iterator i;
    for(i = elements_.begin(); i != elements_.end(); i++) {
        RLInterpreter::getApplicationOutput() << delimetr;
        RLInterpreter::getApplicationOutput() << "[" << i->first << "] ";

        if(i->second->getTypeQualifier() == Array)
            ((RLArray*)i->second)->print_(delimetr+"\t");
        else {
            i->second->print();
        }
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
    return (new RLMark(owner_,linePointer_))->markAsDynamicallyAllocated();
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

                RLInterpreter::getApplicationOutput() << "I'am catch this shit!" << "\n";

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
    throw RLTypeException(std::string("No one binary operation declared for RLMark."));

    return this;
}

void RLMark::print() {
    if(owner_!=NULL) {
        RLInterpreter::getApplicationOutput() << "RLMark.\n";
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

    chain_ = new RLChainCommands();
}

RLProcedure::~RLProcedure() {
    delete chain_;
}

RLTypePrototype* RLProcedure::copy() const {
    RLProcedure* res = new RLProcedure();
    delete res->chain_;
    res->chain_ = (RLChainCommands*)chain_->copy();
    return res->markAsDynamicallyAllocated();
}

void RLProcedure::addCommand(RLCommandPrototype* c) {
    chain_->addCommand(c);
}

void RLProcedure::setLinePointer(int nline) {
    currentLinePointer_ = nline;
}

int RLProcedure::getSize() const {
    return chain_->getChainSize();
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
                RLProcedure* valp = (RLProcedure*)val;
                delete chain_;
                chain_ = (RLChainCommands*)valp->chain_->copy();
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
    RLInterpreter::getApplicationOutput() << "RLProcedure.\n";
}

void RLProcedure::exec() {
    chain_->exec();
}
