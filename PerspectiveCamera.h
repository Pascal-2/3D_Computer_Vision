#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

//#pragma once

#include "Axes.h"
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
    void addObject(SceneObject*);
    std::vector<std::tuple<QVector4D,QVector4D>> projection();
private:
    Axes axes;
    float f;
    Plane imagePlane;
    QVector4D h;
    QVector4D n;
    std::vector<SceneObject*> objects;
};


#endif // PERSPECTIVECAMERA_H
