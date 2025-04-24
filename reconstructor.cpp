#include "reconstructor.h"
#include <iostream>


Reconstructor::Reconstructor(PerspectiveCamera camLeft, PerspectiveCamera camRight): camLeft(camLeft), camRight(camRight) {
    type = SceneObjectType::ST_RECONSTRUCTOR;
    f = camLeft.getFocalLength();
    if (f != camRight.getFocalLength()) {
        std::cout << "Haaaaaalt, die Focal Lengths beider Cameras stimmen nicht überein!\n";
    }
    b = (camRight.getCenterOfProjection() - camLeft.getCenterOfProjection()).length();
    std::cout << "b sollte 2.04 sein: " << b << "\n";
    reconstruct();
}

void Reconstructor::reconstruct() {
    std::vector<std::array<QVector4D, 8>> rightObjects = camRight.getObjects();
    std::vector<std::array<QVector4D, 8>> leftObjects = camLeft.getObjects();
    QVector4D clcop = camLeft.getCenterOfProjection();
    for (size_t i = 0; i < rightObjects.size(); i++) {
        std::array<QVector4D, 8> reconstructedObject;
        for (int j = 0; j < 8; j++) {
            float x_parallax = leftObjects[i][j].x() - rightObjects[i][j].x() + b; //weil b x aligned ist dürfen wir das!
            float z = -f * b / x_parallax;
            float y = -z * leftObjects[i][j].y() / f;
            float x = -z * leftObjects[i][j].x() / f;
            reconstructedObject[j] = QVector4D(x,y,z,1) + clcop;
        }
        reconstructedObjects.push_back(reconstructedObject);
    }
}

void Reconstructor::draw (const RenderCamera& renderer, const QColor&, float) const {
    // render points of reconstructed cubes
    for (auto obj : reconstructedObjects) {
        for (auto point : obj) {
            //std::cout << point[0] << ", " << point[1] << ", " << point[2] << ", " << point[3] << "\n";
            renderer.renderPoint(point, QColorConstants::Blue, 8.0);
        }
    }

    // render lines of reconstructed cubes
    std::array<std::array<int, 2>, 12> lines = {{
        {0,1}, {0,3}, {1,2}, {2,3},
        {4,5}, {4,7}, {5,6}, {6,7},
        {0,4}, {1,5}, {2,6}, {3,7}
    }};

    for (auto obj : reconstructedObjects) {
        for (auto el : lines) {
            renderer.renderLine(
                obj[el[0]],
                obj[el[1]],
                QColorConstants::Blue,
                0.5);
        }
    }
}

void Reconstructor::affineMap(const QMatrix4x4& M)
{

}
