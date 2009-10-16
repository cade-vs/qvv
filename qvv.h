
#ifndef _QVV_H_
#define _QVV_H_

#include <QMainWindow>
#include <QTreeWidget>
#include <QDir>
#include <QString>
#include <QDesktopWidget>
#include <QTextBrowser>

const QString ITEM_TYPE_DIR( "[DIR]" );

extern QDesktopWidget *Desktop;
extern int DeskW;
extern int DeskH;

extern QTextBrowser *help_browser;

void display_help();

class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)
QT_FORWARD_DECLARE_CLASS(QvvMainWindow)

class QvvView : public QWidget
{
    Q_OBJECT

    int xpos;
    int ypos;
    QvvMainWindow *mw;

    QPixmap *pm;

    QString file_name;

    int opt_fit;
    int opt_center;
    int loaded;
    int scale; // in %'s

    int mouse_pan;
    int mouse_pan_x;
    int mouse_pan_y;

    int rotation;

  public:

    QvvView( QvvMainWindow* a_mw );
    ~QvvView();

    void load( QString fn );
    void reView( int a_scale );
/*
    void moverel( int dx, int dy );
    void moveabs( int ax, int ay );

    void statusMsg( const char* msg );

*/
    QvvMainWindow* getMainWindow( QString fn );

    void moverel( int dx, int dy );

  public slots:
    void slotCenter();

    void slotHelp();

  protected:
    void keyPressEvent ( QKeyEvent * e );
    void paintEvent( QPaintEvent * e );
/*
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );
    void closeEvent( QCloseEvent * e ) { ViewX = xpos; ViewY = ypos; QWidget::closeEvent( e ); };
*/
};

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
     QSignalMapper   *mapper;

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

 private:
     void setupMenuBar();
};


#endif