#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SHARDS 4

typedef struct Shard {
    int id;
    // Data storage for the shard
    // This can be a list, a file, or a database connection
    // For simplicity, we'll use an integer array
    int *data;
    int size;
} Shard;

typedef struct ShardManager {
    Shard *shards[NUM_SHARDS];
} ShardManager;

int hashFunction(int key) {
    return key % NUM_SHARDS;
}

ShardManager *createShardManager() {
    ShardManager *manager = (ShardManager *)malloc(sizeof(ShardManager));
    for (int i = 0; i < NUM_SHARDS; i++) {
        Shard *shard = (Shard *)malloc(sizeof(Shard));
        shard->id = i;
        shard->data = (int *)malloc(100 * sizeof(int)); // Allocate space for data
        shard->size = 0;
        manager->shards[i] = shard;
    }
    return manager;
}

void insert(ShardManager *manager, int key) {
    int shardIndex = hashFunction(key);
    Shard *shard = manager->shards[shardIndex];
    shard->data[shard->size++] = key;
}

int search(ShardManager *manager, int key) {
    int shardIndex = hashFunction(key);
    Shard *shard = manager->shards[shardIndex];
    for (int i = 0; i < shard->size; i++) {
        if (shard->data[i] == key) return 1;
    }
    return 0;
}

void delete(ShardManager *manager, int key) {
    int shardIndex = hashFunction(key);
    Shard *shard = manager->shards[shardIndex];
    int i;
    for (i = 0; i < shard->size; i++) {
        if (shard->data[i] == key) break;
    }
    if (i < shard->size) {
        for (int j = i; j < shard->size - 1; j++) {
            shard->data[j] = shard->data[j + 1];
        }
        shard->size--;
    }
}

void printShardData(Shard *shard) {
    for (int i = 0; i < shard->size; i++) {
        printf("%d ", shard->data[i]);
    }
    printf("\n");
}

void printAllShards(ShardManager *manager) {
    for (int i = 0; i < NUM_SHARDS; i++) {
        printf("Shard %d: ", i);
        printShardData(manager->shards[i]);
    }
}

void freeShardManager(ShardManager *manager) {
    for (int i = 0; i < NUM_SHARDS; i++) {
        free(manager->shards[i]->data);
        free(manager->shards[i]);
    }
    free(manager);
}

int main() {
    ShardManager *manager = createShardManager();
    
    insert(manager, 10);
    insert(manager, 20);
    insert(manager, 30);
    insert(manager, 15);
    
    printAllShards(manager);
    
    if (search(manager, 20)) printf("Found\n");
    else printf("Not Found\n");
    
    delete(manager, 20);
    
    printAllShards(manager);
    
    freeShardManager(manager);
    
    return 0;
}
