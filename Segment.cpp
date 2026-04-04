#include "Segment.h"

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

class Node {
public:
    void* start_ptr;
    void* end_ptr;
    Node *prev;
    Node *next;
    statement state;
    //vector<size_t> ptrs;

    Node(void* new_start_ptr, void* new_end_ptr, statement new_state){
        start_ptr = new_start_ptr;
        end_ptr = new_end_ptr;                              
        prev = nullptr;
        next = nullptr;
        state = new_state;
        //ptrs = vector<size_t>{};
    }

    void clear_dll(){
        Node *prev_node = prev, *next_node = next;
        while(prev_node != nullptr){
            Node* tmp_node = prev_node->prev;
            free(prev_node);
            prev_node = tmp_node;
        }

        while(next_node != nullptr){
            Node* tmp_node = next_node->next;
            free(next_node);
            next_node = tmp_node;
        }

        free(this);
    }
};

class Segment {
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

    static size_t* ptrAllocate(unsigned int count){
        Node* tmp_node = ptr_dll_head;
        while (tmp_node->state == EMPTY || (size_t*) tmp_node->end_ptr - count < (size_t*) tmp_node->start_ptr){
            tmp_node = tmp_node->next;
            if(tmp_node == nullptr)
                throw new NoEmptySpaceException;
        }
        
        if(tmp_node->prev == nullptr){
            Node* new_node = new Node(tmp_node->start_ptr, (byte*) (tmp_node->start_ptr) + count, RESERVED);
            new_node->next = tmp_node;
            tmp_node->prev = new_node;
        }
        else 
            tmp_node->prev->end_ptr = (size_t*) tmp_node->prev->end_ptr + count;
        tmp_node->start_ptr = (size_t*) tmp_node->start_ptr + count;

        return (size_t*) (tmp_node->prev->start_ptr);
    }

    void* dataAllocate(unsigned int count){
        Node* tmp_node = data_dll_head;
        while (tmp_node->state == EMPTY || (byte*) tmp_node->end_ptr - count < (byte*) tmp_node->start_ptr){
            tmp_node = tmp_node->next;
            if(tmp_node == nullptr)
                throw new NoEmptySpaceException;
        }

        if(tmp_node->prev == nullptr){
            Node* new_node = new Node(tmp_node->start_ptr, (byte*) (tmp_node->start_ptr) + count , RESERVED);
            new_node->next = tmp_node;
            tmp_node->prev = new_node;
        }
        else 
            tmp_node->prev->end_ptr = (byte*) tmp_node->prev->end_ptr + count;
        tmp_node->start_ptr = (byte*) tmp_node->start_ptr + count;

        return tmp_node->prev->start_ptr;
    }

    void resizeDataSegment(unsigned int new_size){
        //TODO
    }

public:
    unsigned int id;

    Segment() {
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

    ~Segment() {
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

    void NewPointer(void* p, unsigned int bytes){
        size_t* ptr_ptr;
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
                    //TODO расширение текущего сегмента
                }
                return;
            }
        } while(changeSegmentAllowed);
        p = ptr_ptr;
    }

    template <typename T>
    void WritePointer(void* p, T data){
        if(p != nullptr && p != NULL && *p != nullptr && *p != null)
            *(*p) = data;
        else
            throw new NullPtrException();
    }

    template <typename T>
    T ReadPointer(void* p){
        if(p != nullptr && p != NULL && *p != nullptr && *p != null)
            return *(*p);
        else
            throw new NullPtrException();
    }

    template <typename T>
    void SetPointer(void* p, void* b){
        WritePointer(p, ReadPointer(b));
    }

    void FreePointer(){
        //TODO
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