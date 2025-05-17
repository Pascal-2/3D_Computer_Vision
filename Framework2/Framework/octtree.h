#ifndef OCTTREE_H
#define OCTTREE_H
#include "SceneObject.h"

struct Cube_node {
    std::vector<std::array<double, 3>> points;
    struct Cube_node *cubes[8];
    double origin[3];
    double length;
};
typedef struct Cube_node Cube_node;


class Octtree: public SceneObject
{
public:
    Octtree(std::vector<std::array<double, 3>> &point_cloud);
    virtual ~Octtree() override {};
    virtual void affineMap(const QMatrix4x4  & matrix) override;
    Octtree& operator=(const Octtree& kdt);
    void print_tree(Cube_node* node);
    virtual void draw(const RenderCamera& renderer, const QColor&, float) const override;

private:
    std::vector<std::array<double, 3>> points;
    int n;
    Cube_node *root;
    Cube_node *build_octtree(std::vector<std::array<double, 3>> points,
                             double seitenlaengen,
                             double origin[3],
                             int depth);
    void draw_splits(Cube_node * node, int depth, const RenderCamera& renderer) const;
    double origin_corner[3];
    double original_size;
};


#endif // KDTREE_H
