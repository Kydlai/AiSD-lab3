#include "Segment.h"

using namespace std;

enum statement
{
    EMPTY,
    RESERVED
};

class Node {
public:
    unsigned int* start_ptr;
    unsigned int* end_ptr;
    Node *prev;
    Node *next;
    statement state;
    vector<size_t> ptrs;

    Node(unsigned int* new_start_ptr, unsigned int* new_end_ptr, statement new_state){
        start_ptr = new_start_ptr;
        end_ptr = new_end_ptr;                              
        prev = nullptr;
        next = nullptr;
        state = new_state;
        ptrs = vector<size_t>{};
    }

    void clear_dll(){
        Node *prev_node = prev, *next_node = next;
        while(prev_node != nullptr){
            Node* tmp_node = prev_node;
            free(prev_node);
            prev_node = tmp_node;
        }

        while(next_node != nullptr){
            Node* tmp_node = next_node;
            free(next_node);
            next_node = tmp_node;
        }

        free(this);
    }
};

class Segment {
private:
    static inline unsigned int count{0};
    static inline unsigned int default_data_segment_size{1024};
    static inline unsigned int default_ptr_segment_size{128};
    static inline unsigned int idCounter{0};
    static inline vector<Segment*> segments{};

    Node *dll_head;

    unsigned int* data_segment;
    size_t* ptr_segment;
    unsigned int length;

public:
    unsigned int id;

    Segment() {
        ++count;
        id = idCounter++;
        data_segment = (unsigned int*) malloc(sizeof(int) * default_data_segment_size);
        ptr_segment = (size_t*) malloc(sizeof(size_t) * default_ptr_segment_size);
        length = sizeof(int) * default_data_segment_size;
        dll_head = new Node(data_segment, data_segment + length - 1, EMPTY);
        segments.push_back(this);
    }

    ~Segment() {
        --count;
        free(data_segment);
        free(ptr_segment);
        dll_head->clear_dll();
        for(int i = 0; i < segments.size(); ++i)
            if(segments[i]->id == this->id){
                segments.erase(segments.begin() + i);
                break;
            }
    }

    int* NewPointer(){

    }

    void resetDataSegmentSize(unsigned int newSize){
        if(newSize)
            default_data_segment_size = newSize;
    }

    unsigned int getDataSegmentSize(){
        return default_data_segment_size;
    }

    void resetPtrSegmentSize(unsigned int newSize){
        if(newSize)
            default_ptr_segment_size = newSize;
    }

    unsigned int getPtrSegmentSize(){
        return default_ptr_segment_size;
    }


};