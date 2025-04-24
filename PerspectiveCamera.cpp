#include "PerspectiveCamera.h"
#include <iostream>



PerspectiveCamera::PerspectiveCamera(Axes axes, float f): axes(axes), f(f) {
    type = SceneObjectType::ST_PERSPECTIVE_CAMERA;
    // z achsenvektor normieren
    // N + f * normierter Vektor = H
    // Normalenvektor von imagePlane = normierter Vektor
    QVector3D z = - axes.getZ();
    z.normalize();
    n = QVector4D(z);
    QVector4D o4D = axes.getOrigin();
    QVector3D o3D = QVector3D(o4D);
    QVector3D h3D = o3D + f * z;
    QVector4D h4D = QVector4D(h3D);
    imagePlane = Plane(h4D, n);
    H = h4D;
    N = o4D;
}

void PerspectiveCamera::draw (const RenderCamera& renderer, const QColor&, float) const {
    imagePlane.draw(renderer, QColorConstants::Red, 0.2);
    axes.draw(renderer, QColorConstants::Red);
    renderer.renderPoint(H,QColorConstants::White, 10.0);
    renderer.renderPoint(H,QColorConstants::White, 10.0);

    // render points of projected cubes
    for (auto obj : objects) {
        for (auto point : obj) {
            renderer.renderPoint(point, QColorConstants::Green, 8.0);
        }
    }

    // render lines of poijected cubes
    std::array<std::array<int, 2>, 12> lines = {{
        {0,1}, {0,3}, {1,2}, {2,3},
        {4,5}, {4,7}, {5,6}, {6,7},
        {0,4}, {1,5}, {2,6}, {3,7}
    }};

    for (auto obj : objects) {
        for (auto el : lines) {
            renderer.renderLine(
                obj[el[0]],
                obj[el[1]],
                QColorConstants::Green,
                0.5);
        }

    }

}

void PerspectiveCamera::affineMap(const QMatrix4x4& M)
{
    imagePlane.affineMap(M);
    axes.affineMap(M);
}

void PerspectiveCamera::addCube(Cube el) {
    std::array<QVector4D,8> points = el.getPoints();
    auto save = projection(points);
    if (save.has_value()) {
        objects.push_back(save.value());
    }

}

void PerspectiveCamera::addHexahedron(Hexahedron el) {
    std::array<QVector4D,8> points = el.getPoints();
    auto save = projection(points);
    if (save.has_value()) {
        objects.push_back(save.value());
    }
}


std::optional<std::array<QVector4D, 8>> PerspectiveCamera::projection(std::array<QVector4D, 8> object) {
    std::array<QVector4D, 8> projected_object;
    for (int j = 0; j < 8; j++) {
        QVector4D b = N;
        QVector4D v = object[j] - N;
        QVector4D a = H;
        double nenner = QVector4D::dotProduct(n, v);
        if (nenner == 0) {
            return std::nullopt;
        }
        double lambda = - (QVector4D::dotProduct(n, b - a) / nenner);
        QVector4D schnitt = b + lambda * v;
        projected_object[j] = schnitt;
    }
    return projected_object;
}

std::vector<std::array<QVector4D, 8>> PerspectiveCamera::getObjects() {
    return objects;
}

QVector4D PerspectiveCamera::getCenterOfProjection() {
    return N;
}

float PerspectiveCamera::getFocalLength() {
    return f;
}

