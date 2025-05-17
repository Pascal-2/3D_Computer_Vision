#include "octtree.h"
#include <iostream>
#include <cstdlib>
#include "Cube.h"
#include <algorithm>



void Octtree::draw_splits(Cube_node* node, int depth, const RenderCamera& renderer) const {
    if (depth >= 4) {
        return;
    }
    double current_length = node->length * 1;
    QVector3D vol_center = QVector3D(node->origin[0] + current_length,
                            node->origin[1] + current_length,
                            node->origin[2] + current_length);
    std::array<QVector3D,12> plane_points;
    for (int i = 0; i < 12; i++) {
        plane_points[i] = vol_center;
    }
    //xy-Ebene
    plane_points[0][0] -= current_length;
    plane_points[0][1] += current_length;
    plane_points[1][0] -= current_length;
    plane_points[1][1] -= current_length;
    plane_points[2][0] += current_length;
    plane_points[2][1] -= current_length;
    plane_points[3][0] += current_length;
    plane_points[3][1] += current_length;
    renderer.renderPlane(plane_points[0],
                         plane_points[1],
                         plane_points[2],
                         plane_points[3],
                         QColorConstants::Blue, 0.2f);
    //xz-Ebene
    plane_points[4][0] -= current_length;
    plane_points[4][2] += current_length;
    plane_points[5][0] -= current_length;
    plane_points[5][2] -= current_length;
    plane_points[6][0] += current_length;
    plane_points[6][2] -= current_length;
    plane_points[7][0] += current_length;
    plane_points[7][2] += current_length;
    renderer.renderPlane(plane_points[4],
                         plane_points[5],
                         plane_points[6],
                         plane_points[7],
                         QColorConstants::Green, 0.2f);
    //yz-Ebene
    plane_points[8][1] -= current_length;
    plane_points[8][2] += current_length;
    plane_points[9][1] -= current_length;
    plane_points[9][2] -= current_length;
    plane_points[10][1] += current_length;
    plane_points[10][2] -= current_length;
    plane_points[11][1] += current_length;
    plane_points[11][2] += current_length;
    renderer.renderPlane(plane_points[8],
                         plane_points[9],
                         plane_points[10],
                         plane_points[11],
                         QColorConstants::Red, 0.2f);
    for (size_t i = 0; i < 8; i++) {
        if (node->cubes[i] != NULL) {
            draw_splits(node->cubes[i], depth + 1, renderer);
        }
    }
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
    /*
    int bin[8][3] = { {0, 0, 0},
                       {0, 0, 1},
                       {0, 1, 0},
                       {0, 1, 1},
                       {1, 0, 0},
                       {1, 0, 1},
                       {1, 1, 0},
                       {1, 1, 1}};
    */
    int bin[8][3] = {{0, 0, 0},
                     {1, 0, 0},
                     {0, 1, 0},
                     {1, 1, 0},
                     {0, 0, 1},
                     {1, 0, 1},
                     {0, 1, 1},
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
    for (size_t i = 0; i < points.size(); i++) {
        QVector3D point = QVector3D(points[i][0],
                                    points[i][1],
                                    points[i][2]);
        //std::cout << point[0] << "\n";
        renderer.renderPoint(point, QColorConstants::White, 3.0);
    }
    draw_splits(root, 0, renderer);
}

void Octtree::affineMap(const QMatrix4x4& M)
{

}


