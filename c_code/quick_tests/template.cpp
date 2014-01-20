#include <iostream>

using namespace std;

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


int main(){
    int x=6;
    int y=3;

    calc myCalc;
    cout << "x=" << x << " y=" << y << endl << "x + y = " << myCalc.add(x,y) << endl << "x * y = " << myCalc.mult(x,y) << endl;



    return 0;
}
