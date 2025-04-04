#include "reconstructor.h"
#include <iostream>


Reconstructor::Reconstructor(PerspectiveCamera camLeft, PerspectiveCamera camRight): camLeft(camLeft), camRight(camRight) {
    type = SceneObjectType::ST_RECONSTRUCTOR;
    f = camLeft.getFocalLength();
    if (f != camRight.getFocalLength()) {
        std::cout << "Haaaaaalt, die Focal Lengths beider Cameras stimmen nicht überein!\n";
    }
    b = (camRight.getCenterOfProjection() - camLeft.getCenterOfProjection()).length();
    reconstruct();
}

void Reconstructor::reconstruct() {
    std::vector<std::array<QVector4D, 8>> leftObjects = camLeft.getObjects();
    std::vector<std::array<QVector4D, 8>> rightObjects = camRight.getObjects();
    QVector4D clcop = camLeft.getCenterOfProjection(); //add?
    for (size_t i = 0; i < leftObjects.size(); i++) {
        std::array<QVector4D, 8> reconstructedObject;
        for (int j = 0; j < 8; j++) {
            std::cout << "left - right sollte nicht null sein: " << leftObjects[i][j].x()-rightObjects[i][j].x() << "\n";
            std::cout << "leftObject i j x " << leftObjects[i][j].z() << "\n";
            std::cout << "rightObject i j x " << rightObjects[i][j].z() << "\n";
            float z = -f * b / (rightObjects[i][j].z() - leftObjects[i][j].z());
            std::cout << "reconstructed z = : " << z << "\n";
            float y = -z * rightObjects[i][j].y();
            std::cout << "reconstructed y = : " << y << "\n";
            float x = -z * leftObjects[i][j].z() / f;
            std::cout << "reconstructed x = : " << x << "\n";
            reconstructedObject[j] = QVector4D(z,y,x,1) + clcop; //z,y,x weil die Kamera in x Richtung schaut?
        }
        reconstructedObjects.push_back(reconstructedObject);
    }
}

void Reconstructor::draw (const RenderCamera& renderer, const QColor&, float) const {
    // render points of reconstructed cubes
    std::cout << "size of Array reconstructedObjects: " << reconstructedObjects.size() << "\n";
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
