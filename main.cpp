#include "main.h"
#include "Segment.h"

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
    Segment* segment1 = new Segment();

    void* p = nullptr;
    void* d = nullptr;
    segment1->NewPointer(p, sizeof(int));
    segment1->WritePointer(p, 4);
    cout << segment1->ReadPointer<int>(p);
    segment1->NewPointer(d, sizeof(int));
    segment1->WritePointer(d, 8);
    cout << segment1->ReadPointer<int>(d);
    cout << segment1->ReadPointer<int>(p);
    p = nullptr;
    segment1->NewPointer(p, sizeof(int));
    cout << segment1->ReadPointer<int>(p);

    delete segment1;
    return 0;
}