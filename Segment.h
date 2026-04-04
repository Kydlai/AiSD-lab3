#pragma once

#include "main.h"
#include "Node.h"
#include "Utility.h"
#include <cstdlib>
#include <vector>
#include <iostream>

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
    template <typename T> void WritePointer(void* p, T data);
    template <typename T> T ReadPointer(void* p);
    template <typename T> void SetPointer(T* p, T* b);
    void FreePointer(void* p);
    void resetDataSegmentSize(unsigned int newSize);
    unsigned int getDataSegmentSize();
    void resetPtrSegmentSize(unsigned int newSize);
    unsigned int getPtrSegmentSize();
};

