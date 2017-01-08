#ifndef NEWSESSION_H
#define NEWSESSION_H

#include <QDialog>
#include <QMap>

namespace Ui {
class NewSession;
}

typedef QMap <QString, QString> NewSesSetting;

class NewSession : public QDialog
{
    Q_OBJECT

public:
    explicit NewSession(QWidget *parent = 0);
    ~NewSession();

    void getSetting(NewSesSetting &s);

private:
    Ui::NewSession *ui;
    NewSesSetting nss;
};

#endif // NEWSESSION_H
