#include "kdtree.h"
#include <iostream>
#include <cstdlib>
#include "Hexahedron.h"


void KdTree::print_tree(Node* node) {
    if (node != NULL) {
        print_tree(node->left);
        std::cout << node->point[0] << ","
                  << node->point[1] << ","
                  << node->point[2] << "\n";
        print_tree(node->right);
    }
}

void KdTree::draw_splits(Node* node, int depth, Plane_box box, const RenderCamera& renderer) const {
    if (depth >= 6) {
        return;
    }
    if (node != NULL) {
        int axis = depth % 3;
        float offset;

        Plane_box boxl;
        Plane_box boxr;
        if (axis == 0) {
            offset = abs(node->point[axis] - box.points[0][0]);
            // BOXL
            //bleiben gleich
            boxl.points[0] = box.points[0];
            boxl.points[1] = box.points[1];
            boxl.points[4] = box.points[4];
            boxl.points[5] = box.points[5];
            //aendern sich
            boxl.points[3] = box.points[0];
            boxl.points[3][axis] += offset;
            boxl.points[2] = box.points[1];
            boxl.points[2][axis] += offset;
            boxl.points[7] = box.points[4];
            boxl.points[7][axis] += offset;
            boxl.points[6] = box.points[5];
            boxl.points[6][axis] += offset;
            renderer.renderPlane(boxl.points[6],
                                 boxl.points[2],
                                 boxl.points[3],
                                 boxl.points[7],
                                 QColorConstants::Red, 0.2f);
            // BOXR
            //bleiben gleich
            boxr.points[3] = box.points[3];
            boxr.points[2] = box.points[2];
            boxr.points[7] = box.points[7];
            boxr.points[6] = box.points[6];
            //aendern sich
            boxr.points[0] = boxl.points[3];
            boxr.points[1] = boxl.points[2];
            boxr.points[4] = boxl.points[7];
            boxr.points[5] = boxl.points[6];

        } else if (axis == 1) {
            offset = abs(node->point[axis] - box.points[0][1]);
            // BOXL
            //bleiben gleich
            boxl.points[0] = box.points[0];
            boxl.points[1] = box.points[1];
            boxl.points[2] = box.points[2];
            boxl.points[3] = box.points[3];
            //aendern sich
            boxl.points[4] = box.points[0];
            boxl.points[4][axis] += offset;
            boxl.points[5] = box.points[1];
            boxl.points[5][axis] += offset;
            boxl.points[6] = box.points[2];
            boxl.points[6][axis] += offset;
            boxl.points[7] = box.points[3];
            boxl.points[7][axis] += offset;
            renderer.renderPlane(boxl.points[6],
                                 boxl.points[5],
                                 boxl.points[4],
                                 boxl.points[7],
                                 QColorConstants::Green, 0.2f);
            // BOXR
            //bleiben gleich
            boxr.points[4] = box.points[4];
            boxr.points[5] = box.points[5];
            boxr.points[7] = box.points[7];
            boxr.points[6] = box.points[6];
            //aendern sich
            boxr.points[0] = boxl.points[4];
            boxr.points[1] = boxl.points[5];
            boxr.points[3] = boxl.points[7];
            boxr.points[2] = boxl.points[6];

        } else /* (axis == 3) */ {
            offset = abs(node->point[axis] - box.points[0][2]);
            // BOXL
            //bleiben gleich
            boxl.points[3] = box.points[3];
            boxl.points[0] = box.points[0];
            boxl.points[7] = box.points[7];
            boxl.points[4] = box.points[4];
            //aendern sich
            boxl.points[2] = box.points[3];
            boxl.points[2][axis] += offset;
            boxl.points[1] = box.points[0];
            boxl.points[1][axis] += offset;
            boxl.points[6] = box.points[7];
            boxl.points[6][axis] += offset;
            boxl.points[5] = box.points[4];
            boxl.points[5][axis] += offset;
            renderer.renderPlane(boxl.points[5],
                                 boxl.points[1],
                                 boxl.points[2],
                                 boxl.points[6],
                                 QColorConstants::Blue, 0.2f);
            // BOXR
            //bleiben gleich
            boxr.points[2] = box.points[2];
            boxr.points[1] = box.points[1];
            boxr.points[6] = box.points[6];
            boxr.points[5] = box.points[5];
            //aendern sich
            boxr.points[3] = boxl.points[2];
            boxr.points[0] = boxl.points[1];
            boxr.points[7] = boxl.points[6];
            boxr.points[4] = boxl.points[5];
        }
        draw_splits(node->left, depth + 1, boxl, renderer);
        draw_splits(node->right, depth + 1, boxr, renderer);
    }
}

