#include "PerspectiveCamera.h"



PerspectiveCamera::PerspectiveCamera(Axes axes, float f): axes(axes), f(f) {
    type = SceneObjectType::ST_PERSPECTIVE_CAMERA;
    // z achsenvektor normieren
    // N + f * normierter Vektor = H
    // Normalenvektor von imagePlane = normierter Vektor
    QVector3D z = axes.getZ();
    z.normalize();
    QVector4D z4D = QVector4D(z);
    QVector4D o4D = axes.getOrigin();
    QVector3D o3D = QVector3D(o4D);
    QVector3D h3D = o3D + f * z;
    QVector4D h4D = QVector4D(h3D);
    imagePlane = Plane(h4D, z4D);
    h = h4D;
    n = o4D;
}

void PerspectiveCamera::draw (const RenderCamera& renderer, const QColor&, float) const {
    imagePlane.draw(renderer, QColorConstants::Red, 0.2);
    axes.draw(renderer, QColorConstants::Red);
    renderer.renderPoint(h,QColorConstants::White, 10.0);
    renderer.renderPoint(n,QColorConstants::White, 10.0);


}

void PerspectiveCamera::affineMap(const QMatrix4x4& M)
{
    imagePlane.affineMap(M);
    axes.affineMap(M);
}

void PerspectiveCamera::addObject(SceneObject* object) {
    objects.push_back(object);
}

/*
std::vector<std::tuple<QVector4D,QVector4D>> PerspectiveCamera::projection() {
    for (auto obj : objects) if (obj) {
            switch (obj->getType()) {
            case ST_CUBE:
            case ST_HEXAHEDRON:
                obj->draw(renderer,color,2.0f);
                break;

            }
        }
}
*/
