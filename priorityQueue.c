#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10

typedef struct PriorityQueue {
    int *data;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue *createPriorityQueue() {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->data = (int *)malloc(INITIAL_CAPACITY * sizeof(int));
    pq->size = 0;
    pq->capacity = INITIAL_CAPACITY;
    return pq;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue *pq, int index) {
    while (index > 0 && pq->data[index] > pq->data[(index - 1) / 2]) {
        swap(&pq->data[index], &pq->data[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void insert(PriorityQueue *pq, int value) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->data = (int *)realloc(pq->data, pq->capacity * sizeof(int));
    }
    pq->data[pq->size] = value;
    heapifyUp(pq, pq->size);
    pq->size++;
}

void heapifyDown(PriorityQueue *pq, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < pq->size && pq->data[left] > pq->data[largest]) {
        largest = left;
    }
    if (right < pq->size && pq->data[right] > pq->data[largest]) {
        largest = right;
    }
    if (largest != index) {
        swap(&pq->data[index], &pq->data[largest]);
        heapifyDown(pq, largest);
    }
}

int extractMax(PriorityQueue *pq) {
    if (pq->size <= 0) return -1;
    int root = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heapifyDown(pq, 0);
    return root;
}

int peek(PriorityQueue *pq) {
    if (pq->size <= 0) return -1;
    return pq->data[0];
}

void freePriorityQueue(PriorityQueue *pq) {
    free(pq->data);
    free(pq);
}

int main() {
    PriorityQueue *pq = createPriorityQueue();
    
    insert(pq, 10);
    insert(pq, 20);
    insert(pq, 5);
    insert(pq, 15);
    
    printf("Max: %d\n", peek(pq));
    
    printf("Extracted Max: %d\n", extractMax(pq));
    printf("Max: %d\n", peek(pq));
    
    freePriorityQueue(pq);
    
    return 0;
}
