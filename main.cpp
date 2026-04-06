#include "main.h"
#include "Tester.h"
#include "Segment.h"
#include "IOManager.h"

using namespace std;

void inputManager(){
    cout << "\nВыберите какой пункт задания вы хотите испытать?\n";
    cout << "1. Сгенерировать массив А из элементов, найти сумму элементов, больших"
            "2 и меньших 20 и кратных 8, их количество и вывести результаты на экран.\n";
    cout << "2: 2. В одномерном массиве, состоящем из n вещественных элементов,"
            "вычислить количество элементов массива, лежащих в диапазоне от А до В.\n";

    int n;
    clearInputBuffer();
    cin >> n;
    switch (n)
    {
        case 1:
            firstMethod();
            break;
        case 2:
            secondMethod();
            break;
        default:
            //errorMessage();
            break;
    }
    return;
}

void firstMethod(){

}

void secondMethod(){

}


int main() {
    Segment* segment1 = new Segment();
    /*

    void* p1 = nullptr;
    void* p2 = nullptr;
    void* p3 = nullptr;
    void* p4 = nullptr;
    cout << "\n\n";
    Segment::printSegments();
    //segment1->NewPointer(p1, sizeof(int) * 1000);
    //segment1->WritePointer(p1, 1);
    //cout << segment1->ReadPointer<int>(p1);
    segment1->NewPointer(p2, sizeof(int));
    segment1->WritePointer(p2, 5);
    Segment::printSegments();
    segment1->FreePointer(p2);
    //cout << segment1->ReadPointer<int>(p);
    Segment::printSegments();
    */
    cout << "Лабораторная работа №3\nВариант 12\nКудлай Никита\nВПР 21\n";
    cout << errata;
    
    
    delete segment1;
    return 0;
}