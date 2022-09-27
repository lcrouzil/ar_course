#include "Checkpoint.h"

Checkpoint::Checkpoint(QObject *parent)
    : QObject{parent}
{

}

void Checkpoint::deserialize(const QJsonObject &jsonObject) {
    id = jsonObject["id"].toInt();
    x = jsonObject["x"].toInt();
    y = jsonObject["y"].toInt();
}

QJsonObject Checkpoint::toJson() {
    QJsonObject jObject;

    /* set key -> value of json */
    jObject["id"] = this->id;
    jObject["x"] = this->x;
    jObject["y"] = this->y;

    return jObject;

}
