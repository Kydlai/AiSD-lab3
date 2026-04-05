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


    void* p1 = nullptr;
    void* p2 = nullptr;
    void* p3 = nullptr;
    void* p4 = nullptr;
    cout << "\n\n";
    segment1->NewPointer(p1, sizeof(int));
    segment1->WritePointer(p1, 1);
    //cout << segment1->ReadPointer<int>(p1);
    segment1->NewPointer(p2, sizeof(int));
    segment1->WritePointer(p2, 5);
    //cout << segment1->ReadPointer<int>(p);
    Segment::printSegments();

    delete segment1;
    return 0;
}