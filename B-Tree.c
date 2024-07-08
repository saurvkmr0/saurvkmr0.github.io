#include <stdio.h>
#include <stdlib.h>

#define MIN_DEGREE 2

typedef struct BTreeNode {
    int *keys;
    struct BTreeNode **children;
    int num_keys;
    int leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode *root;
} BTree;

BTreeNode *createBTreeNode(int t, int leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->keys = (int *)malloc((2 * t - 1) * sizeof(int));
    node->children = (BTreeNode **)malloc(2 * t * sizeof(BTreeNode *));
    node->num_keys = 0;
    node->leaf = leaf;
    return node;
}

void traverse(BTreeNode *node) {
    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (!node->leaf) traverse(node->children[i]);
        printf("%d ", node->keys[i]);
    }
    if (!node->leaf) traverse(node->children[i]);
}

void insertNonFull(BTreeNode *node, int key, int t) {
    int i = node->num_keys - 1;
    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i]->num_keys == 2 * t - 1) {
            splitChild(node, i, t);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], key, t);
    }
}

void splitChild(BTreeNode *parent, int i, int t) {
    BTreeNode *new_child = createBTreeNode(t, parent->children[i]->leaf);
    BTreeNode *old_child = parent->children[i];
    new_child->num_keys = t - 1;
    for (int j = 0; j < t - 1; j++) new_child->keys[j] = old_child->keys[j + t];
    if (!old_child->leaf) {
        for (int j = 0; j < t; j++) new_child->children[j] = old_child->children[j + t];
    }
    old_child->num_keys = t - 1;
    for (int j = parent->num_keys; j >= i + 1; j--) parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = new_child;
    for (int j = parent->num_keys - 1; j >= i; j--) parent->keys[j + 1] = parent->keys[j];
    parent->keys[i] = old_child->keys[t - 1];
    parent->num_keys++;
}

void insert(BTree *tree, int key) {
    BTreeNode *root = tree->root;
    if (root->num_keys == 2 * MIN_DEGREE - 1) {
        BTreeNode *new_root = createBTreeNode(MIN_DEGREE, 0);
        tree->root = new_root;
        new_root->children[0] = root;
        splitChild(new_root, 0, MIN_DEGREE);
        insertNonFull(new_root, key, MIN_DEGREE);
    } else {
        insertNonFull(root, key, MIN_DEGREE);
    }
}

BTreeNode *search(BTreeNode *node, int key) {
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) i++;
    if (i < node->num_keys && key == node->keys[i]) return node;
    if (node->leaf) return NULL;
    return search(node->children[i], key);
}

void deleteKey(BTree *tree, int key) {
    BTreeNode *root = tree->root;
    deleteKeyFromNode(root, key, MIN_DEGREE);
    if (root->num_keys == 0) {
        BTreeNode *temp = root;
        if (root->leaf) tree->root = NULL;
        else tree->root = root->children[0];
        free(temp->keys);
        free(temp->children);
        free(temp);
    }
}

void deleteKeyFromNode(BTreeNode *node, int key, int t) {
    int idx = findKey(node, key);
    if (node->leaf) {
        if (idx < node->num_keys && node->keys[idx] == key) {
            for (int i = idx + 1; i < node->num_keys; i++) node->keys[i - 1] = node->keys[i];
            node->num_keys--;
        }
    } else {
        if (idx < node->num_keys && node->keys[idx] == key) {
            if (node->children[idx]->num_keys >= t) {
                int pred = getPredecessor(node, idx);
                node->keys[idx] = pred;
                deleteKeyFromNode(node->children[idx], pred, t);
            } else if (node->children[idx + 1]->num_keys >= t) {
                int succ = getSuccessor(node, idx);
                node->keys[idx] = succ;
                deleteKeyFromNode(node->children[idx + 1], succ, t);
            } else {
                merge(node, idx, t);
                deleteKeyFromNode(node->children[idx], key, t);
            }
        } else {
            if (node->children[idx]->num_keys == t - 1) fill(node, idx, t);
            if (idx > node->num_keys) deleteKeyFromNode(node->children[idx - 1], key, t);
            else deleteKeyFromNode(node->children[idx], key, t);
        }
    }
}

int findKey(BTreeNode *node, int key) {
    int idx = 0;
    while (idx < node->num_keys && node->keys[idx] < key) idx++;
    return idx;
}

int getPredecessor(BTreeNode *node, int idx) {
    BTreeNode *cur = node->children[idx];
    while (!cur->leaf) cur = cur->children[cur->num_keys];
    return cur->keys[cur->num_keys - 1];
}

int getSuccessor(BTreeNode *node, int idx) {
    BTreeNode *cur = node->children[idx + 1];
    while (!cur->leaf) cur = cur->children[0];
    return cur->keys[0];
}

void fill(BTreeNode *node, int idx, int t) {
    if (idx != 0 && node->children[idx - 1]->num_keys >= t) {
        borrowFromPrev(node, idx);
    } else if (idx != node->num_keys && node->children[idx + 1]->num_keys >= t) {
        borrowFromNext(node, idx);
    } else {
        if (idx != node->num_keys) merge(node, idx, t);
        else merge(node, idx - 1, t);
    }
}

void borrowFromPrev(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx - 1];
    for (int i = child->num_keys - 1; i >= 0; i--) child->keys[i + 1] = child->keys[i];
    if (!child->leaf) {
        for (int i = child->num_keys; i >= 0; i--) child->children[i + 1] = child->children[i];
    }
    child->keys[0] = node->keys[idx - 1];
    if (!child->leaf) child->children[0] = sibling->children[sibling->num_keys];
    node->keys[idx - 1] = sibling->keys[sibling->num_keys - 1];
    child->num_keys++;
    sibling->num_keys--;
}

void borrowFromNext(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];
    child->keys[child->num_keys] = node->keys[idx];
    if (!child->leaf) child->children[child->num_keys + 1] = sibling->children[0];
    node->keys[idx] = sibling->keys[0];
    for (int i = 1; i < sibling->num_keys; i++) sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->num_keys; i++) sibling->children[i - 1] = sibling->children[i];
    }
    child->num_keys++;
    sibling->num_keys--;
}

void merge(BTreeNode *node, int idx, int t) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];
    child->keys[t - 1] = node->keys[idx];
    for (int i = 0; i < sibling->num_keys; i++) child->keys[i + t] = sibling->keys[i];
    if (!child->leaf) {
        for (int i = 0; i <= sibling->num_keys; i++) child->children[i + t] = sibling->children[i];
    }
    for (int i = idx + 1; i < node->num_keys; i++) node->keys[i - 1] = node->keys[i];
    for (int i = idx + 2; i <= node->num_keys; i++) node->children[i - 1] = node->children[i];
    child->num_keys += sibling->num_keys + 1;
    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}
