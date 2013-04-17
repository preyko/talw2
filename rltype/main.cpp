#include <iostream>
#include "RLType.h"

int main() {
	RLIdentifierRegister* iregister = new RLIdentifierRegister();
	RLType::setRegister(iregister);

	RLNumber bool1(12312312);

	std::cout << bool1.getValue();

	delete iregister;
}
