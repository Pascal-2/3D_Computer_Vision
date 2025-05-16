#ifndef KDTREE_H
#define KDTREE_H
#include "PointCloud.h"

typedef struct {
    float x;
    float y;
    float z;
    int idx;
} Point;

struct Node {
    struct Node *left;
    struct Node *right;
    float point[3];
};

class KdTree
{
public:
    void print_tree(Node* node);
    KdTree(const QString& filePath);

private:
    PointCloud *points;
    int k;
    int n;
    std::vector<std::vector<int>> sorted_indices;
    Point* point_pointer;
    Node *root;
    Node *build_kd_tree(std::vector<std::vector<int>> sorted_lists, int depth);
};



#endif // KDTREE_H
