#pragma once

#include <stdio.h>
#include <vector>
#include <iostream>

namespace RLTokenizer {
class Exception {
public:
    Exception(std::string descr,int line = -1) {
        description_ = descr;
        linePosition_ = line;
    }

    int whatLine() {
        return linePosition_;
    }

    std::string& what() {
        return description_;
    }

private:
    int linePosition_;
    std::string description_;
};

class Exceptions {
public:
    typedef std::vector<Exception> Collectioner;

    void add(Exception exc) {
        collectionerException_.push_back(exc);
    }

    void clear() {
        collectionerException_.clear();
    }

    bool isEmpty() {
        return collectionerException_.size() == 0;
    }
    
    Exception& getLast() {
        return collectionerException_[collectionerException_.size()-1];
    }

    Collectioner& getCollectioner() {
        return collectionerException_;
    }

private:
    std::vector<Exception> collectionerException_;

};


extern std::ostream* tokenOutput_;

std::ostream& getTokenizerOutput();
void setTokenizerOutput(std::string filename);
void setTokenizerOutput(std::ostream& ostr);

void Tokenize(const char* input);

}
