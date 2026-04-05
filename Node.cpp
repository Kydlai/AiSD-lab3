#include "Node.h"

Node::Node(void* new_start_ptr, void* new_end_ptr, statement new_state){
    start_ptr = new_start_ptr;
    end_ptr = new_end_ptr;                              
    prev = nullptr;
    next = nullptr;
    state = new_state;
    //ptrs = vector<size_t>{};
}


void Node::clear_dll(){
    Node *prev_node = prev, *next_node = next;
    while(prev_node != nullptr){
        Node* tmp_node = prev_node->prev;
        if(prev_node->next != nullptr)
            prev_node->next->prev = nullptr;
        if(prev_node->prev != nullptr)
            prev_node->prev->next = nullptr;
        delete prev_node;
        prev_node = tmp_node;
    }
    delete this;
}

void Node::removeData(void* p, size_t bytes){
    Node* tmp_node = this;

    int8_t* start_data = (int8_t*) p;
    int8_t* end_data = (int8_t*) p + bytes;
    /*
    byte* start_data = (byte*) p;
    byte* end_data = (byte*) p + bytes; // с роя-ли <---------------------------------------------------------------------------------
    */
    
    while(!(start_data >= tmp_node->start_ptr && end_data <= tmp_node->end_ptr)){
        tmp_node = tmp_node->prev;
        if(tmp_node == nullptr)
            throw NodeNotFoundException();
    }
    Node* deleting_node = new Node(start_data, end_data, EMPTY);
    Node* new_last_node = new Node(end_data, tmp_node->end_ptr, tmp_node->state);
    tmp_node->next->prev = new_last_node;
    new_last_node->next = tmp_node->next;
    deleting_node->next = new_last_node;
    new_last_node->prev = deleting_node;
    deleting_node->prev = tmp_node;
    tmp_node->next = deleting_node;
    Node::nodeCollapse(deleting_node);
}

void Node::nodeCollapse(Node* node){
    if(node->start_ptr == node->end_ptr){
        if(node->prev != nullptr)
            node->prev->next = node->next;
        if(node->next != nullptr)
            node->next->prev = node->prev;
        if(node->prev != nullptr && node->next != nullptr)
            nodeConnect(node->prev, node->next);
        delete node;
    }
}

void Node::nodeConnect(Node* node1, Node* node2){
    if(node1->state == node2->state){
        if(node1->start_ptr > node2->start_ptr)
            swap(node1, node2);
        if(node1->end_ptr == node2->start_ptr){
            node1->end_ptr = node2->end_ptr;
            node1->next = node2->next;
            node2->next->prev = node1;
            delete node2;
        }
    }
}