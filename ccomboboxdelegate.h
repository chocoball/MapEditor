#ifndef CCOMBOBOXDELEGATE_H
#define CCOMBOBOXDELEGATE_H

#include <QItemDelegate>

class CComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CComboBoxDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
	void setEditorData(QWidget *editor, const QModelIndex &index) const ;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const ;

	void setDisplayList(QStringList strList) { m_dispList = strList ; }

signals:

public slots:

private:
	QStringList		m_dispList ;
};

#endif // CCOMBOBOXDELEGATE_H
