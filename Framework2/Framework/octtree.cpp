#include "octtree.h"
#include <iostream>
#include <cstdlib>
#include "Cube.h"
#include <algorithm>


void Octtree::print_tree(Cube_node* node) {

}

void Octtree::draw_splits(Cube_node* node, int depth, const RenderCamera& renderer) const {

}

Octtree::Octtree(std::vector<std::array<double, 3>> &point_cloud) {
    type = SceneObjectType::ST_Octtree;
    points = point_cloud;
    this->n = this->points.size();
    double max_dists[3] = {-9999, -9999, -9999};
    double min_dists[3] = {9999, 9999, 9999}; // box origin != world origin!
    for (int i = 0; i < n; i++) {
        for (size_t j = 0; j < 3; j++) {
            max_dists[j] = std::max(max_dists[j], points[i][j]);
            min_dists[j] = std::min(min_dists[j], points[i][j]);
        }
    }
    for (size_t k = 0; k < 3; k++) {
        max_dists[k] -= min_dists[k];
        max_dists[k] /= 2;
    }
    double global_max_dist = std::max(std::max(max_dists[0], max_dists[1]), max_dists[2]);
    for (size_t l = 0; l < 3; l++) {
        this->origin_corner[l] = min_dists[l];
    }
    this->original_size = global_max_dist * 2;
    this->root = build_octtree(points, global_max_dist, min_dists, 0);
}


Cube_node *Octtree::build_octtree(std::vector<std::array<double, 3>> param_points,
                                  double seitenlaengen,
                                  double origin[3],
                                  int depth) {
    std::cout << param_points.size() << "\n";
    if (param_points.size() < 2) {
        Cube_node *node = new Cube_node;
        node->points = param_points;
        for (size_t i = 0; i < 8; i++) {
            node->cubes[i] = NULL;
        }
        for (size_t i = 0; i < 3; i++) {
            node->origin[i] = origin[i];
        }
        node->length = seitenlaengen / 2;
        return node;
    }
    std::array<std::vector<std::array<double,3>>,8> cube_points;
    for (size_t i = 0; i < param_points.size(); i++) {
        int idx_x = param_points[i][0] < origin[0] + seitenlaengen ? 0 : 1;
        int idx_y = param_points[i][1] < origin[1] + seitenlaengen ? 0 : 1;
        int idx_z = param_points[i][2] < origin[2] + seitenlaengen ? 0 : 1;
        int idx = idx_x + 2*idx_y + 4*idx_z;
        cube_points[idx].push_back(param_points[i]);
    }
    int bin[8][3] = { {0, 0, 0},
                       {0, 0, 1},
                       {0, 1, 0},
                       {0, 1, 1},
                       {1, 0, 0},
                       {1, 0, 1},
                       {1, 1, 0},
                       {1, 1, 1}};
    double new_origin[3];
    Cube_node *result = new Cube_node;
    result->points = param_points;
    for (size_t i = 0; i < 8; i++) {
        new_origin[0] = origin[0] + bin[i][0] * seitenlaengen;
        new_origin[1] = origin[1] + bin[i][1] * seitenlaengen;
        new_origin[2] = origin[2] + bin[i][2] * seitenlaengen;
        result->cubes[i] = build_octtree(cube_points[i], seitenlaengen/2, new_origin, depth + 1);
    }
    return result;
}




void Octtree::draw(const RenderCamera& renderer, const QColor& cl, float fl) const {
    QVector4D cube_corner = QVector4D(origin_corner[0], origin_corner[1], origin_corner[2], 1);
    Cube cub = Cube(cube_corner, original_size);
    cub.draw(renderer, cl, fl);
}

void Octtree::affineMap(const QMatrix4x4& M)
{

}


