#pragma once

#include "main.h"
#include "Node.h"
#include "Utility.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

class Segment{
private:
    static inline unsigned int count{0}; // общее количество активных сегментов
    static inline unsigned int default_data_segment_size{1024};
    static inline unsigned int default_ptr_segment_size{128};
    static inline unsigned int idCounter{0}; // Счетчик, используемый для генерации id
    static inline vector<Segment*> segments{};
    static inline size_t* ptr_segment{nullptr};
    static inline Node* ptr_dll_head{nullptr};

    Node* data_dll_head;
    void* data_segment;
    unsigned int data_segment_size;
    bool changeSegmentAllowed = false;

    template <typename T> static inline T* Allocate(unsigned int count, Node* dll_head);
    static size_t* ptrAllocate(unsigned int count);
    void* dataAllocate(unsigned int count);
    void resizeDataSegment(unsigned int new_size);

public:
    unsigned int id;

    Segment();
    ~Segment();
    void NewPointer(void*& p, unsigned int bytes);
    template <typename T> void SetPointer(T* p, T* b);
    void FreePointer(void* p);
    static void printSegments();
    void printSegment(string label, Node* dll_head, int num);
    void resetDataSegmentSize(unsigned int newSize);
    unsigned int getDataSegmentSize();
    void resetPtrSegmentSize(unsigned int newSize);
    unsigned int getPtrSegmentSize();

    template <typename T> void WritePointer(void* p, T data){ // <--------------------------------------------
    if(p != nullptr){
        size_t* ptr = (size_t*) p;
        if(ptr != nullptr)
            *((T*) *ptr) = data;
        else throw NullPtrException();
    } else throw NullPtrException(); 
    return;
    }

    template <typename T> T ReadPointer(void* p){ // <--------------------------------------------------------
        if(p != nullptr){
            size_t* ptr = (size_t*)p;
            if(ptr != nullptr)
                return *((T*) *ptr);
            else throw NullPtrException();
        } else throw NullPtrException();
    }
};

