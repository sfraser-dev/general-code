// http://www.cprogramming.com/tutorial/templates.html

#include <iostream>

// Not best practice to use namespaces, should use std::cout and std::endl instead.
using namespace std;

// Simple "concrete" class
class calc {
    public:
        int mult(int x, int y);
        int add(int x, int y);
};
int calc::mult(int x, int y){
    return x*y;
}
int calc::add(int x, int y){
    return x+y;
}

// Template class (based on the simple concrete class)
template <class A_Type> class tcalc {
    public:
        A_Type mult(A_Type x, A_Type y);
        A_Type add (A_Type x, A_Type y);
};
template <class A_Type> A_Type tcalc<A_Type>::mult(A_Type x, A_Type y){
    return x*y;
}
template <class A_Type> A_Type tcalc<A_Type>::add(A_Type x, A_Type y){
    return x+y;
}

int main(){
    int x=6;
    int y=3;
    float a=2.5;
    float b=3;

    calc myCalc;
    cout << "x=" << x << " y=" << y << endl << "x + y = " << myCalc.add(x,y) << endl << "x * y = " << myCalc.mult(x,y) << endl;

    tcalc<float>  myCalcFlt;
    cout << "a=" << a << " b=" << b << endl << "a + b = " << myCalcFlt.add(a,b) << endl << "a * b = " << myCalcFlt.mult(a,b) << endl;

    return 0;
}
