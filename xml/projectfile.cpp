#include "projectfile.h"

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

ProjectFile::ProjectFile(QObject *parent) : QObject(parent)
{
    init();
}

bool ProjectFile::Load(QString filename)
{
    bool ret = true;
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!doc->setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        return false;
    }

    file.close();

    return ret;
}

void ProjectFile::getSession(QDomElement &sesEle, Session &ses)
{
    QDomNodeList list;

    ses.id = sesEle.attribute("ID");

    list = sesEle.elementsByTagName("param");
    for (int i = 0; i < list.size(); i ++)
    {
        QDomElement paramEle = list.item(i).toElement();

        getParam(paramEle, ses);
    }
}

void ProjectFile::getParam(QDomElement &parEle, Session &ses)
{
    QDomNamedNodeMap nmap;
    QString key, value;

    nmap = parEle.attributes();

    for (int i = 0; i < nmap.size(); i ++)
    {
        key = nmap.item(i).nodeName();
        value = nmap.item(i).nodeValue();

        ses.param[key] = value;
    }
}

bool ProjectFile::GetSessionList(SesList &sl)
{
    bool ret = false;
    QDomElement r;

    *root = doc->documentElement();

    QDomNodeList list;

    list = doc->elementsByTagName("type");
    for (int i = 0; i < list.size(); i ++)
    {
        QDomNode item = list.item(i);
        QDomElement type;
        Session ses;
        QDomNodeList seslist;

        type = item.toElement();
        seslist = type.elementsByTagName("session");

        ses.type = type.attribute("ID");

        for (int n = 0; n < seslist.size(); n ++)
        {
            QDomElement sesEle = seslist.item(n).toElement();

            ses.id = sesEle.attribute("ID");
            ses.name = sesEle.attribute("name");

            getSession(sesEle, ses);

            sl.append(ses);
        }
    }

    return ret;
}

void ProjectFile::AddType(QString name)
{
    QDomElement type;

    type = doc->createElement("type");
    type.setAttribute("ID", name);
    root->appendChild(type);
}

void ProjectFile::AddSession(Session &ses)
{
    QDomNodeList list;

    list = doc->elementsByTagName("type");

    for (int i = 0; i < list.size(); i ++)
    {
        QDomNode item = list.item(i);
        QDomElement type;

        type = item.toElement();
        if (type.attribute("ID") == ses.type)
        {
            QDomElement e;

            makeSesID(ses.id);
            e = doc->createElement("session");
            e.setAttribute("ID", ses.id);
            e.setAttribute("name", ses.name);
            type.appendChild(e);

            addParam(e, ses);
            break;
        }
    }
}

void ProjectFile::addParam(QDomElement &e, Session &ses)
{
    QDomElement child;
    QList<QString> keys;
    int cnt;

    keys = ses.param.keys();
    cnt = keys.size();

    child = doc->createElement("param");
    for (int i = 0; i < cnt; i ++)
    {
        QString key = keys[i];

        child.setAttribute(key, ses.param[key]);
    }
    e.appendChild(child);
}

void ProjectFile::init()
{
    QDomProcessingInstruction instruction;

    doc = new QDomDocument;
    root = new QDomElement;

    instruction = doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    *root = doc->createElement("Project");

    doc->appendChild(instruction);
    doc->appendChild(*root);
}

void ProjectFile::makeSesID(QString &id)
{
    QDateTime dt;

    dt = dt.currentDateTime();
    id = dt.toString("dd-mm-yyyy hh:mm:ss.zzz");
}

void ProjectFile::Save()
{
    QFile file;

    file.setFileName("project.xml");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out(&file);

    doc->save(out, 4);
}
