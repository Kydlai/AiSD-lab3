#include "Segment.h"

using namespace std;


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

size_t* Segment::ptrAllocate(unsigned int count){
    return Allocate<size_t>(count, ptr_dll_head);
}

void* Segment::dataAllocate(unsigned int count){
    return Allocate<byte>(count, data_dll_head);
}

void Segment::resizeDataSegment(unsigned int new_size){
    //TODO
    return;
}

Segment::Segment() {
    if(!count++){
        if(idCounter)
            ptr_dll_head->clear_dll();
        ptr_segment = (size_t*) malloc(sizeof(size_t) * default_ptr_segment_size);
        ptr_dll_head = new Node(ptr_segment, ptr_segment + sizeof(size_t) * default_ptr_segment_size, EMPTY);
    }
    data_segment_size = default_data_segment_size;
    id = idCounter++;
    data_segment = malloc(data_segment_size);
    data_dll_head = new Node(data_segment, (byte*) data_segment + (data_segment_size - 1), EMPTY);
    segments.push_back(this);
}

Segment::~Segment() {
    --count;
    free(data_segment);
    free(ptr_segment);
    data_dll_head->clear_dll();
    for(int i = 0; i < segments.size(); ++i)
        if(segments[i]->id == this->id){
            segments.erase(segments.begin() + i);
            break;
        }
}

void Segment::NewPointer(void*& p, unsigned int bytes){
    size_t* ptr_ptr = nullptr;
    try{
        ptr_ptr = ptrAllocate(1); // Указатель на ячеку, храняюшую указатель на данные
    } catch (NoEmptySpaceException e) {
        cout << e.what() << " для указателя\n";
        return;
    }
    do{
        int tmpSegmentId = this->id;
        try{
            *ptr_ptr = (size_t) this->dataAllocate(bytes); // Указатель на ячейку с данными
        } catch (NoEmptySpaceException e) {
            tmpSegmentId = (tmpSegmentId + 1) % idCounter;
            if(!changeSegmentAllowed || tmpSegmentId == this->id){
                int a = 0;
                //TODO расширение текущего сегмента
            }
            return;
        }
    } while(changeSegmentAllowed);
    p = ptr_ptr;
    return;
}

void Segment::FreePointer(void* p){
    //TODO
}

void Segment::resetDataSegmentSize(unsigned int newSize){
    if(newSize)
        default_data_segment_size = newSize;
}

unsigned int Segment::getDataSegmentSize(){
    return default_data_segment_size;
}
void Segment::resetPtrSegmentSize(unsigned int newSize){
    if(newSize)
        default_ptr_segment_size = newSize;
}

unsigned int Segment::getPtrSegmentSize(){
    return default_ptr_segment_size;
}
