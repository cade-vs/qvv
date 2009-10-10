
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QKeyEvent>

#include <qdebug.h>

#include <qvv.h>

QvvView::QvvView( QvvMainWindow* a_mw )
{
  mw         = a_mw;
  loaded     = 0;
  mouse_pan  = 0;
  rotation   = 0;
  opt_fit    = 1;
  opt_center = 0;
  pm         = NULL;

  move( 0, 0 );
}

QvvView::~QvvView()
{
  if( pm ) delete pm;
  close();
}

void QvvView::moverel( int dx, int dy )
{
  move( x() + dx, y() + dy );
};

void QvvView::slotCenter()
{
  if( ! opt_center ) return;
  // COPYRIGHT :)
  // taken as is from http://sector.ynet.sk/qt4-tutorial/my-first-qt-gui-application.html
  QDesktopWidget *desktop = QApplication::desktop();

  int screenWidth, width;
  int screenHeight, height;
  int x, y;
  QSize windowSize;

  screenWidth  = desktop->width(); // get width of screen
  screenHeight = desktop->height(); // get height of screen

  windowSize = size(); // size of our application window
  width      = windowSize.width();
  height     = windowSize.height();

  // little computations
  x = ( screenWidth  - width  ) / 2;
  y = ( screenHeight - height ) / 2;
  y -= 50;

  // move window to desired coordinates
  move( x, y );
};

void QvvView::load( QString fn )
{
  file_name = fn;
  reView( opt_fit ? -1 : 100 );
}

void QvvView::reView( int a_scale )
{
  // file_name = ":/images/Half_Face_by_uzorpatorica.jpg";

  QImage im;
  im.load( file_name );
  if( pm ) delete pm;
  pm = new QPixmap( im.width(), im.height() );
  *pm = QPixmap::fromImage( im );
  loaded = pm->isNull() ? 0 : 1;

  setWindowTitle( file_name );
  setWindowIcon( QIcon( *pm ) );
  slotCenter();
  resize( pm->width(), pm->height() );
  setMinimumSize( pm->width(), pm->height() );
  setMaximumSize( pm->width(), pm->height() );
  if( ! isVisible() ) show();

  qDebug() << "REVIEW: [" + file_name + "] loaded:"+QVariant( loaded ).toString() << " visible:" << isVisible() << pm->width() << pm->height();

/*
  loaded = 1;
  if ( strcmp( fname, "*logo*" ) == 0 )
    {
    img.loadFromData( (const uchar*)&human_dreams_data, sizeof(human_dreams_data) - 1 );
    }
  else if (!img.load( fname ))
    {
    char caps[1024];
    sprintf( caps, "LOAD FAILED: %s", fname );
    setCaption( caps );
    resize( width(), 100 );
    setMinimumSize( width(), 100 );
    setMaximumSize( width(), 100 );
    show();
    loaded = 0;
    return;
    }
  // fprintf( stderr, "qvv: load image: %s, %s\n", fname, loaded ? "ok" : "fail" );

  int w = img.width();
  int h = img.height();

  scale = a_scale;

  if ( scale == -1 )
    {
    if ( w <= DeskW && h <= DeskH )
      scale = 100;
    else
      scale = MIN( ((100 * DeskW) / w) , ((100 * DeskH) / h) );
    }
  else if ( scale == -2 )
    {
    w = app->desktop()->width();
    h = app->desktop()->height();
    scale = 100;
    }
  else
    {
    scale = (scale / 10) * 10;
    }

  if ( scale < MINSCALE ) scale = MINSCALE;
  if ( scale > MAXSCALE ) scale = MAXSCALE;

  w = (w * scale) / 100;
  h = (h * scale) / 100;

  if (w > DeskW || h > DeskH)
  {
      statusMsg("Invert Move");
      invertMove = -1;
  }
  else
  {
      statusMsg("Normal Move");
      invertMove = 1;
  }

  if ( scale != 100 )
    pm.fromImage( img.scaled( QSize( w, h ) ) );
  else if ( w != img.width() || h != img.height() )
    pm.fromImage( img.scaled( QSize( w, h ) ) );
  else
    pm.fromImage( img );


  if ( rotation )
    {
    QWMatrix m;
    m.rotate( double(rotation) );
    pm = pm.xForm(m);

    w = pm.width();
    h = pm.height();
    }

  resize( w, h );
  setMinimumSize( w, h );
  setMaximumSize( w, h );
  show();
  if ( optCenter )
    CenterView( this );
  else
    move( ViewX, ViewY );

  char caps[1024];
  sprintf( caps, "%dx%d@%d%% %s", img.width(), img.height(), scale, fname );
  setCaption( caps );
  update();

  // fprintf( stderr, "scale set to: %d%%\n", scale );
*/
};

void QvvView::paintEvent( QPaintEvent * pe )
{
  if( ! loaded ) return;
  QPainter painter( this );
  painter.drawPixmap( 0, 0, *pm );
};

