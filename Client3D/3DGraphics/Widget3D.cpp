﻿#include "Widget3D.h"

static int iterKey = 0;

Qt3DCore::QEntity* Widget3D::createScene()
{
    // Configuration de la scène + spécification de l'entity racine
    //En soi, un Qt3DCore::QEntity est une coquille vide.
    //Le comportement d'un objet Qt3DCore::QEntity est défini par les objets Qt3DCore::QComponent auxquels il fait référence.
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    PlaneGraphics3D *planeEntity = new PlaneGraphics3D(rootEntity);

    return rootEntity;
}

Widget3D::Widget3D(): Qt3DExtras::Qt3DWindow()
{
    mScene = this->createScene();
    // Camera
    camerA = this->camera();
    camerA->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camerA->setPosition(QVector3D(0, 0, 4));
    camerA->setViewCenter(QVector3D(0, 0, 0));
    camerA->setFarPlane(1000);
    //camerA->setFieldOfView(50);

    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(mScene);
    camController->setLinearSpeed( 500.0f );
    camController->setLookSpeed( 180.0f );
    camController->setCamera(camerA);


    connect(Map::getInstance(), SIGNAL(updated()), this, SLOT(updateMap3D()));
    connect(Properties::getInstance(), SIGNAL(updated()), this, SLOT(updateProperties3D()));
    connect(GameMode::getInstance(), SIGNAL(updated()), this, SLOT(updateGameMode3D()));
    this->setRootEntity(mScene);
}


void Widget3D::keyPressEvent(QKeyEvent *e)
{


    qDebug() << "Appui touche !";
    if (e->key() == Qt::Key_P) {
      qDebug() << " droite pressed " ;
      if(iterKey + 1 < GameMode::getInstance()->_players->size()){
      iterKey ++;
      }
    }
    if (e->key() == Qt::Key_O) {
      qDebug() << " gauche pressed " ;
      if(iterKey - 1 >= 0){
      iterKey -- ;
      }
    }
    qDebug() << "iterKey = " << iterKey;
}



void Widget3D::updateProperties3D(){
    ObstacleGraphics3D::heigth = Properties::getInstance()->getRectangleHeight();
    ObstacleGraphics3D::width = Properties::getInstance()->getRectangleWidth();
    ObstacleGraphics3D::radius = Properties::getInstance()->getCircleRadius();
    CheckpointGraphics3D::radiusCheckpoint = Properties::getInstance()->getCheckpointRadius();
    BananaGraphics3D::bananaRadius = Properties::getInstance()->getBananaRadius();
    RocketGraphics3D::rocketRadius = Properties::getInstance()->getRocketRadius();
    BombGraphics3D::bombRadius = Properties::getInstance()->getBombRadius();

}

