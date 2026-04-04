#pragma once

#include "Utility.h"

using namespace std;

class Node{
public:
    void* start_ptr;
    void* end_ptr;
    Node *prev;
    Node *next;
    statement state;
    //vector<size_t> ptrs;

    static inline void nodeCollapse(Node* node);
    static inline void nodeConnect(Node* node1, Node* node2);
    Node(void* new_start_ptr, void* new_end_ptr, statement new_state);
    void clear_dll();
};