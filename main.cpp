#include "main.h"

using namespace std;

void clearInputBuffer(){
    cin.sync();  
}

bool acceptRequest() {
    cout << "[Y/N]?\n";
    char c;
    clearInputBuffer();
    cin >> c;
    return c == 'Y' || c == 'y';
}

int main() {


    return;
}