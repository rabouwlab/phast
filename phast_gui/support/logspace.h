#ifndef LOGSPACE_H
#define LOGSPACE_H

/*
    logspace.h

    Author: "luk32"

    Description:
        The Logspace class is used to generate a logarithmically-
        spaced range.

    Source
        http://stackoverflow.com/questions/21429294/is-there-something-like-numpy-logspace-in-c/21430882#21430882

*/

template<typename T>
class Logspace {
private:
    T curValue, base;

public:
    Logspace(T first, T base) : curValue(first), base(base) {}

    T operator()() {
        T retval = curValue;
        curValue *= base;
        return retval;
    }
};

#endif // LOGSPACE_H
