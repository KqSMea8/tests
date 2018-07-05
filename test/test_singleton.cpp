#include <stdio.h>
#include <iostream>

class B{
public:
    B(){
        member_ = 0;
    }
public:
    int member_;
};

class A{
public:
    A(B* b){
        count_ = 0;
        b_ = b;
    }
public:
    int count_;
    B* b_;
};

A* Create(){
    static A a(new B());
    return &a;
}

A* Create2(){
    static A* a = new A(new B());
    return a;
}


int main(){
    A* a0 = Create();
    A* a1 = Create2();

    std::cout << "a0:" << a0 
        << ", a1:" << a1;
}
