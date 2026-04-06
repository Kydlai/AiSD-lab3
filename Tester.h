#pragma once

#include "Segment.h"
#include "IOManager.h"
#include <vector>
#include <string>

using namespace std;

inline string tester_errata = "1. Создать указатель на данные размером b байт: (new b)\n"
                      "2. Записать значение n (int/float) в указатель p: (write p n)\n"
                      "3. Прочитать значение n с указателя p: (read p)\n"
                      "4. Освободить указатель p: (free p)\n"
                      "5. Скопировать значение *p в *q: (set q p)\n"
                      "6. Создать массив на n элементов по sizeof(int) байт: (arr n)\n"
                      "7. Узнать размер указателя p: (size p)\n"
                      "Все операции с массивом кроме free требуют указания индекса: (write p i n), (read p i), (set q p i)\n"
                      "q для выхода";
                      
inline vector<byte*> ptrs{};
inline vector<bool> isArr{};

void testAllocator();