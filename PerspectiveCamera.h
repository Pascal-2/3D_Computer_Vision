#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

//#pragma once

#include "Axes.h"
#include "Cube.h"
#include "Plane.h"
#include "SceneObject.h"


class PerspectiveCamera: public SceneObject
{
public:
    PerspectiveCamera(Axes axis, float f);
    virtual void draw (const RenderCamera& renderer, const QColor&, float) const override;
    virtual ~PerspectiveCamera() override {}
    virtual void affineMap(const QMatrix4x4  & matrix) override;
    PerspectiveCamera& operator=(const PerspectiveCamera& pc);
    void addCube(Cube el);
    void addHexahedron(Hexahedron el);

private:
    Axes axes;
    float f;
    Plane imagePlane;
    QVector4D H;
    QVector4D N;
    QVector4D n;
    std::optional<std::array<QVector4D, 8>> projection(std::array<QVector4D, 8> object);
    std::vector<std::array<QVector4D, 8>> objects;
};


#endif // PERSPECTIVECAMERA_H
