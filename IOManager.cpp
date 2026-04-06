#include "IOManager.h"


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

template <typename T> T* myin(T* result){
    if(result == nullptr){
        result = new T();
    }
    string s;
    cin >> s;
    if(s == "p") {// print
        Segment::printSegments();
        result = myin(result);
    } else if(s == "s") { // settings
        cout << "Текущие настройки\n";
        cout << "1. default_data_segment_size: " << Segment::getDataSegmentSize() << endl;
        cout << "2. default_ptr_segment_size: " << Segment::getPtrSegmentSize() << endl;
        cout << "Количество сегментов данных: " << Segment::getCount() << endl;
        cout << "Желаете ли вы что-либо изменить? ";
        if(acceptRequest()){
            cout << "Введите номер пункта для изменения\n";
            int* a;
            checkInput(a);
            while(*a < 1 || *a > 2){
                cout << "Введите корректный номер пункта\n";
                checkInput(a);
            }
            unsigned int* size;
            cout << "Введите размер сегмента ";
            switch (*a)
            {
            case 1:
                cout << "данных: ";
                checkInput(size);
                Segment::resetDataSegmentSize(*size);
                break;
            
            case 2:
                cout << "указателей: ";
                checkInput(size);
                Segment::resetPtrSegmentSize(*size);
                break;
            }
        }
        result = myin(result);
    } else if(s == "h") {// help
        cout << errata;
        cout << "Возврат к вводу\n";
        result = myin(result);
    } else if(s == "t"){// test mode
        cout << "Переход к режиму тестирования\n";
        testAllocator();
        cout << "Возврат к основной программе\n";
        result = myin(result);
    } else if(s == "q") {// quit
        throw TesterExitEception();
    } else{
        checkPreInput(result, s);
    }
    return result;
}

template int* myin<int>(int*);
template float* myin<float>(float*);
template unsigned int* myin<unsigned int>(unsigned int*);
template string* myin<string>(string*);