#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DATA_POINTS 100
#define MAX_CLUSTERS 10
#define MAX_ITERATIONS 100

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point center;
    int count;
} Cluster;

Point data[MAX_DATA_POINTS];
Cluster clusters[MAX_CLUSTERS];
int k, num_points;

double distance(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void initClusters() {
    for (int i = 0; i < k; i++) {
        clusters[i].center = data[i];
        clusters[i].count = 0;
    }
}

void assignClusters() {
    for (int i = 0; i < k; i++) {
        clusters[i].count = 0;
    }
    for (int i = 0; i < num_points; i++) {
        int min_index = 0;
        double min_dist = distance(data[i], clusters[0].center);
        for (int j = 1; j < k; j++) {
            double dist = distance(data[i], clusters[j].center);
            if (dist < min_dist) {
                min_index = j;
                min_dist = dist;
            }
        }
        clusters[min_index].count++;
    }
}

void updateCenters() {
    for (int i = 0; i < k; i++) {
        clusters[i].center.x = 0;
        clusters[i].center.y = 0;
    }
    for (int i = 0; i < num_points; i++) {
        int min_index = 0;
        double min_dist = distance(data[i], clusters[0].center);
        for (int j = 1; j < k; j++) {
            double dist = distance(data[i], clusters[j].center);
            if (dist < min_dist) {
                min_index = j;
                min_dist = dist;
            }
        }
        clusters[min_index].center.x += data[i].x;
        clusters[min_index].center.y += data[i].y;
    }
    for (int i = 0; i < k; i++) {
        if (clusters[i].count > 0) {
            clusters[i].center.x /= clusters[i].count;
            clusters[i].center.y /= clusters[i].count;
        }
    }
}

void kMeans() {
    int iter = 0;
    initClusters();
    while (iter < MAX_ITERATIONS) {
        assignClusters();
        updateCenters();
        iter++;
    }
}

void printClusters() {
    for (int i = 0; i < k; i++) {
        printf("Cluster %d: Center (%.2f, %.2f)\n", i, clusters[i].center.x, clusters[i].center.y);
    }
}

int main() {
    num_points = 6;
    k = 2;
    data[0] = (Point){2.0, 10.0};
    data[1] = (Point){2.0, 5.0};
    data[2] = (Point){8.0, 4.0};
    data[3] = (Point){5.0, 8.0};
    data[4] = (Point){7.0, 5.0};
    data[5] = (Point){6.0, 4.0};
    kMeans();
    printClusters();
    return 0;
}
