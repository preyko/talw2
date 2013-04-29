#include <iostream>
#include "RLType.h"
#include "RLCommand.h"
#include "RLInterpreter.h"

int main() {
    RLInterpreter::Initialization();

    new RLNumber(10,1);
    new RLNumber(0,2);

    RLProcedure* up0 = new RLProcedure();
    RLInterpreter::addCommand(new RLCycle(up0,new RLCommand(compare,new RLDereference(1),new RLDereference(new RLNumber(10)))));
    RLInterpreter::upStack(up0);

    RLInterpreter::addCommand(new RLCommand(increment,new RLDereference(2)));

    RLProcedure* up1 = new RLProcedure();
    RLInterpreter::addCommand(new RLConditional(up1,new RLCommand(compare,new RLDereference(2),new RLDereference(new RLNumber(5)))));
    RLInterpreter::upStack(up1);

    RLInterpreter::addCommand(new RLCommand(assign,new RLDereference(1),new RLDereference(new RLNumber(11))));


    RLInterpreter::Perform();

    RLIdentRegister::get(1)->print();
    RLIdentRegister::get(2)->print();


}
