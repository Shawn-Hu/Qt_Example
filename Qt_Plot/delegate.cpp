#include "delegate.h"

Delegate::Delegate(QObject *parent) :
    QItemDelegate (parent)
{
}


QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *edit=new QLineEdit(parent);
    return edit;

}
void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::EditRole).toString();
    QLineEdit *lineditor = static_cast<QLineEdit*>(editor);
    lineditor->setText(value);

}
void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineditor = static_cast<QLineEdit*>(editor);
    QString value = lineditor->text();
    model->setData(index,value , Qt::EditRole);
}
void Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