KdTree::KdTree(std::vector<Point> point_cloud) {
    type = SceneObjectType::ST_KdTree;
    this->points = point_cloud;
    this->n = this->points.size();
    this->k = 3;
    this->sorted_indices.resize(3);

    std::vector<Point> temp = points;
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
    points = temp;
    std::vector<Point>().swap(temp);
    this->root = build_kd_tree(sorted_indices, 0);
    this->bbox = get_bounding_box();
    //print_tree(root);

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
        //std::cout << depth << "\n";
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
    //free sorted_lists
    for (auto& innerVec : sorted_lists) {
        std::vector<int>().swap(innerVec); // Frees memory of inner vector
    }
    std::vector<std::vector<int>>().swap(sorted_lists); // Frees memory of outer vector


    Node *left = build_kd_tree(left_sorted_lists, depth + 1);
    Node *right = build_kd_tree(right_sorted_lists, depth + 1);
    *result = (Node) {left, right, {median_point[0], median_point[1], median_point[2]}};
    return result;

}


Bounding_box KdTree::get_bounding_box() const {
    int max_idx = sorted_indices[0].size() - 1;
    float min_x = points[sorted_indices[0][0]].coordinates[0];
    float min_y = points[sorted_indices[1][0]].coordinates[1];
    float min_z = points[sorted_indices[2][0]].coordinates[2];
    float max_x = points[sorted_indices[0][max_idx]].coordinates[0];
    float max_y = points[sorted_indices[1][max_idx]].coordinates[1];
    float max_z = points[sorted_indices[2][max_idx]].coordinates[2];
    Bounding_box box;
    box.origin = QVector4D(min_x, min_y, min_z, 1);
    box.dx = max_x - min_x;
    box.dy = max_y - min_y;
    box.dz = max_z - min_z;
    box.center = box.origin + QVector4D(box.dx/2, box.dy/2, box.dz/2, 0);
    // fill corners
    box.corners[0] = QVector3D(min_x, min_y, min_z);
    box.corners[1] = QVector3D(min_x, min_y, max_z);
    box.corners[2] = QVector3D(max_x, min_y, max_z);
    box.corners[3] = QVector3D(max_x, min_y, min_z);
    box.corners[4] = QVector3D(min_x, max_y, min_z);
    box.corners[5] = QVector3D(min_x, max_y, max_z);
    box.corners[6] = QVector3D(max_x, max_y, max_z);
    box.corners[7] = QVector3D(max_x, max_y, min_z);
    return box;
}


void KdTree::draw(const RenderCamera& renderer, const QColor& cl, float fl) const {
    Hexahedron hexhe = Hexahedron(bbox.origin, bbox.dx, bbox.dy, bbox.dz);
    hexhe.draw(renderer, cl, fl);
    for (int i = 0; i < points.size(); i++) {
        QVector3D point = QVector3D(points[i].coordinates[0],
                                    points[i].coordinates[1],
                                    points[i].coordinates[2]);
        renderer.renderPoint(point, QColorConstants::White, 3.0);
    }
    renderer.renderPoint(bbox.origin, QColorConstants::Red, 8.0);

    Plane_box box;
    for (int i = 0; i < 8; i++) {
        box.points[i] = bbox.corners[i];
    }
    draw_splits(root, 0, box, renderer);
}

void KdTree::affineMap(const QMatrix4x4& M)
{

}


