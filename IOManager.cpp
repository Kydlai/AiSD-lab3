#include "IOManager.h"
#include "Segment.h"
#include "main.h"

template <typename T> void checkInput(T* result){
    T a;
    cin << a;
    while(cin.fail()){
        cin.clear();
        cout << "Ошибка во входном типе данных, пожалуйста, повторите попытку ввода\n"
        cin << a;
    }
    *result = a;
}

template <typename T> void myin(T* result){
    string s;
    cin >> s;
    switch (s)
    {
    case "p": // print
        Segment::printSegments();
        myin(result);
        break;
    
    case "s": // settings
        cout << "Текущие настройки\n";
        cout << "1. default_data_segment_size: " << Segment::getDataSegmentSize() << endl;
        cout << "2. default_ptr_segment_size: " << Segment::getPtrSegmentSize() << endl;
        cout << "Количество сегментов данных: " << Segment::getCount() << endl;
        cout << "Желаете ли вы что-либо изменить? ";
        if(acceptRequest()){
            cout << "Введите номер пункта для изменения\n";
            int a;
            checkInput(a);
            while(a < 1 || a > 2){
                cout << "Введите корректный номер пункта\n"
                checkInput(a);
            }
            unsigned int size;
            cout << "Введите размер сегмента ";
            switch (a)
            {
            case 1:
                cout << "данных: ";
                checkInput(size);
                Segment::resetDataSegmentSize(size);
                break;
            
            case 2:
                cout << "указателей: ";
                checkInput(size);
                Segment::resetPtrSegmentSize(size);
                break;
            }
        }
        myin(result);
        break;

    case "h": // help
        cout << errata;
        cout << "Возврат к вводу\n";
        myin(result);
        break;
    
    default:
        checkInput(result);
        break;
    }
}