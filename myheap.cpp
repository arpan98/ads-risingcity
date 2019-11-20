#include <iostream>
#include <vector>
#include <limits>
#include <math.h>

class MyHeap
{
private:
    std::vector<BuildingDetails *> v;
    int heap_size = 0;

    int get_exec_time(int i);

    int parent(int i);
    int left(int i);
    int right(int i);

    void heapify(int i);

public:
    bool is_empty();
    BuildingDetails* extract_min();
    void decrease_key(int i, int key);
    void insert(BuildingDetails *&bd);

    int getHeapSize() {
        return heap_size;
    }

    void print_heap() {
        std::cout<<"Heap size "<<heap_size<<"\n";
        for (int i = 0; i<heap_size; i++) {
            std::cout<<"("<<v[i]->executed_time<<","<<v[i]->buildingNum<<") ";
        }
        std::cout<<"\n";
    }
};

void MyHeap::heapify(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && *v[l] < *v[i]) {
        smallest = l;
    }
    if (r < heap_size && *v[r] < *v[smallest]) {
        smallest = r;
    }
    if (smallest != i) {
        std::swap(v[i], v[smallest]);
        heapify(smallest);
    }
}

int MyHeap::parent(int i) {
    return floor(i / 2);
}

int MyHeap::left(int i) {
    return 2 * i;
}

int MyHeap::right(int i) {
    return 2 * i + 1;
}

bool MyHeap::is_empty() {
    return (heap_size <= 0);
}

BuildingDetails* MyHeap::extract_min() {
    if (heap_size < 1) {
        BuildingDetails *b = new BuildingDetails(0, 0, 0);
        return b; // Error
    }
    // std::cout<<"extract min\n";
    BuildingDetails *min = v[0];
    heap_size--;
    v[0] = v[heap_size];
    heapify(0);
    return min;
}

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

void MyHeap::insert(BuildingDetails *&bd) {
    int exec_time = bd->executed_time;
    bd->executed_time = std::numeric_limits<int>::max();
    // std::cout<<"insert "<<heap_size<<" "<<exec_time<<" "<<bd.executed_time<<" : "<<bd.buildingNum<<"\n";
    if (v.size() == heap_size) {
        v.push_back(bd);
    } else {
        v[heap_size] = bd;
    }
    heap_size++;
    decrease_key(heap_size - 1, exec_time);
}