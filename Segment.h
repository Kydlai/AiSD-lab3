#pragma once

#include "main.h"
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace std;

enum statement
{
    EMPTY,
    RESERVED
};

class NoEmptySpaceException : public exception{
public:
    const char* what() const noexcept override {
        return "В сегменте нет свободного места для выделения памяти";
    }
};

class NullPtrException : public exception{
public:
    const char* what() const noexcept override {
        return "Обращение к nullptr";
    }
};



class Node{
public:
    void* start_ptr;
    void* end_ptr;
    Node *prev;
    Node *next;
    statement state;
    //vector<size_t> ptrs;

    Node(void* new_start_ptr, void* new_end_ptr, statement new_state);
    void clear_dll();
};

class Segment{
private:
    static inline unsigned int count{0}; // общее количество активных сегментов
    static inline unsigned int default_data_segment_size{1024};
    static inline unsigned int default_ptr_segment_size{128};
    static inline unsigned int idCounter{0}; // Счетчик, используемый для генерации id
    static inline vector<Segment*> segments{};
    static size_t* ptr_segment;
    static Node* ptr_dll_head;

    Node* data_dll_head;
    void* data_segment;
    unsigned int data_segment_size;
    bool changeSegmentAllowed = false;

    static size_t* ptrAllocate(unsigned int count);
    void* dataAllocate(unsigned int count);
    void resizeDataSegment(unsigned int new_size);

public:
    unsigned int id;

    Segment();
    ~Segment();
    void NewPointer(void* p, unsigned int bytes);
    template <typename T> void WritePointer(void* p, T data);
    template <typename T> T ReadPointer(void* p);
    template <typename T> void SetPointer(void* p, void* b);
    void FreePointer();
    void resetDataSegmentSize(unsigned int newSize);
    unsigned int getDataSegmentSize();
    void resetPtrSegmentSize(unsigned int newSize);
    unsigned int getPtrSegmentSize();
};

