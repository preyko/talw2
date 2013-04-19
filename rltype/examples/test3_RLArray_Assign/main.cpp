#include <iostream>
#include "RLType.h"

int main() {
	RLIdentifierRegister* iregister = new RLIdentifierRegister();
    RLType::setRegister(iregister);

    RLArray array1(RLType::Number);
    RLArray array2(RLType::Number);

    array1.setElem(1,&RLNumber(100));
    array1.setElem(2,&RLNumber(101));

    array2.applyBinary(assign,&array1);

    array1.getElem(1)->applyUnary(increment);

    array1.print();
    array2.print();

	delete iregister;
}
