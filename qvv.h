
#ifndef _QVV_H_
#define _QVV_H_

#include <QMainWindow>
#include <QTreeWidget>
#include <QDir>
#include <QString>

const QString ITEM_TYPE_DIR( "[DIR]" );

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

     int           opt_thumbs;

 public:
     QvvMainWindow();

     void loadDir( QString path );
     void goToDir( int mode );
     void Enter( QTreeWidgetItem *item );

     void loadThumbs();

 protected:
     void showEvent(QShowEvent *event);
     void keyPressEvent(QKeyEvent *event);

 public slots:
     void actionTriggered(QAction *action);
     void switchLayoutDirection();

     void slotNewWindow();
     void slotGoUp();
     void slotThumbs();

 private:
     void setupMenuBar();
};


#endif