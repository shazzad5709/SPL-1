#pragma once

#include <iostream>
using namespace std;

enum Encapsulation {
    PRIVATE, PROTECTED, PUBLIC
};

Encapsulation getEncapsulation (char c)
{
    if(c=='-')
        return PRIVATE;
    else if(c=='#')
        return PROTECTED;
    else if(c=='+')
        return PUBLIC;
    else
        throw runtime_error("Encapsulation could not be deduced!");
}

ostream& operator<<(ostream& os, Encapsulation encap)
{
    if(encap==PRIVATE)
        return os<<"private:";
    else if(encap==PROTECTED)
        return os<<"protected:";
    else if(encap==PUBLIC)
        return os<<"public:";
    else
        throw runtime_error("Encapsulation could not be deduced!");
}