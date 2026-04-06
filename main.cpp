#include "main.h"
#include "Tester.h"
#include "Segment.h"
#include "IOManager.h"

using namespace std;



void inputManager(){
    cout << "\nВыберите какой пункт задания вы хотите испытать?\n";
    cout << "1. Сгенерировать массив А из элементов, найти сумму элементов, больших"
            "2 и меньших 20 и кратных 8, их количество и вывести результаты на экран.\n";
    cout << "2. В одномерном массиве, состоящем из n вещественных элементов,"
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
    intSegmentOutput = true;
    Segment* segment = new Segment();
    cout << "Введите количество элементов n\n";
    int* n = new int{};
    n = myin(n);
    void* array_ptr = nullptr;
    segment->NewPointer(array_ptr, sizeof(int) * *n);
    int* max_n = new int{};
    cout << "Стандартное максимальное значение 40. Желаете изменить? ";
    if(acceptRequest()){
        cout << "Введите максимально допустимое значение n\n";
        myin(max_n);
    } else {
        max_n = new int{40};
    }
    cout << "Сгенерированные числа: ";
    for(int i = 0; i < *n; ++i){
        int a = generateInt(*max_n);
        cout << a << " ";
        segment->WritePointer(array_ptr, i, a);
    }
    cout << endl << "\nВсе данные помещены в память\n";

    unsigned int count = 0;

    void* result_ptr = nullptr;
    segment->NewPointer(result_ptr, sizeof(int) * *n); // TODO копия массива, перенос массива

    for(int i = 0; i < *n; ++i){
        if(segment->ReadPointer<int>(array_ptr, i) > 2 && segment->ReadPointer<int>(array_ptr, i) < 20 && segment->ReadPointer<int>(array_ptr, i) % 8 == 0)
            segment->SetPointer<int>(result_ptr, count++, array_ptr, i);
        
    }
    cout << "По условию подходит " << count << " элементов\n";
    if(count){
        cout << "Подходящие по условию элементы: ";
        for(int i = 0; i < count; ++i){
            cout << segment->ReadPointer<int>(result_ptr, i) << " ";
        }
        cout << endl;
    }
    while(true)
        try{
            cout << "Введите q чтобы выйти";
            int* a = new int{};
            myin(a);
        } catch(TesterExitEception e){
            delete segment;
        }
}

void secondMethod(){
    intSegmentOutput = false;
    Segment* segment = new Segment();
    int* n = new int{};
    void* array_ptr = nullptr;
    segment->NewPointer(array_ptr, sizeof(float) * *n);
    void* result_ptr = nullptr;
    segment->NewPointer(result_ptr, sizeof(float) * *n);

    cout << "Введите количество элементов n\n";
    n = myin(n);
    cout << "Вводите числа\n";
    for(int i = 0; i < *n; ++i){
        cout << i << ": ";
        float* a = new float{};
        myin(a);
        segment->WritePointer(array_ptr, i, *a);
    }

    cout << "Введите границы диапазона А и В\n";
    cout << "A: ";
    float* A = new float{};
    myin(A);
    cout << "B: ";
    float* B = new float{};
    myin(B);

    int count = 0;
    //Segment::printSegments<float>();
    //segment->printSegment("", segment->data_dll_head, )

    for(int i = 0; i < *n; ++i){
        if(segment->ReadPointer<float>(array_ptr, i) >= *A && segment->ReadPointer<float>(array_ptr, i) < *B){
            segment->SetPointer<float>(result_ptr, count++, array_ptr, i);
        }
    }

    

    cout << "По условию подходит " << count << " элементов\n";
    if(count){
        cout << "Подходящие по условию элементы: ";
        for(int i = 0; i < count; ++i){
            cout << segment->ReadPointer<float>(result_ptr, i) << " ";
        }
        cout << endl;
    }

    

    while(true)
        try{
            cout << "Введите q чтобы выйти";
            int* a = new int{};
            myin(a);
        } catch(TesterExitEception e){
            break;
        }
    //delete segment;
}

int generateInt(size_t max_int){
    return rand() % (max_int + 1);
}


int main() {
    srand(time(nullptr));
    
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
    inputManager();
    while(true){
        if(repeatRequest())
            inputManager();
        else
            break;
    }
    
    
    //delete segment1;
    return 0;
}