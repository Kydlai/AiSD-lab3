#pragma once

#include "Segment.h"
#include "Tester.h"
#include "Utility.h"
#include <string>
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

inline bool intSegmentOutput{true};
inline string errata =  "В любой момент вы можете ввести одну из этих комманд\n"
                "h\thelp\np\tprint all segments\ns\topen settings\nt\ttest mode\nq\text\n";

void clearInputBuffer();
bool acceptRequest();
bool repeatRequest();

template <typename T> void checkInput(T* result){
    T a;
    cin >> a;
    while(cin.fail()){
        cin.clear();
        clearInputBuffer();
        cout << "Ошибка во входном типе данных, пожалуйста, повторите попытку ввода\n";
        cin >> a;
    }
    *result = a;
}

template <typename T> void checkPreInput(T* result, string s){
    T a;
    istringstream iss(s);
    iss.clear();
    iss >> a;
    cin.clear();
    while(iss.fail() || cin.fail()){
        iss.clear();
        cin.clear();
        clearInputBuffer();
        numeric_limits<streamsize>::max();
        cout << "Ошибка во входном типе данных, пожалуйста, повторите попытку ввода\n";
        cin >> a;
    }
    *result = a;
}

template <typename T> T* myin(T* result);