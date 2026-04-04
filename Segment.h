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
    static inline size_t* ptr_segment{nullptr};
    static inline Node* ptr_dll_head{nullptr};

    Node* data_dll_head;
    void* data_segment;
    unsigned int data_segment_size;
    bool changeSegmentAllowed = false;

    template <typename T>
    static inline T* Allocate(unsigned int count, Node* dll_head){
        Node* tmp_node = dll_head; // голова всегда последний блок
        if(tmp_node->next != nullptr)
            tmp_node = tmp_node->next; // переход на начальный блок
        if(tmp_node->state == RESERVED){
            tmp_node = tmp_node->next; // копия послеследующей строки
            while (tmp_node->state == RESERVED || (T*) tmp_node->end_ptr - count < (T*) tmp_node->start_ptr){
                tmp_node = tmp_node->next;
                if(tmp_node == nullptr)
                    throw NoEmptySpaceException();
            }
        }
        
        int shift = 0; // смещение на случай добавления в существующий блок

        if(tmp_node->prev == nullptr){
            Node* new_node = new Node(tmp_node->start_ptr, (T*) (tmp_node->start_ptr) + count, RESERVED);
            new_node->next = tmp_node;
            tmp_node->prev = new_node;
        }
        else{
            tmp_node->prev->end_ptr = (T*) tmp_node->prev->end_ptr + count;
        }
        tmp_node->start_ptr = (T*) tmp_node->start_ptr + count;
        if(tmp_node->next == nullptr)
            tmp_node->next = tmp_node->prev;
        //TODO проверка удаления tmp_node
        return (T*) (tmp_node->prev->end_ptr) - count;
    }

    static size_t* ptrAllocate(unsigned int count);
    void* dataAllocate(unsigned int count);
    void resizeDataSegment(unsigned int new_size);

public:
    unsigned int id;

    Segment();
    ~Segment();
    void NewPointer(void*& p, unsigned int bytes);

    

    template <typename T>
    void WritePointer(void* p, T data){
        if(p != nullptr){
            size_t* ptr = (size_t*) p;
            if(ptr != nullptr)
                *((T*) *ptr) = data;
            else throw NullPtrException();
        } else throw NullPtrException();
        return;
    }

    template <typename T>
    T ReadPointer(void* p){
        if(p != nullptr){
            size_t* ptr = (size_t*)p;
            if(ptr != nullptr)
                return *((T*) *ptr);
            else
                throw NullPtrException();
        } else throw NullPtrException();
    }

    template <typename T>
    void SetPointer(T* p, T* b){
        WritePointer<T>(p, ReadPointer<T>(b));
    }

    void FreePointer();
    void resetDataSegmentSize(unsigned int newSize);
    unsigned int getDataSegmentSize();
    void resetPtrSegmentSize(unsigned int newSize);
    unsigned int getPtrSegmentSize();
};

