#include "pattimagedelegate.h"
#include <QtGui>
#include <QAbstractTableModel>

pattImageDelegate::pattImageDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
    enableBkChange = true;
}

void pattImageDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QPixmap dispaly;
    QAbstractItemModel *model = (QAbstractItemModel*)index.model();

    if (index != currentIndex)
    {
        // 将非当前单元格画正色
        dispaly = index.model()->data(index, Qt::DisplayRole).value< QPixmap >();
        painter->drawPixmap(option.rect, dispaly);
    }
    else
    {
        // 切换正反色
        dispaly = index.model()->data(index, Qt::UserRole).value< QPixmap >();
        painter->drawPixmap(option.rect, dispaly);
        model->setData(index, true, Qt::UserRole + 1);
    }
//    static int cnt=0;
//    qDebug("XXXXX %d\n", cnt ++);
}

QSize pattImageDelegate::sizeHint(const QStyleOptionViewItem & /* option */,
                              const QModelIndex & /* index */) const
{
    return QSize(20, 20);
}

QWidget* pattImageDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &,
                                    const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regxp("[0-9a-fA-F]");

    editor->setValidator(new QRegExpValidator(regxp, parent));
    editor->setMaxLength(1);
    //enableBkChange = false;

    return editor;
}

void pattImageDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    lineEdit->setText(text);
}

void pattImageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    model->setData(index, QVariant(lineEdit->text()), Qt::EditRole);
    //enableBkChange = false;
}

void pattImageDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &) const

{
    editor->setGeometry(option.rect);
}
