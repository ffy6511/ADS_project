# ifndef FIBHEAP
# define FIBHEAP
//这个可以建一个GLOBAL文件专门存放这些
typedef int Elementtype; 
const int maxDegree = 45;


typedef struct FibNode* Fib_Position;
typedef struct Fib* FibHeap;

struct Fib {
	Fib_Position root;
	int size;
	int rootdegree;
};

struct FibNode {
	Elementtype element;
	Fib_Position left_sibling;
	Fib_Position right_sibling;

	Fib_Position parent;
	Fib_Position children;
	int degree;
	int marked;
};



extern Fib_Position Create_Node(Elementtype x);
extern void Print_Node(Fib_Position p);
extern FibHeap Fibheap_Make_Heap();
extern FibHeap Fibheap_Insert(FibHeap heap, Elementtype x);
extern FibHeap Fibheap_Delete_Min(FibHeap heap);
extern Fib_Position Fibheap_Find(FibHeap heap, Elementtype x);
extern FibHeap Fibheap_Merge(FibHeap h1, FibHeap h2);
extern void Fibheap_Decrease_Key(FibHeap heap, Fib_Position p, Elementtype x);

# endif
