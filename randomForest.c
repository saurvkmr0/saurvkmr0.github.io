#include <stdio.h>
#include <stdlib.h>

#define MAX_TREES 100
#define MAX_FEATURES 10
#define MAX_DATA_POINTS 100
#define MAX_NODES 1000

typedef struct {
    double features[MAX_FEATURES];
    int label;
} DataPoint;

typedef struct {
    int feature;
    double threshold;
    int left;
    int right;
    int label;
} DecisionTreeNode;

typedef struct {
    DecisionTreeNode nodes[MAX_NODES];
    int root;
} DecisionTree;

typedef struct {
    DecisionTree trees[MAX_TREES];
    int num_trees;
} RandomForest;

int getRandomFeature(int num_features) {
    return rand() % num_features;
}

int getRandomSample(int num_samples) {
    return rand() % num_samples;
}

void splitData(DataPoint data[], int num_samples, int feature, double threshold, DataPoint left[], DataPoint right[], int *left_size, int *right_size) {
    *left_size = 0;
    *right_size = 0;
    for (int i = 0; i < num_samples; i++) {
        if (data[i].features[feature] <= threshold) {
            left[(*left_size)++] = data[i];
        } else {
            right[(*right_size)++] = data[i];
        }
    }
}

int createDecisionTree(DataPoint data[], int num_samples, int max_depth, int current_depth) {
    if (num_samples == 0) return -1;
    if (current_depth >= max_depth) return -1;

    int best_feature = getRandomFeature(MAX_FEATURES);
    double best_threshold = getRandomSample(num_samples) / 100.0;

    DataPoint left[MAX_DATA_POINTS];
    DataPoint right[MAX_DATA_POINTS];
    int left_size, right_size;
    splitData(data, num_samples, best_feature, best_threshold, left, right, &left_size, &right_size);

    int node_index = 0;
    if (left_size > 0 && right_size > 0) {
        node_index = current_depth;
        DecisionTreeNode *node = &trees[current_tree].nodes[node_index];
        node->feature = best_feature;
        node->threshold = best_threshold;
        node->left = createDecisionTree(left, left_size, max_depth, current_depth + 1);
        node->right = createDecisionTree(right, right_size, max_depth, current_depth + 1);
    } else {
        DecisionTreeNode *node = &trees[current_tree].nodes[node_index];
        node->label = data[0].label;
    }

    return node_index;
}

void createRandomForest(DataPoint data[], int num_samples, int num_trees, int max_depth) {
    for (int i = 0; i < num_trees; i++) {
        current_tree = i;
        createDecisionTree(data, num_samples, max_depth, 0);
    }
}

int predictRandomForest(RandomForest *forest, DataPoint *dp) {
    int votes[MAX_TREES] = {0};
    for (int i = 0; i < forest->num_trees; i++) {
        int node_index = forest->trees[i].root;
        while (node_index != -1) {
            DecisionTreeNode *node = &forest->trees[i].nodes[node_index];
            if (dp->features[node->feature] <= node->threshold) {
                node_index = node->left;
            } else {
                node_index = node->right;
            }
        }
        int label = forest->trees[i].nodes[node_index].label;
        votes[label]++;
    }
    int max_votes = 0;
    int predicted_label = 0;
    for (int i = 0; i < MAX_TREES; i++) {
        if (votes[i] > max_votes) {
            max_votes = votes[i];
            predicted_label = i;
        }
    }
    return predicted_label;
}

int main() {
    DataPoint data[MAX_DATA_POINTS];
    int num_samples = 100;
    int num_features = 5;
    int num_trees = 10;
    int max_depth = 5;

    RandomForest forest;
    createRandomForest(data, num_samples, num_trees, max_depth);

    DataPoint new_point = {{1.0, 2.0, 3.0, 4.0, 5.0}, 0};
    int label = predictRandomForest(&forest, &new_point);
    printf("Predicted label: %d\n", label);

    return 0;
}
