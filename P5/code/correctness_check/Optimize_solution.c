#include<stdio.h>
#include<stdlib.h>

/* global variables */
int N;      // number of items and coupons
float money;

// record relevant item's min_cost by available coupon
typedef struct item{
  int price_id;    
  int coupon_id;  // index of available coupon
  float min_cost; // current least cost
}item;

// min_heap of items
typedef struct min_heap{
  item* items;    
  int size;       
}min_heap;

/* function declaration */
void swap(item* a, item* b);                         // swap a and b
void heapify(min_heap* heap, int i);                 // Maintain the properties of the heap
float delete_min(min_heap* heap, float* prices, float* coupons);  // Get and update the smallest element
void initialize(min_heap* heap, float* prices, float* coupons);   // Initialize the small heap
int partition(float* coupons, int low, int high); // partition for quick sort
void quickSort(float* coupons, int low, int high); // main part of quick sort


/* main function*/
int main(){
   int count_item = 0; // record amount of items bought
   printf("please enter number of item and coupon (N) and money(D):\n");
   scanf("%d %f", &N, &money);
   
   /*read inputs of prices and coupons*/
   float* prices = (float*)malloc(sizeof(float) * N);
   float* coupons = (float*)malloc(sizeof(float) * N);

   for(int i = 0; i < N; i++)
       scanf("%f", &prices[i]);
   for(int i = 0; i < N; i++)
       scanf("%f", &coupons[i]);

    // Sort coupons in descending order
    quickSort(coupons, 0, N-1);

   min_heap heap;
   initialize(&heap, prices, coupons); 

   if(heap.items->min_cost < 0){
    printf("Invalid input : coupon_max < price_min required.\n");
    return 0;
   }

   while(heap.size > 0){
       float min_cost = delete_min(&heap, prices, coupons);
       if(money - min_cost >= 0){
           count_item++;
           money -= min_cost;
       }else{
           break;
       }
   }

   printf("%d %.0f\n", count_item, money);
   
   free(prices);
   free(coupons);
   free(heap.items);
   return 0;
}

/* Function implementation */

/* Quick Sort related functions */
// Partition the array and return the pivot position
int partition(float* coupons, int low, int high) {
    float pivot = coupons[high];  // Choose the last element as pivot
    int i = (low - 1);  // Index of smaller element
    
    for(int j = low; j <= high - 1; j++) {
        // If current element is greater than pivot (for descending order)
        if(coupons[j] > pivot) {
            i++;  // Increment index of smaller element
            // Swap elements
            float temp = coupons[i];
            coupons[i] = coupons[j];
            coupons[j] = temp;
        }
    }
    // Place pivot in its correct position
    float temp = coupons[i + 1];
    coupons[i + 1] = coupons[high];
    coupons[high] = temp;
    return (i + 1);
}

// The main quickSort function
void quickSort(float* coupons, int low, int high) {
    if(low < high) {
        // pi is partitioning index
        int pi = partition(coupons, low, high);
        
        // Separately sort elements before and after partition
        quickSort(coupons, low, pi - 1);
        quickSort(coupons, pi + 1, high);
    }
}

// Swap two elements in the heap
void swap(item* a, item* b) {
  item temp = *a;
  *a = *b;
  *b = temp;
}

// Adjust downward from position i to maintain the nature of the small top heap
void heapify(min_heap* heap, int i) {
  int smallest = i;
  int left = 2 * i + 1;    // left child
  int right = 2 * i + 2;   // right chil
  
  // find the min among i,leef,right
  if(left < heap->size && heap->items[left].min_cost < heap->items[smallest].min_cost)
      smallest = left;
  if(right < heap->size && heap->items[right].min_cost < heap->items[smallest].min_cost)
      smallest = right;
      
// If the minimum value is not i, swap and recursively adjust
  if(smallest != i) {
      swap(&heap->items[i], &heap->items[smallest]);
      heapify(heap, smallest);
  }
}

// Take out the smallest element and update the heap
float delete_min(min_heap* heap, float* prices, float* coupons) {
  if(heap->size == 0) return -1;  // If the heap is empty, -1 is returned.
  
  float min_cost = heap->items[0].min_cost;  // Save minimum spend

  // Update the top element of the heap to the next state (use the next coupon)
  heap->items[0].coupon_id++;
  if(heap->items[0].coupon_id < N) {
      // calculate the new min_cost
      heap->items[0].min_cost = prices[heap->items[0].price_id] -  coupons[heap->items[0].coupon_id];
  } else {
      // The coupon is used up, remove the item from the heap
      heap->items[0] = heap->items[heap->size - 1];
      heap->size--;
  }
  
  heapify(heap, 0);  // Maintain heap properties
  return min_cost;
}

//Initialize the small top heap
void initialize(min_heap* heap, float* prices, float* coupons) {
  heap->size = N;
  heap->items = (item*)malloc(sizeof(item) * N);
  
  // Initialize the initial state of each item (using the maximum coupon)
  for(int i = 0; i < N; i++) {
      heap->items[i].price_id = i; // just copy the price id 
      heap->items[i].coupon_id = 0; // initial using the biggest coupon
      heap->items[i].min_cost = prices[i] - coupons[0]; 
  }
  
  // Build a  heap from the bottom up
  for(int i = N/2 - 1; i >= 0; i--)
      heapify(heap, i);
}
