#include <iostream>
#include <vector>
#include <limits>
#include <math.h>

// Class for implementation of the min-heap
class MyHeap
{
private:
    // Vector of BuildingDetails object pointers to store in the heap
    std::vector<BuildingDetails *> v;
    int heap_size = 0;

    // Returns the parent index for node index i
    int parent(int i);
    // Returns the left child index for node index i
    int left(int i);
    // Returns the right child index for node index i
    int right(int i);

    // Function to heapify down the subtree rooted at i comparing the node to its children
    void heapify(int i);

public:
    // Function returns true if the heap is empty
    bool is_empty();

    // Function to extract the min building from the heap (at index 0)
    BuildingDetails* extract_min();

    // Function to decrease key(executed_time) of building at index i in heap
    void decrease_key(int i, int key);

    // Function to insert a building into the heap
    void insert(BuildingDetails *&bd);

    // Debug function to print heap as array    
    void print_heap() {
        std::cout<<"Heap size "<<heap_size<<"\n";
        for (int i = 0; i<heap_size; i++) {
            std::cout<<"("<<v[i]->executed_time<<","<<v[i]->buildingNum<<") ";
        }
        std::cout<<"\n";
    }
};

// Function to heapify down - check node with its children and swap with the smaller of the two
void MyHeap::heapify(int i) {
    int l_child = left(i);
    int r_child = right(i);
    int smaller = i;
    if (l_child < heap_size && *v[l_child] < *v[i]) {
        smaller = l_child;
    }
    if (r_child < heap_size && *v[r_child] < *v[smaller]) {
        smaller = r_child;
    }
    if (smaller != i) {
        std::swap(v[i], v[smaller]);
        heapify(smaller);
    }
}

// Function to return parent index given a node index
int MyHeap::parent(int i) {
    return floor((i - 1) / 2);
}

// Function to return left child index given a node index
int MyHeap::left(int i) {
    return (2 * i) + 1;
}

// Function to return right child index given a node index
int MyHeap::right(int i) {
    return (2 * i) + 2;
}

// Function to check if heap is empty
bool MyHeap::is_empty() {
    return (heap_size <= 0);
}

// Function to extract min and return building from heap, then run heapify from root
BuildingDetails* MyHeap::extract_min() {
    if (heap_size < 1) {
        BuildingDetails *b = new BuildingDetails(0, 0, 0);
        return b; // Error: heap is empty
    }
    BuildingDetails *min = v[0];
    heap_size--;
    v[0] = v[heap_size];
    heapify(0);
    return min;
}

// Function to decrease key of node at index i and set it = key
// Follow path up from parent to root swapping when necessary
void MyHeap::decrease_key(int i, int key) {
    if (key > v[i]->executed_time) {
        return; // Error
    }
    v[i]->executed_time = key;
    while (i > 0 && *v[i] < *v[parent(i)]) {
        std::swap(v[i], v[parent(i)]);
        i = parent(i);    
    }
}

// Insert new building into the heap, sets its key(exec_time) as infinity
// And add it to the end of the heap vector
void MyHeap::insert(BuildingDetails *&bd) {
    int exec_time = bd->executed_time;
    bd->executed_time = std::numeric_limits<int>::max();
    if (v.size() == heap_size) {
        v.push_back(bd);
    } else {
        v[heap_size] = bd;
    }
    heap_size++;
    decrease_key(heap_size - 1, exec_time);
}