void QvvView::keyPressEvent( QKeyEvent * e )
{
  if( e->modifiers() & Qt::ALT )
    {
    switch( e->key() )
      {
/*
      case Qt::Key_X   : QApplication::quit(); break;
*/
      default: QWidget::keyPressEvent( e );
      }
    }
  else
    {
    switch( e->key() )
      {
      case Qt::Key_Home  : move( 0, 0 ); break;

      case Qt::Key_Left         : moverel( -16, 0 ); break;
      case Qt::Key_Right        : moverel( +16, 0 ); break;
      case Qt::Key_Up           : moverel( 0, -16 ); break;
      case Qt::Key_Down         : moverel( 0, +16 ); break;

      case Qt::Key_Escape:
      case Qt::Key_Return:
      case Qt::Key_F4    : mw->closeView( this );
                           break;

      case Qt::Key_Insert       : opt_center = ! opt_center;
      case Qt::Key_Tab          : moverel( 0, 0 ); break;

      case Qt::Key_PageUp       :
      case Qt::Key_BracketLeft  : if (mw) mw->slotGoPrev(); break;
      case Qt::Key_Space        :
      case Qt::Key_PageDown     :
      case Qt::Key_BracketRight : if (mw) mw->slotGoNext(); break;

/*
      case Qt::Key_F3    : slotNewWindow(); break;
      case Qt::Key_Left  : slotGoUp(); break;
      case Qt::Key_Right :
      case Qt::Key_Return: Enter( tree->currentItem() ); break;

      //case Qt::Key_F6    : slotThumbs(); break;


      case Qt::Key_F1    : closeAll();
                           views.append( new qvvView( this ) );
                           views.first()->load( "*logo*" );
                           break;

      case Qt::Key_F5    : loadDir( cdir.absolutePath() ); break;

      case Qt::Key_BracketLeft  : slotGoPrev(); break;
      case Qt::Key_BracketRight : slotGoNext(); break;
      case Qt::Key_Backspace    :
      case Qt::Key_Escape       : closeAll(); break;
      case Qt::Key_Insert       : optCenter = !optCenter; break;
      case Qt::Key_Delete       : slotDelete(); break;
*/
/*
      default:
        if ( e->text().toAscii().at(0) && isalnum(e->text().toAscii().at(0)) )
          find( e->text().toAscii().at(0) );
        else
          switch( e->text().toAscii().at(0) )
            {
            case 13  : Enter( tree->currentItem() ); break;
            case '~' : goToDir( '~' ); break;
            case '`' : goToDir( '`' ); break;
            default: QMainWindow::keyPressEvent( e );
            }
========================================================
    switch( e->key() )
      {
      case ALT+Qt::Key_X        : app->quit(); break;
      case Qt::Key_Home         : move(  0,  0 ); break;

      case Qt::Key_Left         : moverel( -16*invertMove, 0 ); break;
      case Qt::Key_Right        : moverel( +16*invertMove, 0 ); break;
      case Qt::Key_Up           : moverel( 0, -16*invertMove ); break;
      case Qt::Key_Down         : moverel( 0, +16*invertMove ); break;

      case Qt::Key_Insert       : optCenter = !optCenter;
      case Qt::Key_Tab          : CenterView( this ); moverel( 0, 0 ); break;

      case Qt::Key_F1           : load( "*logo*" ); break;
      case Qt::Key_F4           :
      case Qt::Key_Escape       : if (mw) mw->views.removeOne( this );
                              close(); delete this; break;

      case Qt::Key_F5           : reView( scale ); break;

      case Qt::Key_PageUp       :
      case Qt::Key_BracketLeft  : if (mw) mw->slotGoPrev(); break;
      case Qt::Key_Space        :
      case Qt::Key_PageDown     :
      case Qt::Key_BracketRight : if (mw) mw->slotGoNext(); break;
      case Qt::Key_Delete       : if (mw)
                                {
                                mw->slotDelete();
                                mw->Enter( mw->vb->currentItem() );
                                }
                                break;
      default:
              switch( e->text().toAscii().at(0) )
              {
              case 13   : if (mw) mw->views.removeOne( this );
                          close(); delete this; break;
              case '+'  : reView( scale + 20 ); break;
              case '-'  : reView( scale - 20 ); break;
              case '*'  : reView(  -1 ); optFit = 1; break;
              case '/'  : reView( 100 ); optFit = 0; break;

              case '1'  : reView( 100 ); break;
              case '2'  : reView( 200 ); break;
              case '3'  : reView( 300 ); break;
              case '4'  : reView( 400 ); break;
              case '5'  : reView( 500 ); break;

        case '<'  :
        case ','  : rotation -= 90; reView( scale ); break;
        case '>'  :
        case '.'  : rotation += 90; reView( scale ); break;

              case 'Z'  : reView( -2 );
              case 'z'  : setDesktopBackground(); break;

              default:
                QWidget::keyPressEvent( e );
              break;
              }
      break;
      }
======================================================

*/
      default: QWidget::keyPressEvent( e );
      }
    }
};
