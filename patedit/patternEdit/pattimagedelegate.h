#ifndef PATTIMAGEDELEGATE_H
#define PATTIMAGEDELEGATE_H

#include <QAbstractItemDelegate>
#include <QLineEdit>
#include <QLabel>
#include <QRect>

class pattImageDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit pattImageDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index ) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

signals:

public slots:

public:
    QModelIndex currentIndex;
};

#endif // PATTIMAGEDELEGATE_H
