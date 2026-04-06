#include "Tester.h"

void testAllocator(){
    cout << tester_errata << endl;
    Segment test_segment = Segment();
    while(true){
        string input = "";
        try{
            myin(&input);
            if(input == "new"){ // new b
                int* b = nullptr;
                b = myin(b);
                void* ptr = nullptr;
                test_segment.NewPointer(ptr, *b);
                ptrs.push_back((byte*) ptr);
                isArr.push_back(false);
                cout << "Создан указатель " << ptrs.size() - 1 << " на данные размером " << *b << " байт\n";
            } else if(input == "write"){ // write p n
                unsigned int* p = nullptr;
                p = myin(p);
                if(*p < ptrs.size() && ptrs[*p] != nullptr){
                    unsigned int it_d = 0;
                    unsigned int* it = &it_d;
                    float* n = nullptr;
                    if(isArr[*p]){
                        it = myin(it);
                        if(*it >= test_segment.getSize(ptrs[*p])  / sizeof(int)){
                            cout << "Обращение по недопустимому смещению\n";
                            clearInputBuffer();
                            continue;
                        }
                    } else {
                        *it = 0;
                    }
                    n = myin(n);
                    test_segment.WritePointer((int*) ptrs[*p], *it, *n);
                    cout << "В ячейку с указателем " << *p << " и смещением " << *it << " записано значение " << *n << endl;
                } else {
                    cout << "Данного указателя не существует\n";
                    clearInputBuffer();
                }
            } else if(input == "read"){ // read p;
                unsigned int* p = nullptr;
                p = myin(p);
                if(*p < ptrs.size() && ptrs[*p] != nullptr){
                    unsigned int it_d = 0;
                    unsigned int* it = &it_d;
                    if(isArr[*p]){
                        it = myin(it);
                    } else {
                        *it = 0;
                    }
                    float b = test_segment.ReadPointer<float>((int*) ptrs[*p], *it);
                    cout << "В ячейке с указателем " << *p << " находтся значение " << b << endl;
                } else {
                    cout << "Данного указателя не существует\n";
                    clearInputBuffer();
                }
            } else if(input == "free") { // free p
                unsigned int* p = nullptr;
                p = myin(p);
                if(*p < ptrs.size() && ptrs[*p] != nullptr){
                    test_segment.FreePointer(ptrs[*p]);
                    isArr[*p] = false;
                    ptrs[*p] = nullptr;
                    cout << "Данные с указателем " << *p << " освобождены" << endl;
                } else {
                    cout << "Данного указателя не существует\n";
                    clearInputBuffer();
                }
            } else if(input == "set"){ // set q p
                unsigned int* p = nullptr;
                p = myin(p);
                if(*p < ptrs.size() && ptrs[*p] != nullptr){
                    if(isArr[*p]){
                        cout << "Операция set не поддерживает работу с массивами в рамках тестера\n";
                        continue;
                    }
                    unsigned int* q = nullptr;
                    q = myin(q);
                    if(*q < ptrs.size() && ptrs[*q] != nullptr){
                        if(isArr[*q]){
                            cout << "Операция set не поддерживает работу с элементами массива в рамках тестера\n";
                        continue;
                        } 
                        test_segment.SetPointer((int*) ptrs[*p], (int*) ptrs[*q]);
                        cout << "В ячейку с указателем " << *p << " записано значение " << test_segment.ReadPointer<float>(ptrs[*q]) << " из ячейки с указателем " << *q << endl;
                    } else {
                        cout << "Данного указателя не существует\n";
                        clearInputBuffer();
                    } 
                } else {
                    cout << "Данного указателя не существует\n";
                    clearInputBuffer();
                }
            } else if(input == "arr"){ //arr n
                unsigned int* n = nullptr;
                n = myin(n);
                void* ptr = nullptr;
                test_segment.NewPointer(ptr, *n * sizeof(int));
                ptrs.push_back((byte*) ptr);
                isArr.push_back(true);
                cout << "Создан указатель на массив " << ptrs.size() - 1 << " на данные размером " << *n * sizeof(int) << " байт\n";
            } else if(input == "size"){
                unsigned int* p = nullptr;
                p = myin(p);
                if(*p < ptrs.size() && ptrs[*p] != nullptr){
                    cout << "Размер ячейки данных с указателем"  << *p << " равен " << test_segment.getSize(ptrs[*p]) << " байт\n";
                } else {
                    cout << "Данного указателя не существует\n";
                    clearInputBuffer();
                }
            } else {
                cout << "Такой команды не существует\n";
            }

        } catch(TesterExitEception e){
            return;
        }
    }
}