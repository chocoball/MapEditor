#include "ccomboboxdelegate.h"
#include <QComboBox>

CComboBoxDelegate::CComboBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *CComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QComboBox *p = new QComboBox(parent) ;
	p->addItems(m_dispList) ;
	return p ;
}

void CComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	int value = index.model()->data(index, Qt::EditRole).toInt() ;
	QComboBox *p = static_cast<QComboBox *>(editor) ;
	p->setCurrentIndex(value) ;
}

void CComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *p = static_cast<QComboBox *>(editor) ;
	model->setData(index, p->currentIndex(), Qt::EditRole) ;
}
