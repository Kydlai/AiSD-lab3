#pragma once

#include <string>
#include <iostream>

using namespace std;

string errata =  "В любой момент вы можете ввести одну из этих комманд\n\\
                \\h\thelp\np\tprint all segments\ns\topen settings\n";

template <typename T> void checkInput(T* result);
template <typename T> void myin(T* result);
