#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include "PerspectiveCamera.h"

class Reconstructor: public SceneObject //Careful: Always assumes 'normal case of stereo vision'!
{
public:
    Reconstructor(PerspectiveCamera camLeft, PerspectiveCamera camRight);
    void reconstruct();
    virtual void draw (const RenderCamera& renderer, const QColor&, float) const override;
    virtual ~Reconstructor() override {}
    virtual void affineMap(const QMatrix4x4  & matrix) override;
    Reconstructor& operator=(const Reconstructor& rec);
private:
    PerspectiveCamera camLeft;
    PerspectiveCamera camRight;
    float f; //focal length
    float b; //distance between centers of projections
    std::vector<std::array<QVector4D, 8>> reconstructedObjects;
};

#endif // RECONSTRUCTOR_H
