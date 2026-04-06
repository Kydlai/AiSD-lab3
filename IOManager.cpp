#include "IOManager.h"



void clearInputBuffer(){
    cin.sync();  
}

bool acceptRequest() {
    cout << "[Y/N]?\n";
    string* c = new string;
    clearInputBuffer();
    myin<string>(c);
    return *c == "Y" || *c == "y";
}

bool repeatRequest(){
    cout << "Желаете попробовать еще раз? ";
    return acceptRequest();
}

template <typename T> T* myin(T* result){
    if(result == nullptr){
        result = new T();
    }
    string s;
    cin >> s;
    if(s == "p") {// print
        if(intSegmentOutput)
            Segment::printSegments<int>();
        else
            Segment::printSegments<float>();
        result = myin(result);
    } else if(s == "s") { // settings
        cout << "Текущие настройки\n";
        cout << "1. default_data_segment_size: " << Segment::getDataSegmentSize() << endl;
        cout << "2. default_ptr_segment_size: " << Segment::getPtrSegmentSize() << endl;
        cout << "3. Выводимый тип данных: " << (intSegmentOutput ? "int" : "float") << endl;
        cout << "Количество сегментов данных: " << Segment::getCount() << endl;
        cout << "Желаете ли вы что-либо изменить? ";
        if(acceptRequest()){
            cout << "Введите номер пункта для изменения\n";
            int* a = new int{};
            checkInput(a);
            while(*a < 1 || *a > 3){
                cout << "Введите корректный номер пункта\n";
                checkInput(a);
            }
            unsigned int* size = new unsigned int{};
            switch (*a)
            {
            case 1:
                cout << "Введите размер сегмента "; 
                cout << "данных: ";
                checkInput(size);
                Segment::resetDataSegmentSize(*size);
                break;
            
            case 2:
                cout << "Введите размер сегмента ";
                cout << "указателей: ";
                checkInput(size);
                Segment::resetPtrSegmentSize(*size);
                break;
            case 3:
                cout << "Желаете изменить выводимый тип данных? ";
                if(acceptRequest())
                    intSegmentOutput = ! intSegmentOutput;
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
        checkPreInput<T>(result, s);
    }
    return result;
}

template int* myin<int>(int*);
template float* myin<float>(float*);
template unsigned int* myin<unsigned int>(unsigned int*);
template string* myin<string>(string*);