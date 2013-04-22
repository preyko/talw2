#include <iostream>
#include "RLType.h"

int main() {
	RLIdentifierRegister* iregister = new RLIdentifierRegister();
    RLType::setRegister(iregister);

	RLNumber numb1(10,10);
	RLBool bool1(true,13);

    RLType* tmp = iregister->get(10);
    tmp->applyUnary(decrement);
    tmp->applyUnary(decrement);
	std::cout << numb1.getValue() << std::endl;
    tmp->applyBinary(assign,&RLNumber(15));

	std::cout << bool1.getValue() << std::endl;
	std::cout << numb1.getValue() << std::endl;

	delete iregister;
}
