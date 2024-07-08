#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_DOCUMENTS 100
#define MAX_WORDS 1000

typedef struct DocumentList {
    int docID;
    struct DocumentList *next;
} DocumentList;

typedef struct InvertedIndex {
    char word[MAX_WORD_LENGTH];
    DocumentList *documents;
    struct InvertedIndex *next;
} InvertedIndex;

InvertedIndex *createNode(const char *word) {
    InvertedIndex *node = (InvertedIndex *)malloc(sizeof(InvertedIndex));
    strcpy(node->word, word);
    node->documents = NULL;
    node->next = NULL;
    return node;
}

void addDocument(InvertedIndex *index, int docID) {
    DocumentList *docNode = (DocumentList *)malloc(sizeof(DocumentList));
    docNode->docID = docID;
    docNode->next = index->documents;
    index->documents = docNode;
}

InvertedIndex *findIndex(InvertedIndex *head, const char *word) {
    InvertedIndex *current = head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void addWordToIndex(InvertedIndex **head, const char *word, int docID) {
    InvertedIndex *index = findIndex(*head, word);
    if (index == NULL) {
        index = createNode(word);
        index->next = *head;
        *head = index;
    }
    addDocument(index, docID);
}

void tokenize(char *content, InvertedIndex **index, int docID) {
    char *token = strtok(content, " \t\n");
    while (token != NULL) {
        char word[MAX_WORD_LENGTH];
        int i = 0;
        for (i = 0; token[i]; i++) {
            word[i] = tolower(token[i]);
        }
        word[i] = '\0';
        addWordToIndex(index, word, docID);
        token = strtok(NULL, " \t\n");
    }
}

void printInvertedIndex(InvertedIndex *index) {
    while (index != NULL) {
        printf("%s: ", index->word);
        DocumentList *doc = index->documents;
        while (doc != NULL) {
            printf("%d ", doc->docID);
            doc = doc->next;
        }
        printf("\n");
        index = index->next;
    }
}

void freeInvertedIndex(InvertedIndex *index) {
    while (index != NULL) {
        InvertedIndex *temp = index;
        index = index->next;
        DocumentList *doc = temp->documents;
        while (doc != NULL) {
            DocumentList *docTemp = doc;
            doc = doc->next;
            free(docTemp);
        }
        free(temp);
    }
}

int main() {
    InvertedIndex *index = NULL;

    char *documents[MAX_DOCUMENTS] = {
        "Hello world",
        "Hello there",
        "World of programming"
    };

    for (int i = 0; i < 3; i++) {
        tokenize(documents[i], &index, i + 1);
    }

    printInvertedIndex(index);
    freeInvertedIndex(index);

    return 0;
}
