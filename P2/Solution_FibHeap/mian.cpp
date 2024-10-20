# include "fibheap.h"

int main(void) {
    FibHeap heap = Fibheap_Make_Heap();
    Fibheap_Insert(heap, 10);
    Fibheap_Insert(heap, 2);
    Fibheap_Insert(heap, 15);
    Fibheap_Insert(heap, 6);
    Fibheap_Delete_Min(heap);
    Print_Node(Fibheap_Find(heap, 6));
    return 0;
}