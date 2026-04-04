#include "Segment.h"

template <typename T> static inline T* Segment::Allocate(unsigned int count, Node* dll_head){
    Node* tmp_node = dll_head; // голова всегда последний блок   if(tmp_node->next != nullptr)
        tmp_node = tmp_node->next; // переход на начальный блок
    if(tmp_node->state == RESERVED){
        tmp_node = tmp_node->next; // копия послеследующей строки
        while (tmp_node->state == RESERVED || (T*) tmp_node->end_ptr - count < (T*) tmp_node->start_ptr){
            tmp_node = tmp_node->next;
            if(tmp_node == nullptr)
                throw NoEmptySpaceException();
        }
    }
    
    if(tmp_node->prev == nullptr){ // Если необходимо создать новый блок
        Node* new_node = new Node(tmp_node->start_ptr, (T*) (tmp_node->start_ptr) + count, RESERVED);
        new_node->next = tmp_node;
        tmp_node->prev = new_node;
    }
    else // Если необходимо расширить существующий
        tmp_node->prev->end_ptr = (T*) tmp_node->prev->end_ptr + count;
    
    tmp_node->start_ptr = (T*) tmp_node->start_ptr + count;
    if(tmp_node->next == nullptr)
        tmp_node->next = tmp_node->prev;   auto result = (T*) (tmp_node->prev->end_ptr) - count;
    Node::nodeCollapse(tmp_node);
    return result;
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

template <typename T> void Segment::WritePointer(void* p, T data){
    if(p != nullptr){
        size_t* ptr = (size_t*) p;
        if(ptr != nullptr)
            *((T*) *ptr) = data;
        else throw NullPtrException();
    } else throw NullPtrException();
    return;
}

template <typename T> T Segment::ReadPointer(void* p){
    if(p != nullptr){
        size_t* ptr = (size_t*)p;
        if(ptr != nullptr)
            return *((T*) *ptr);
        else
            throw NullPtrException();
    } else throw NullPtrException();
}

template <typename T> void Segment::SetPointer(T* p, T* b){
    WritePointer<T>(p, ReadPointer<T>(b));
}

void Segment::FreePointer(void* p){
    
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
