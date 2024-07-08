#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10

typedef struct Item {
    int key;
    int value;
    struct Item* next;
} Item;

Item* hashTable[TABLE_SIZE];

int hashFunction(int key) {
    return key % TABLE_SIZE;
}

void initHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

void insert(int key, int value) {
    int index = hashFunction(key);
    Item* newItem = (Item*)malloc(sizeof(Item));
    newItem->key = key;
    newItem->value = value;
    newItem->next = hashTable[index];
    hashTable[index] = newItem;
}

Item* search(int key) {
    int index = hashFunction(key);
    Item* current = hashTable[index];
    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void delete(int key) {
    int index = hashFunction(key);
    Item* current = hashTable[index];
    Item* prev = NULL;
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void printHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Item* current = hashTable[i];
        while (current != NULL) {
            printf("Key: %d, Value: %d\n", current->key, current->value);
            current = current->next;
        }
    }
}

int main() {
    initHashTable();
    insert(1, 100);
    insert(2, 200);
    insert(11, 1000);
    insert(21, 2000);

    Item* item = search(2);
    if (item != NULL) {
        printf("Found key 2 with value %d\n", item->value);
    } else {
        printf("Key 2 not found\n");
    }

    delete(2);
    item = search(2);
    if (item != NULL) {
        printf("Found key 2 with value %d\n", item->value);
    } else {
        printf("Key 2 not found\n");
    }

    printHashTable();
    return 0;
}
