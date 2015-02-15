/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

#ifndef _QVV_VIEW_H_
#define _QVV_VIEW_H_

#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QMenu>
#include <QPoint>

class QvvMainWindow;

class QvvView : public QWidget
{
    Q_OBJECT

    int xpos;
    int ypos;
    QvvMainWindow *mw;

    QPixmap *pm;

    QMenu *popup_menu;

    QString file_name;

    int opt_fit;
    int opt_center;
    int loaded;
    int scale; // in %'s

    int mouse_pan;
    int mouse_pan_x;
    int mouse_pan_y;

    int rotation;

    int mouse_move;
    QPoint mouse_move_mo; // mouse origin
    QPoint mouse_move_wo; // window origin

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
    void popup( QPoint pos );

    QString getFileName() { return file_name; };

  public slots:
    void slotCenter();

    void slotHelp();

    void slotViewFit() { opt_fit = 1; reView(  -1 ); };
    void slotView100() { reView( 100 ); };
    void slotView200() { reView( 200 ); };
    void slotView300() { reView( 300 ); };
    void slotView400() { reView( 400 ); };
    void slotView500() { reView( 500 ); };
    void slotViewCenter() { opt_center = 1; reView( -1 ); }
    void slotViewNoCenter() { opt_center = 0; reView( -1 ); }
    void slotViewHome() { opt_center = 0; move( 0, 0 ); reView( -1 ); }

    void slotDelete();

  protected:
    void keyPressEvent ( QKeyEvent * e );

    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );

    void paintEvent( QPaintEvent * e );
    void wheelEvent ( QWheelEvent * e );

/*
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );
    void closeEvent( QCloseEvent * e ) { ViewX = xpos; ViewY = ypos; QWidget::closeEvent( e ); };
*/
};

#endif
