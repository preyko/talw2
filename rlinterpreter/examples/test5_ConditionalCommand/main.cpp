#include <iostream>
#include "RLType.h"
#include "RLCommand.h"

int main() {
    RLIdentRegister* iregister = new RLIdentRegister();

    new RLNumber(0,4);
    new RLNumber(100,5);

    RLProcedure* proc = new RLProcedure(3);
    RLProcedure* efff = new RLProcedure(-1);


    proc->addCommand(new RLConditional(efff,new RLCommand(compare,new RLDereference(5),new RLDereference(new RLNumber(100)))));
    efff->addCommand(new RLCommand(assign, new RLDereference(4), new RLCommand(increment,new RLDereference(5))));
    proc->exec();


    RLIdentRegister::get(4)->print();
    RLIdentRegister::get(5)->print();


	delete iregister;
}
