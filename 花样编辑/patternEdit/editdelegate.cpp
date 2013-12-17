#include "editdelegate.h"

EditDelegate::EditDelegate()
{
}

QWidget* EditDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &,
                                    const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regxp("[0-9a-fA-F]");

    editor->setValidator(new QRegExpValidator(regxp, parent));
    editor->setMaxLength(1);

    return editor;
}

void EditDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    lineEdit->setText(text);
}

void EditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString text = lineEdit->text();
    QStandardItemModel *m = static_cast<QStandardItemModel*>(model);

    m->itemFromIndex(index)->setBackground(QBrush(QColor(66, 88, 66)));

    model->setData(index, text, Qt::EditRole);
}

void EditDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &) const

{
    editor->setGeometry(option.rect);
}
