#pragma once

#include "Utility.h"
#include <inttypes.h>

using namespace std;

class Node{
public:
    void* start_ptr;
    void* end_ptr;
    Node *prev;
    Node *next;
    statement state;

    static inline void nodeCollapse(Node* node, Node* dll_head);
    static inline void nodeConnect(Node* node1, Node* node2);
    void removeData(void* p, size_t bytes);
    Node(void* new_start_ptr, void* new_end_ptr, statement new_state);
    void clear_dll();
};