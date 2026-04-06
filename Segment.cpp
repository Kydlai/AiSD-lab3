#include "Segment.h"

template <typename T> T* Segment::Allocate(unsigned int count, Node* dll_head){ // <---------------------------------------------------
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
    if(tmp_node->prev == nullptr){ // Если необходимо создать новый блок
        Node* new_node = new Node(tmp_node->start_ptr, (T*) (tmp_node->start_ptr) + count, RESERVED);
        new_node->next = tmp_node;
        tmp_node->prev = new_node;
    }
    else // Если необходимо расширить существующий
        tmp_node->prev->end_ptr = (T*) tmp_node->prev->end_ptr + count;
    
    tmp_node->start_ptr = (T*) tmp_node->start_ptr + count;
    if(tmp_node->next == nullptr)
        tmp_node->next = tmp_node->prev;   
    auto result = (T*) (tmp_node->prev->end_ptr) - count;
    Node::nodeCollapse(tmp_node, dll_head);
    return result;
}

size_t* Segment::ptrAllocate(unsigned int count){
    if(count > default_ptr_segment_size)
        throw NoEmptySpaceException();
    return Allocate<size_t>(count, ptr_dll_head);
}

void* Segment::dataAllocate(unsigned int count){
    if(count > data_segment_size)
        throw NoEmptySpaceException();
    return Allocate<byte>(count, data_dll_head);
}

