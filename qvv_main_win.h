/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

#ifndef _QVV_MAIN_WIN_H_
#define _QVV_MAIN_WIN_H_

#include <QMainWindow>
#include <QTreeWidget>
#include <QDir>
#include <QMenu>
#include <QString>

class QvvView;

class QvvTreeWidget : public QTreeWidget
{

     Q_OBJECT

 public:
     QvvTreeWidget( QWidget *parent );

     void findNext( QString str );

 protected:
     void keyPressEvent(QKeyEvent *event);
};

class QvvMainWindow : public QMainWindow
{

     Q_OBJECT

     QMenu           *menu;

     int rand_seeded;

 public:

     QvvTreeWidget   *tree;

     QList<QvvView*> views;

     QDir            cdir;

     int             last_sort_col;
     Qt::SortOrder   last_sort_ord;

     int             opt_thumbs;
     int             opt_dirs_only;

     QvvMainWindow();
     ~QvvMainWindow();

     void loadDir( QString path );
     void goToDir( int mode );

     void loadThumbs();

     void goPrevNext( int r );

     void closeView( QvvView *view );

     void sortColumn( int n );

 protected:
     void showEvent(QShowEvent *event);
     void keyPressEvent(QKeyEvent *event);

 public slots:
     void enter( QTreeWidgetItem *item );

     void actionTriggered(QAction *action);
     void switchLayoutDirection();

     void slotItemActivated( QTreeWidgetItem *item, int column );

     void slotNewWindow();
     void slotGoUp();
     void slotThumbs();
     void slotCreateThumbs();
     void slotJPEGThumbs();
     void slotChangeDir();
     void slotHomeDir();
     void slotReloadDir();
     void slotShowDirsOnly();

     void slotGoNext();
     void slotGoPrev();

     void slotSortColumn1();
     void slotSortColumn3();

     void slotRandomItem();

     void slotHelp();
     void slotAbout();

 private:
     void setupMenuBar();
};

#endif
