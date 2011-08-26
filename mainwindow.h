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

protected:
	void closeEvent(QCloseEvent *) ;

private:
	void fileOpen(QString &fileName) ;
	void fileWrite(QString &fileName) ;

	void restoreSettings() ;

private:
    Ui::MainWindow	*ui;

	QString			m_strSaveFileName ;
	QSplitter		*m_pSplitterImage ;
	QSplitter		*m_pSplitterMap ;
};

#endif // MAINWINDOW_H
