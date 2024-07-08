#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HASH_SIZE 32
#define RING_SIZE 256

struct MerkleNode {
    unsigned char hash[HASH_SIZE];
    struct MerkleNode *left;
    struct MerkleNode *right;
};

struct Node {
    unsigned char hash[HASH_SIZE];
    struct MerkleNode *merkleTree;
    struct Node *next;
};

struct ConsistentHashRing {
    struct Node *nodes[RING_SIZE];
};

unsigned char* computeHash(unsigned char* data, int dataSize);

struct MerkleNode* createLeafNode(unsigned char* data, int dataSize) {
    struct MerkleNode* node = malloc(sizeof(struct MerkleNode));
    memcpy(node->hash, computeHash(data, dataSize), HASH_SIZE);
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct MerkleNode* createInternalNode(struct MerkleNode* left, struct MerkleNode* right) {
    struct MerkleNode* node = malloc(sizeof(struct MerkleNode));
    unsigned char combinedHash[2 * HASH_SIZE];
    memcpy(combinedHash, left->hash, HASH_SIZE);
    memcpy(combinedHash + HASH_SIZE, right->hash, HASH_SIZE);
    memcpy(node->hash, computeHash(combinedHash, 2 * HASH_SIZE), HASH_SIZE);
    node->left = left;
    node->right = right;
    return node;
}

struct MerkleTree* buildMerkleTree(unsigned char** data, int numLeaves) {
    struct MerkleTree* tree = malloc(sizeof(struct MerkleTree));
    struct MerkleNode** nodes = malloc(numLeaves * sizeof(struct MerkleNode*));
    for (int i = 0; i < numLeaves; i++) {
        nodes[i] = createLeafNode(data[i], DATA_SIZE);
    }
    int currentLevelSize = numLeaves;
    while (currentLevelSize > 1) {
        int nextLevelSize = (currentLevelSize + 1) / 2;
        struct MerkleNode** nextLevelNodes = malloc(nextLevelSize * sizeof(struct MerkleNode*));
        for (int i = 0; i < nextLevelSize; i++) {
            struct MerkleNode* left = nodes[2 * i];
            struct MerkleNode* right = (2 * i + 1 < currentLevelSize) ? nodes[2 * i + 1] : left;
            nextLevelNodes[i] = createInternalNode(left, right);
        }
        free(nodes);
        nodes = nextLevelNodes;
        currentLevelSize = nextLevelSize;
    }
    tree->root = nodes[0];
    tree->numLeaves = numLeaves;
    free(nodes);
    return tree;
}

void addNodeToRing(struct ConsistentHashRing *ring, unsigned char* data, int dataSize) {
    struct Node* node = malloc(sizeof(struct Node));
    memcpy(node->hash, computeHash(data, dataSize), HASH_SIZE);
    node->merkleTree = buildMerkleTree(&data, 1);
    int pos = *(int*)node->hash % RING_SIZE;
    node->next = ring->nodes[pos];
    ring->nodes[pos] = node;
}

struct Node* getNodeFromRing(struct ConsistentHashRing *ring, unsigned char* data, int dataSize) {
    unsigned char hash[HASH_SIZE];
    memcpy(hash, computeHash(data, dataSize), HASH_SIZE);
    int pos = *(int*)hash % RING_SIZE;
    struct Node* node = ring->nodes[pos];
    return node;
}

void freeMerkleNode(struct MerkleNode* node) {
    if (node == NULL) return;
    freeMerkleNode(node->left);
    freeMerkleNode(node->right);
    free(node);
}

void freeMerkleTree(struct MerkleTree* tree) {
    freeMerkleNode(tree->root);
    free(tree);
}

void freeNode(struct Node* node) {
    if (node == NULL) return;
    freeMerkleTree(node->merkleTree);
    free(node);
}

void freeConsistentHashRing(struct ConsistentHashRing* ring) {
    for (int i = 0; i < RING_SIZE; i++) {
        struct Node* node = ring->nodes[i];
        while (node != NULL) {
            struct Node* next = node->next;
            freeNode(node);
            node = next;
        }
    }
    free(ring);
}
