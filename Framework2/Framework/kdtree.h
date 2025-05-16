#ifndef KDTREE_H
#define KDTREE_H
#include "PointCloud.h"
#include "SceneObject.h"

typedef struct {
    float coordinates[3];
    int idx;
} Point;

struct Node {
    struct Node *left;
    struct Node *right;
    float point[3];
};

typedef struct {
    QVector4D origin;
    QVector4D center;
    QVector3D corners[8];
    float dx;
    float dy;
    float dz;
} Bounding_box;

typedef struct {
    QVector3D points[8];
} Plane_box;

class KdTree: public SceneObject
{
public:
    void print_tree(Node* node);
    KdTree(std::vector<Point> point_cloud);
    virtual ~KdTree() override {};
    virtual void affineMap(const QMatrix4x4  & matrix) override;
    KdTree& operator=(const KdTree& kdt);
    virtual void draw(const RenderCamera& renderer, const QColor&, float) const override;

private:
    std::vector<Point> points;
    int k;
    int n;
    std::vector<std::vector<int>> sorted_indices;
    Node *root;
    Bounding_box bbox;
    Node *build_kd_tree(std::vector<std::vector<int>> sorted_lists, int depth);
    Bounding_box get_bounding_box() const;
    void draw_splits(Node* node, int depth, Plane_box box, const RenderCamera& renderer) const;
};




#endif // KDTREE_H
