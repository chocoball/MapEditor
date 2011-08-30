#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

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

protected:
	void closeEvent(QCloseEvent *) ;
	void resizeEvent(QResizeEvent *) ;

private:
	void fileOpen(QString &fileName) ;
	void fileWrite(QString &fileName) ;

	void restoreSettings() ;
	void setSpaceSize() ;

private:
    Ui::MainWindow	*ui;

	QString			m_strSaveFileName ;
	QSplitter		*m_pSplitterImage ;
	QSplitter		*m_pSplitterMap ;

	QSize			m_frameImageSpace, m_frameMapSpace, m_windowSpace ;
};

#endif // MAINWINDOW_H
