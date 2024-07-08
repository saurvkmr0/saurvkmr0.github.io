struct MerkleNode {
    unsigned char hash[HASH_SIZE];
    struct MerkleNode *left;
    struct MerkleNode *right;
};

struct MerkleTree {
    struct MerkleNode *root;
    int numLeaves;
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

int verifyMerklePath(unsigned char* leafData, unsigned char** path, int* pathIndices, int pathLength, struct MerkleTree* tree) {
    unsigned char leafHash[HASH_SIZE];
    memcpy(leafHash, computeHash(leafData, DATA_SIZE), HASH_SIZE);
    unsigned char currentHash[HASH_SIZE];
    memcpy(currentHash, leafHash, HASH_SIZE);
    for (int i = 0; i < pathLength; i++) {
        unsigned char combinedHash[2 * HASH_SIZE];
        if (pathIndices[i] == 0) {
            memcpy(combinedHash, currentHash, HASH_SIZE);
            memcpy(combinedHash + HASH_SIZE, path[i], HASH_SIZE);
        } else {
            memcpy(combinedHash, path[i], HASH_SIZE);
            memcpy(combinedHash + HASH_SIZE, currentHash, HASH_SIZE);
        }
        memcpy(currentHash, computeHash(combinedHash, 2 * HASH_SIZE), HASH_SIZE);
    }
    return memcmp(currentHash, tree->root->hash, HASH_SIZE) == 0;
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
