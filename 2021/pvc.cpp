#include <iostream>

class A {
public:
    explicit A() {
        foo();
    }

    void foo() {
        foo2();
    }

    virtual void foo2() = 0;
};

class B: public A {
    void foo2() override {
        std::cout << "hi\n";
    }
};

int main() {
    B b;

    return 0;
}
