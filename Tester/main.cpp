#include <iostream>

class MyClass
{
public:
    virtual int getValue() { return value1_; }

    virtual int getValue2() { return value2_; }

private:
    int value1_{-171};
    int value2_{ -1237 };
};

int main(int argc, char** argv)
{
    std::cout << "Hello world" << std::endl;
    MyClass c1;
    std::cout << c1.getValue() << std::endl;
}