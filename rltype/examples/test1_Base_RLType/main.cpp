#include <iostream>
#include "RLType.h"

int main() {
	RLIdentifierRegister* iregister = new RLIdentifierRegister();
    //RLType::setRegister(iregister);

	RLNumber numb1(10,10);
	RLBool bool1(true,13);

	iregister->get(10)->applyUnary(decrement);
	iregister->get(10)->applyUnary(decrement);
	std::cout << numb1.getValue() << std::endl;
    iregister->get(10)->applyBinary(assign,&RLNumber(15));

	std::cout << bool1.getValue() << std::endl;
	std::cout << numb1.getValue() << std::endl;

	delete iregister;
}
