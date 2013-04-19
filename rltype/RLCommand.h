#include <iostream>

#include <RLType.h>


class RLCommand {
public:
    RLType* exec();

private:
    RLType* first_;
    RLType* second_;

    RLOperator operator_;
};

