
#ifndef _QVV_H_
#define _QVV_H_

#include <QMainWindow>
#include <QTreeWidget>

class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

class QvvMainWindow : public QMainWindow
{

     Q_OBJECT

     QTreeWidget   *tree;
     QMenu         *menu;
     QSignalMapper *mapper;

     QDir          cdir;

 public:
     QvvMainWindow();

     void loadDir( const char* path );
     void goToDir( int mode );
     void Enter( QTreeWidgetItem * );

 protected:
     void showEvent(QShowEvent *event);
     void keyPressEvent(QKeyEvent *event);

 public slots:
     void actionTriggered(QAction *action);
     void saveLayout();
     void loadLayout();
     void switchLayoutDirection();
     void slotNewWindow();

 private:
     void setupMenuBar();
};


#endif