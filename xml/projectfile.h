#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QObject>
#include "Types.h"

class QDomDocument;
class QDomElement;

class ProjectFile : public QObject
{
    Q_OBJECT
public:
    explicit ProjectFile(QObject *parent = 0);

    bool Load(QString filename);
    bool GetSessionList(SesList &sl);
    void AddType(QString name);
    void AddSession(Session &ses);
    void Save();

signals:

public slots:

private:
    void init();
    void makeSesID(QString &id);
    void addParam(QDomElement &e, Session &ses);
    void getSession(QDomElement &sesEle, Session &ses);
    void getParam(QDomElement &parEle, Session &ses);

private:
    QDomDocument *doc;
    QDomElement *root;
};

#endif // PROJECTFILE_H
