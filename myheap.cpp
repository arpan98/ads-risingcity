#include <iostream>
#include <vector>
#include <limits>
#include <math.h>

#include "building.h"

class MyHeap
{
private:
    std::vector<BuildingDetails> v;
    int heap_size = 0;

    int parent(int i);
    int left(int i);
    int right(int i);

    void heapify(int i);

public:
    int peek_min();
    int extract_min();
    void decrease_key(int i, int key);
    void insert(BuildingDetails bd);

    void print_heap() {
        std::cout<<"Heap size "<<heap_size<<"\n";
        for (BuildingDetails item : v) {
            std::cout<<item.executed_time<<" ";
        }
        std::cout<<"\n";
    }
};

void MyHeap::heapify(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l <= heap_size && v[l].executed_time < v[i].executed_time) {
        smallest = l;
    }
    if (r <= heap_size && v[r].executed_time < v[smallest].executed_time) {
        smallest = r;
    }
    if (smallest != i) {
        BuildingDetails temp = v[i];
        v[i] = v[smallest];
        v[smallest] = temp;
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

int MyHeap::peek_min() {
    return v[0].executed_time;
}

int MyHeap::extract_min() {
    if (heap_size < 1) {
        return -1; // Error
    }
    int min = v[0].executed_time;
    v[0] = v[heap_size--];
    heapify(0);
    return min;
}

void MyHeap::decrease_key(int i, int key) {
    if (key > v[i].executed_time) {
        return; // Error
    }
    v[i].executed_time = key;
    while (i>0 && v[parent(i)].executed_time > v[i].executed_time) {
        BuildingDetails temp = v[i];
        v[i] = v[parent(i)];
        v[parent(i)] = temp;
        i = parent(i);
    }
}

void MyHeap::insert(BuildingDetails bd) {
    int exec_time = bd.executed_time;
    bd.executed_time = std::numeric_limits<int>::max();
    heap_size += 1;
    std::cout<<heap_size<<" "<<exec_time<<" "<<bd.executed_time<<"\n";
    v.push_back(bd);
    decrease_key(heap_size - 1, exec_time);
}