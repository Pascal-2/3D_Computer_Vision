#include "kdtree.h"
#include "PointCloud.h"
#include <iostream>
#include <cstdlib>

int compare_x(const void *a, const void *b) {
    Point *a_p = (Point *)a;
    Point *b_p = (Point *)b;
    float diff = a_p->x - b_p->x;
    if (diff < 0) {
        return -1;
    }
    if (diff > 0) {
        return 1;
    }
    return 0;
}

int compare_y(const void *a, const void *b) {
    Point *a_p = (Point *)a;
    Point *b_p = (Point *)b;
    float diff = a_p->y - b_p->y;
    if (diff < 0) {
        return -1;
    }
    if (diff > 0) {
        return 1;
    }
    return 0;
}

int compare_z(const void *a, const void *b) {
    Point *a_p = (Point *)a;
    Point *b_p = (Point *)b;
    float diff = a_p->z - b_p->z;
    if (diff < 0) {
        return -1;
    }
    if (diff > 0) {
        return 1;
    }
    return 0;
}

KdTree::KdTree(const QString& filePath) {
    // filePath = "../../../Data/bunny.ply"
    this->points.loadPLY(filePath);
    this->n = this->points.size();
    this->k = 3;
    this->point_pointer = (Point *)malloc(sizeof(Point) * this->n);
    if(!this->point_pointer) {
        std::cout << "Error using malloc\n";
        exit(1);
    }

    for(int i = 0; i < this->n; i++) {
        this->point_pointer[i] = (Point) {this->points[i][0], this->points[i][1], this->points[i][2], i};
    }

    qsort(this->point_pointer, this->n, sizeof(Point), compare_x);
    for(int i = 0; i < this->n; i++) {
        this->sorted_indices[0][i] = this->point_pointer[i].idx;
        this->sorted_indices[0].push_back(this->point_pointer[i].idx);
    }
    qsort(this->point_pointer, this->n, sizeof(Point), compare_y);
    for(int i = 0; i < this->n; i++) {
        this->sorted_indices[1].push_back(this->point_pointer[i].idx);
    }
    qsort(this->point_pointer, this->n, sizeof(Point), compare_z);
    for(int i = 0; i < this->n; i++) {
        this->sorted_indices[2].push_back(this->point_pointer[i].idx);
    }


}

Node *KdTree::build_kd_tree(std::vector<std::vector<int>> sorted_lists, int depth) {
    bool b = true;
    for (int i = 0; i < 3; i++) {
        if (sorted_lists[i].size() > 0) {
            b = false;
            break;
        }
    }
    if (b) {
        return NULL;
    }

    int axis = depth % this->k;

    if (!sorted_lists[axis].size() > 0) {
        return NULL;
    }

    int median_idx = sorted_lists[axis].size() / 2;
    int median_point_idx = sorted_lists[axis][median_idx];
    QVector4D median_point = this->points[median_point_idx];

    std::vector<int> left_sorted_lists;
    std::vector<int> right_sorted_lists;

    for (int a = 0; a < 3; a++) {
        std::vector<int> left_list;
        std::vector<int> right_list;
        for (int x : sorted_lists[a]) {
            if (this->points[x][axis] < median_point[axis])
        }
    }



}

//Für Abgabe 3:


