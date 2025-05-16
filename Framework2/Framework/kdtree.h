#ifndef KDTREE_H
#define KDTREE_H
#include "PointCloud.h"

typedef struct {
    float coordinates[3];
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
    KdTree(std::vector<Point> point_cloud);

private:
    std::vector<Point> points;
    int k;
    int n;
    std::vector<std::vector<int>> sorted_indices;
    Node *root;
    Node *build_kd_tree(std::vector<std::vector<int>> sorted_lists, int depth);
};



#endif // KDTREE_H
