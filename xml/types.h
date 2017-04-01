#ifndef TYPES_H
#define TYPES_H

#include <QMap>
#include <QList>

typedef QMap<QString, QString> SesParam;

typedef struct
{
    QString type;
    QString name;
    QString id;

    SesParam param;
}Session;

typedef QList<Session> SesList;

#endif // TYPES_H