void Segment::resizeDataSegment(unsigned int new_size){
    if(new_size > this->data_segment_size){
        byte* old_data_segment_start = (byte*) data_segment;
        byte* old_data_segment_end = (byte*) data_segment + data_segment_size;
        data_segment = (byte*) realloc(data_segment, new_size);
        data_segment_size = new_size;
        for(Node* tmp_node = ptr_dll_head; ;){ // Перенос данных ptr_segment
            if(tmp_node->state == RESERVED)
                if((byte*) *((size_t*) tmp_node->start_ptr) <= (byte*) old_data_segment_end && (byte*) *((size_t*) tmp_node->end_ptr) >= (byte*) old_data_segment_start)
                    for(byte** i = (byte**) tmp_node->start_ptr; i != tmp_node->end_ptr; i += 2) // i -> указатель на указатель
                        if((byte*) *i >= old_data_segment_start && (byte*) *(i) <=  old_data_segment_end)
                            *i = (byte*) data_segment + ((byte*) *i - (byte*) old_data_segment_start);
                        
            tmp_node = tmp_node->next;

            if(tmp_node->next == nullptr || tmp_node == ptr_dll_head)
                break;
        }
    }
    //printSegments();
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
    data_dll_head = new Node(data_segment, (byte*) data_segment + data_segment_size, EMPTY);
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

void Segment::NewPointer(void*& p, unsigned int bytes){ // <-------------------------------------------------------
    size_t* ptr_ptr = nullptr;
    try{
        ptr_ptr = ptrAllocate(2); // Указатель на ячеку, храняюшую указатель на данные
    } catch (NoEmptySpaceException e) {
        cout << e.what() << " для указателя\n";
        return;
    }
    do{
        int tmpSegmentId = this->id;
        try{
            *ptr_ptr = (size_t) this->dataAllocate(bytes); // Указатель на ячейку с данными
            size_t* bytes_count_ptr = ptr_ptr + 1;
            *bytes_count_ptr = bytes;
        } catch (NoEmptySpaceException e) {
            //printSegments();
            FreePointer(ptr_ptr);
            tmpSegmentId = (tmpSegmentId + 1) % idCounter;
            //printSegments();
            if(!changeSegmentAllowed || tmpSegmentId == this->id){
                this->resizeDataSegment(data_segment_size * 8);
                return NewPointer(p, bytes);
            }
        }
    } while(changeSegmentAllowed && ptr_ptr != nullptr);
    p = ptr_ptr;
    return;
}


// Копирует из b в p
template <typename T> void Segment::SetPointer(void* p, unsigned int shift_p, void* b, unsigned int shift_b){ // <---------------------------------------------------
    WritePointer<T>((T*)p, shift_p, ReadPointer<T>((T*)b, shift_b));
}

template <typename T> void Segment::SetPointer(void* p, void* b){ 
   SetPointer<T>(p, 0, b, 0);
}

template void Segment::SetPointer<int>(void*, void*);
template void Segment::SetPointer<float>(void*, void*);

template void Segment::SetPointer<int>(void*, unsigned int shift_p, void*, unsigned int shift_b);
template void Segment::SetPointer<float>(void*, unsigned int shift_p, void*, unsigned int shift_b);

void Segment::FreePointer(void* p){ // <---------------------------------------------------------------------------
    size_t** ptr_ptr = (size_t**) ((byte*) p); // указатель на указатель
    byte* data_ptr = (byte*) *(ptr_ptr); // указатель на данные
    //printSegments();
    this->data_dll_head->removeData(data_ptr, (size_t) *(ptr_ptr + 1));
    ptr_dll_head->removeData(ptr_ptr, sizeof(size_t) * 2);
   // printSegments();
}

unsigned int Segment::getSize(void* p){
    size_t* ptr = (size_t*) p;
    return *(ptr + 1);
}

template <typename T> void Segment::printSegments(){
    cout << "\n\n";
    Segment::segments[0]->printSegment<int>("Segment ptrs", ptr_dll_head, -1);
    cout << "\n\n";
    for(int i = 0; i < Segment::segments.size(); ++i){
        Segment::segments[i]->printSegment<T>("Segment №" , Segment::segments[i]->data_dll_head, i);
        //cout << "\n═══════════════════════════════════════════════════════════════════\n";
        cout << "\n───────────────────────────────────────────────────────────────────\n";
    }
}

template void Segment::printSegments<int>();
template void Segment::printSegments<float>();

template <typename T> void Segment::printSegment(string label, Node* dll_head, int num){
    int tabs_count = 4;

    string one_tab_over = "╦══════════════";
    string n_tabs_over = "";
    for(int i = 0; i < tabs_count; ++i){
        n_tabs_over += one_tab_over;
    }

    string one_tab_middle = "╬══════════════";
    string n_tabs_middle = "";
    for(int i = 0; i < tabs_count; ++i){
        n_tabs_middle += one_tab_middle;
    }
    string one_tab_lover = "╩══════════════";
    string n_tabs_lover = "";
    for(int i = 0; i < tabs_count; ++i){
        n_tabs_lover += one_tab_lover;
    }
    string sololine = "════════════════════════════════════════════════════════════";
    cout << "╔═════" << sololine << "╗\n";
    if(num == -1)
        printf("║%40s%42s║\n", "Сегмент указателей", "");
    else
        printf("║%35s%-5d%34s║\n", "Сегмент №", num, "");
    cout << "╠═════" << n_tabs_over << "╣\n";
    printf("║ %5s ║ %12s ║ %12s ║ %12s ║ %12s ║\n", "№", "start ptr", "end ptr", "status", "data");
    cout << "╠═════" << n_tabs_middle << "╣\n";
    int j = 0;
    for(Node* tmp_node = dll_head->next; true; tmp_node = tmp_node->next){
        if(tmp_node == nullptr)
            break;
        printf("║ %3d ║ %12p ║ %12p ║ %12s ║ %12s ║\n", j++, tmp_node->start_ptr, tmp_node->end_ptr, tmp_node->state == EMPTY ? "EMPTY" : "RESERVED", "");
        cout << "╠═════" << n_tabs_middle << "╣\n";
        if(tmp_node->state == RESERVED){
            int k = 0;
            for(float* i = (float*) tmp_node->start_ptr; i < (float*) tmp_node->end_ptr; ++i, ++k){
                if(dll_head != ptr_dll_head){
                    if constexpr (is_same_v<T, float>)
                        printf("║ %3s ║ %12p ║ %12s ║ %12s ║ %12.1f ║\n", "", i, "", "int",  *((float*)i));
                    else
                        printf("║ %3s ║ %12p ║ %12s ║ %12s ║ %12d ║\n", "", i, "", "int",  *((int*)i));
                    cout << "╠═════" << n_tabs_middle << "╣\n";
                }
                else{
                    if(!(k%2))
                        printf("║ %3s ║ %12p ║ %12s ║ %12s ║ %12p ║\n", "", i, "", "ptr", (int*) *((int**)i++));
                    else{
                        if constexpr (is_same_v<T, float>)
                            printf("║ %3s ║ %12p ║ %12s ║ %12s ║ %12.1f ║\n", "", i, "", "bytes", *((float*)i++));
                        else
                            printf("║ %3s ║ %12p ║ %12s ║ %12s ║ %12d ║\n", "", i, "", "bytes", *((int*)i++));
                        cout << "╠═════" << n_tabs_middle << "╣\n";;
                    }
                }
                
            }
        }
        if(tmp_node == dll_head) break;
    }
    if(dll_head != ptr_dll_head)
        printf( "║size:║ %12d ║ %12s ║ %12s ║ %12s ║\n", default_ptr_segment_size, "", "", "");
    else
        printf( "║size:║ %12d ║ %12s ║ %12s ║ %12s ║\n", this->data_segment_size, "", "", "");
    cout << "╚═════" << n_tabs_lover << "╝\n";
}

template void Segment::printSegment<int>(string label, Node* dll_head, int num);
template void Segment::printSegment<float>(string label, Node* dll_head, int num);

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

unsigned int Segment::getCount(){
    return count;
}
