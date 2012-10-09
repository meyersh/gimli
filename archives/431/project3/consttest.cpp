#include <string>
#include <iostream>

using namespace std;

struct foo {
    foo() {
        cout << "foo constructor" << endl;
    }
};

void f(const foo &obj) {
    return;
}


int main() {
    foo obj;
    f(obj);
}
