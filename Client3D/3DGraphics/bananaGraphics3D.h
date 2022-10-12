#ifndef BANANAGRAPHICS3D_H
#define BANANAGRAPHICS3D_H

#include "Qt3DExtras/Qt3DWindow"
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <QPropertyAnimation>

#include <QDiffuseSpecularMaterial>
#include <QOrbitCameraController>

#include <Kart/Map/Map.h>
#include <Kart/Player/Item.h>

class BananaGraphics3D: public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    BananaGraphics3D(Item *, Qt3DCore::QEntity *mScene, QNode *parent = nullptr);
    void updateBanana3D(Item *);
    qreal getX();
    qreal getY();
    static qreal bananaRadius;

private:
    qreal x;
    qreal y;
};

#endif // BANANAGRAPHICS3D_H
