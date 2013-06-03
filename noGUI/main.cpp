#include <iostream>
#include <fstream>

#include <RLTokenizer.h>
#include <RLPrecompiler.h>
#include <RLInterpreter.h>

int main(const int argn, const char** arg) {
	if(argn != 2) {
		std::cout << "Usage:\n\t rlprecompiler [RlFILE with code]\n";
		return 1;
	}
	
	std::ofstream tokenOutput("tok.tmp");
	//std::ofstream precpOutput("log.tmp");
	//std::ofstream appliOuptut("appout.tmp");
	
	RLTokenizer::setTokenizerOutput(tokenOutput);
	RLPrecompiler::setPrecompilerOutput(std::cerr);
	RLInterpreter::setApplicationOutput(std::cout);
	
    try {
        RLTokenizer::Tokenize(arg[1]);

        RLPrecompiler::Precompile("tok.tmp");
		
        RLInterpreter::Perform();
    } catch(RLPerformException& exc) {
        RLPrecompiler::getPrecompilerOutput() << "Run-time error: l"
                                              << intToStr(exc.whatLine()) 
                                              << ":"
                                              << exc.what().c_str()
											  << std::endl;
    } catch(RLPrecompiler::Exceptions& exc) {
        for(uint i=0;i<exc.getCollectioner().size();i++) {
            RLPrecompiler::Exception* exception = &exc.getCollectioner()[i];

            RLPrecompiler::getPrecompilerOutput() << "Precompile error: l"
                                                  << exception->whatLine()
                                                  << " : " 
                                                  << exception->what().c_str()
												  << std::endl;
        }
    } catch(RLTokenizer::Exceptions& exc) {
        for(uint i=0;i<exc.getCollectioner().size();i++) {
            RLTokenizer::Exception* exception = &exc.getCollectioner()[i];

            RLPrecompiler::getPrecompilerOutput() << "Unexpected token: l"
                                                  << exception->whatLine()
                                                  << " : " 
                                                  << exception->what().c_str()
												  << std::endl;
        }
    }
};
