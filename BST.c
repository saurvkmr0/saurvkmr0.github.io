#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *left, *right;
} Node;

int search(Node* root, int value) {
    if (root == NULL) {
        return 0; // Not found
    }

    if (value == root->value) {
        return 1; // Found
    } else if (value < root->value) {
        return search(root->left, value);
    } else {
        return search(root->right, value);
    }
}

Node* createNode(int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

int main() {
    Node *root = createNode(50);
    root->left = createNode(30);
    root->right = createNode(70);
    root->left->left = createNode(20);
    root->left->right = createNode(40);
    root->right->left = createNode(60);
    root->right->right = createNode(80);

    printf("Searching for 40: %s\n", search(root, 40) ? "Found" : "Not Found");
    printf("Searching for 25: %s\n", search(root, 25) ? "Found" : "Not Found");

    return 0;
}
