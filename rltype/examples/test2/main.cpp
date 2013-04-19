#include <iostream>
#include "RLType.h"

int main() {
	RLIdentifierRegister* iregister = new RLIdentifierRegister();
    RLType::setRegister(iregister);

    std::cout << "Test 1\n";
    RLArray array(RLType::Number);

    array.setElem(1,&RLNumber(100));
    array.setElem(2,&RLNumber(112));
    array.setElem(2,&RLBool(false));

    //array.print();

    RLArray* array2 = array.copy();

    std::cout << array.applyBinary(compare,array2) << std::endl;

    array2->getElem(2)->applyUnary(increment);

    std::cout << array.applyBinary(compare,array2) << std::endl;

    array.print();
    array2->print();

    std::cout << "Test 1\n";
    RLArray arr1(RLType::Number);
    RLArray arr2(RLType::Number);

    arr1.setElem(1,&RLNumber(101));
    arr1.setElem(2,&RLNumber(102));

    arr1.setElem(2,&RLNumber(101));
    arr1.setElem(1,&RLNumber(102));

    std::cout << arr1.applyBinary(compare,&arr2) << std::endl;

	delete iregister;
}
