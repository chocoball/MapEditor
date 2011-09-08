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

	void slot_clickedListViewMap(QModelIndex) ;
	void slot_clickedListViewTreasure(QModelIndex) ;
	void slot_clickedListViewPoint(QModelIndex) ;

	void slot_tabChanged(int index) ;

	void slot_pushAddTreasure() ;
	void slot_pushDelTreasure() ;

	void slot_pushAddPoint() ;
	void slot_pushDelPoint() ;

	void slot_clickAddStartPoint() ;
	void slot_clickAddEndPoint() ;

	void slot_changeDataModified(int index) ;

	void slot_clickViewSquare() ;
	void slot_clickViewQuarter() ;

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

	void dataInit() ;

	bool checkSaveFileChanged() ;

private:
    Ui::MainWindow	*ui;

	QString			m_strSaveFileName ;
	QSplitter		*m_pSplitterMap ;

	QSize			m_frameImageSpace ;
	QSize			m_frameMapSpace ;
	QSize			m_windowSpace ;
	QSize			m_frameTreeSpace ;

	QAction			*m_pActAddStartPoint ;
	QAction			*m_pActAddEndPoint ;

	QAction			*m_pActUndo ;
	QAction			*m_pActRedo ;

	int				m_undoIndex ;
};

#endif // MAINWINDOW_H