void Widget3D::updateMap3D() {

    QList<Checkpoint*> checkpointList;
    QList<int> checkpointIds;

    for (Obstacle *iterObstacle : Map::getInstance()->getObstacles()->values()) {
        ObstacleGraphics3D* obstacleGraphics3D = localObstacles3D.value(iterObstacle->getId());
        if(!obstacleGraphics3D){
            obstacleGraphics3D = new ObstacleGraphics3D(iterObstacle, mScene);
            localObstacles3D.insert(obstacleGraphics3D->getId(), obstacleGraphics3D);
        }
        // Modifier la position
        obstacleGraphics3D->updateObstacle3D(iterObstacle);
    }

    for (Checkpoint *iterCheckpoint : Map::getInstance()->getCheckpoints()->values()) {
        CheckpointGraphics3D* checkpointGraphics3D = localCheckpoint3D.value(iterCheckpoint->getId());
        if(!checkpointGraphics3D){
            checkpointGraphics3D = new CheckpointGraphics3D(iterCheckpoint, mScene);
            localCheckpoint3D.insert(checkpointGraphics3D->getId(), checkpointGraphics3D);
        }
        // Modifier la position
        checkpointGraphics3D->updateCheckpoint3D(iterCheckpoint);
        checkpointList.append(iterCheckpoint);
        checkpointIds.append(iterCheckpoint->getId());
    }

    // trier les checkpoints
    std::sort(checkpointIds.begin(), checkpointIds.end()); // sort id
    QList<Checkpoint*> checkpointListSorted;
    int iId = 0;

    while((iId+1) < checkpointList.count()) {
        for (Checkpoint *iterCheckpoint : Map::getInstance()->getCheckpoints()->values()) {
            if(checkpointIds[iId] == iterCheckpoint->getId()){
                checkpointListSorted.append(iterCheckpoint);
                iId++;
            }
        }
    }

    //tracer des lignes entre les checkpoint
    int nCheckpoint = Map::getInstance()->getCheckpoints()->count();

    for(int it_checkpoint = 0; it_checkpoint < nCheckpoint; it_checkpoint++) {
        if((it_checkpoint+1) >= nCheckpoint) {
            RoadGraphics3D rg3d(checkpointListSorted[it_checkpoint],
                                checkpointListSorted[0],
                                mScene);
          //qDebug() << "je suis passé par  = " << checkpointListSorted[it_checkpoint] ;

        } else {
            RoadGraphics3D rg3d(checkpointListSorted[it_checkpoint],
                                checkpointListSorted[it_checkpoint+1],
                                mScene);


        }
    }

}

void Widget3D::updateGameMode3D() {

    int iter = 0;
    for (Player *iterPlayer : GameMode::getInstance()->_players->values()) {
        PlayerGraphics3D* playerGraphics3D = localPlayers3D.value(iterPlayer->getUuid());
        if(!playerGraphics3D){
            playerGraphics3D = new PlayerGraphics3D(iterPlayer, mScene);
            localPlayers3D.insert(playerGraphics3D->getUuid(), playerGraphics3D);
        }
        // Modifier la position
        playerGraphics3D->updatePlayer3D(iterPlayer);
        if(iter == iterKey){
            qDebug() << "iter " << iter << " iterKey = " << iterKey;
            qDebug() << "iterPlayer " << iterPlayer->getUuid();
            playerGraphics3D->followCameraPlayer(iterPlayer, camerA);
        }
        iter++;
    }


    for (BananaGraphics3D *iterLocalBanana : localBanana3D){
        iterLocalBanana->setEnabled(false);
        delete(iterLocalBanana);
    }
    for (RocketGraphics3D *iterLocalRocket : localRocket3D){
        iterLocalRocket->setEnabled(false);
        delete(iterLocalRocket);
    }

    for (BombGraphics3D *iterLocalBomb : localBomb3D){
        iterLocalBomb->setEnabled(false);
        delete(iterLocalBomb);
    }

    localBanana3D.clear();
    localRocket3D.clear();
    localBomb3D.clear();

    for (Item *iterItem : *GameMode::getInstance()->_items) {
        if(iterItem->getType() == "banana" && iterItem->getX() < 1000 &&  iterItem->getY() < 1000 && iterItem->getX() > -1000 && iterItem->getY() > -1000){
            BananaGraphics3D *bananaGraphics3D = new BananaGraphics3D(iterItem, mScene);
            localBanana3D.append(bananaGraphics3D);
        }

        if(iterItem->getType() == "rocket" && iterItem->getX() < 1000 &&  iterItem->getY() < 1000 && iterItem->getX() > -1000 && iterItem->getY() > -1000){
            RocketGraphics3D *rocketGraphics3D = new RocketGraphics3D(iterItem, mScene);
            localRocket3D.append(rocketGraphics3D);
        }

        if(iterItem->getType() == "bomb" && iterItem->getX() < 1000 &&  iterItem->getY() < 1000 && iterItem->getX() > -1000 && iterItem->getY() > -1000){
            BombGraphics3D *bombGraphics3D = new BombGraphics3D(iterItem, mScene);
            localBomb3D.append(bombGraphics3D);
        }
    }



}
