#include <iostream>
#include "RLType.h"
#include "RLCommand.h"

int main() {
    RLIdentRegister* iregister = new RLIdentRegister();

    new RLNumber(0,4);
    new RLNumber(2341234,5);

    RLProcedure* proc = new RLProcedure(3);

    proc->addCommand(new RLCommand(assign, new RLDereference(4), new RLCommand(increment,new RLDereference(5))));
    proc->exec();


    RLIdentRegister::get(4)->print();
    RLIdentRegister::get(5)->print();


	delete iregister;
}
