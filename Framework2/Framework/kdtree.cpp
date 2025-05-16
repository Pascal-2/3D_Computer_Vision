#include "kdtree.h"
#include "PointCloud.h"
#include <iostream>
#include <cstdlib>


void KdTree::print_tree(Node* node) {
    if (node != NULL) {
        print_tree(node->left);
        std::cout << node->point[0] << ","
                  << node->point[1] << ","
                  << node->point[2] << "\n";
        print_tree(node->right);
    }

}

KdTree::KdTree(std::vector<Point> point_cloud) {
    std::cout << "hallo, ich konstruiere\n"; //debugging
    this->points = point_cloud;
    this->n = this->points.size();
    std::cout << n << "\n";
    this->k = 3;

    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.coordinates[0] < b.coordinates[0];
    });
    for(int i = 0; i < this->n; i++) {
        this->sorted_indices[0].push_back(this->points[i].idx);
    }
    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.coordinates[1] < b.coordinates[1];});
    for(int i = 0; i < this->n; i++) {
        this->sorted_indices[1].push_back(this->points[i].idx);
    }
    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.coordinates[2] < b.coordinates[2];});
    for(int i = 0; i < this->n; i++) {
        this->sorted_indices[2].push_back(this->points[i].idx);
    }
    this->root = build_kd_tree(sorted_indices, 0);
    print_tree(root);

}

Node *KdTree::build_kd_tree(std::vector<std::vector<int>> sorted_lists, int depth) {
    std::cout << "1"; //debugging
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

    if (!(sorted_lists[axis].size() > 0)) {
        return NULL;
    }

    int median_idx = sorted_lists[axis].size() / 2;
    int median_point_idx = sorted_lists[axis][median_idx];
    float median_point[3] = {points[median_point_idx].coordinates[0],
                             points[median_point_idx].coordinates[1],
                             points[median_point_idx].coordinates[2]};

    std::vector<std::vector<int>> left_sorted_lists;
    std::vector<std::vector<int>> right_sorted_lists;

    for (int a = 0; a < 3; a++) {
        std::vector<int> left_list;
        std::vector<int> right_list;
        for (int x : sorted_lists[a]) {
            if (points[x].coordinates[axis] < median_point[axis]) {
                left_list.push_back(x);
            } else if (points[x].coordinates[axis] > median_point[axis]) {
                right_list.push_back(x);
            }
        }
        right_sorted_lists.push_back(right_list);
        left_sorted_lists.push_back(left_list);
    }
    Node *result = (Node *) malloc(sizeof(Node));
    if (!result) {
        std::cout << "Error using malloc\n";
        exit(1);
    }
    //free(sorted_lists); TODO: free
    Node *left = build_kd_tree(left_sorted_lists, depth + 1);
    Node *right = build_kd_tree(right_sorted_lists, depth + 1);
    *result = (Node) {left, right, {median_point[0], median_point[1], median_point[2]}};
    return result;

}




//Für Abgabe 3:


