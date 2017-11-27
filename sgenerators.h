#ifndef SGENERATORS_H
#define SGENERATORS_H

template <typename T>
class SRange
{
    T step;
    T current;
public:
    SRange():step(T(1)),current(T()){}
    SRange(const T& l_limit):step(T(1)),current(l_limit){}
    SRange(const T& l_limit, const T& step):step(step),current(l_limit){}
    T operator()()
    {
        T ret=current;
        current+=step;
        return ret;
    }
};

#endif // SGENERATORS_H
