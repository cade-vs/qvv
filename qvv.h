
#ifndef _QVV_H_
#define _QVV_H_

#include <QMainWindow>
#include <QTreeWidget>
#include <QDir>
#include <QString>
#include <QDesktopWidget>

const QString ITEM_TYPE_DIR( "[DIR]" );

extern QDesktopWidget *Desktop;
extern int DeskW;
extern int DeskH;

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
    QvvMainWindow* getMainWindow() { return (QvvMainWindow*)parent(); };

    void moverel( int dx, int dy );

  public slots:
    void slotCenter();

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

class QvvMainWindow : public QMainWindow
{

     Q_OBJECT

     QTreeWidget     *tree;
     QMenu           *menu;
     QSignalMapper   *mapper;

 public:

     QList<QvvView*> views;

     QDir            cdir;

     int             opt_thumbs;
     int             opt_dirs_only;

     QvvMainWindow();
     ~QvvMainWindow();

     void loadDir( QString path );
     void goToDir( int mode );

     void loadThumbs();

     void goPrevNext( int r );

     void closeView( QvvView *view );

 protected:
     void showEvent(QShowEvent *event);
     void keyPressEvent(QKeyEvent *event);

 public slots:
     void enter( QTreeWidgetItem *item );

     void actionTriggered(QAction *action);
     void switchLayoutDirection();

     void slotNewWindow();
     void slotGoUp();
     void slotThumbs();
     void slotChangeDir();
     void slotHomeDir();
     void slotReloadDir();
     void slotShowDirsOnly();

     void slotGoNext();
     void slotGoPrev();

 private:
     void setupMenuBar();
};


#endif