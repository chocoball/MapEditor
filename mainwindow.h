#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QModelIndex>
#include <QStandardItem>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void slot_fileOpen() ;
	void slot_fileSave() ;
	void slot_fileSaveAs() ;

	void slot_splitterMoveImage(int, int) ;
	void slot_splitterMoveMap(int, int) ;

	void slot_changeImageGridW(int val) ;
	void slot_changeImageGridH(int val) ;
	void slot_changeMapGridW(int val) ;
	void slot_changeMapGridH(int val) ;

	void slot_clickCheckBoxUnit(bool val) ;
	void slot_clickCheckBoxThrough(bool val) ;

	void slot_changeSelectGridRect() ;

	void slot_clickPushAdd() ;
	void slot_clickPushDel() ;

	void slot_treeViewClicked(QModelIndex index) ;

	void slot_addMap() ;
	void slot_addTreasure() ;

	void slot_changeTreeItem(QStandardItem*) ;
signals:
	void sig_keyPress(QKeyEvent *) ;
	void sig_keyRelease(QKeyEvent *) ;

protected:
	void closeEvent(QCloseEvent *) ;
	void resizeEvent(QResizeEvent *) ;

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	void fileOpen(QString &fileName) ;
	void fileWrite(QString &fileName) ;

	void restoreSettings() ;
	void setSpaceSize() ;

private:
    Ui::MainWindow	*ui;

	QString			m_strSaveFileName ;
//	QSplitter		*m_pSplitterImage ;
	QSplitter		*m_pSplitterMap ;

	QSize			m_frameImageSpace ;
	QSize			m_frameMapSpace ;
	QSize			m_windowSpace ;
	QSize			m_frameTreeSpace ;

//	QAction			*m_pActAddMap ;
//	QAction			*m_pActAddTreasure ;
};

#endif // MAINWINDOW_H
