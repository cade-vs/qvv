/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QAction>

#include <qdebug.h>

#include "qvv.h"
#include "qvv_view.h"
#include "qvv_main_win.h"
#include "qvv_help.h"

QvvView::QvvView( QvvMainWindow* a_mw )
{
  mw         = a_mw;
  loaded     = 0;
  mouse_pan  = 0;
  rotation   = 0;
  opt_fit    = 1;
  opt_center = 0;
  scale      = 100;
  pm         = NULL;

  popup_menu = NULL;

  mouse_move = 0;

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

  int x, y;

  x = ( DeskW - width()  ) / 2;
  y = ( DeskH - height() ) / 2;
  y -= 50;

  move( x, y );
};

void QvvView::slotDelete()
{
  if (! getMainWindow(file_name) ) return;
  if( mw->deleteItems( 1 ) > 0 )
    mw->slotGoNext();
}

void QvvView::load( QString fname )
{
  file_name = fname;
  reView( opt_fit ? -1 : 100 );
}

void QvvView::reView( int a_scale )
{
  // file_name = ":/images/Half_Face_by_uzorpatorica.jpg";

  opt_fit = a_scale < 0 ? 1 : 0;

  scale = a_scale;
  if( scale <  0 ) scale = 100;
  if( scale == 0 ) scale =  20;

  QImage im;
  im.load( file_name );
  int ow = im.width();
  int oh = im.height();

  if( ( opt_fit && ( ow > DeskW || oh > DeskH - 50 ) ) || ( a_scale == -2 ) )
    {
    // scale to fit screen when fit required or stretch to fit desktop, keep aspect
    im = im.scaled( QSize( DeskW, DeskH - 50 ) , Qt::KeepAspectRatio, Qt::SmoothTransformation );
    scale = int(100*im.width()/ow);
    }
  else if( a_scale == -3 )
    {
    im = im.scaled( QSize( DeskW, DeskH ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    scale = int(100*im.width()/ow);
    }
  else if( scale != 100 )
    {
    im = im.scaled( QSize( int( ow * scale / 100 ), int( oh * scale / 100 ) - 50 ) , Qt::KeepAspectRatio, Qt::SmoothTransformation );
    }

  if( pm ) delete pm;
  pm = new QPixmap( im.width(), im.height() );
  *pm = QPixmap::fromImage( im );
  loaded = pm->isNull() ? 0 : 1;

  if( ! loaded )
    {
    im.load( ":/images/image-broken.png" );
    pm = new QPixmap( im.width(), im.height() );
    *pm = QPixmap::fromImage( im );
    loaded = pm->isNull() ? 0 : 1;
    }

  int w = pm->width();
  int h = pm->height();

  setWindowTitle( file_name + " @ " + QVariant(scale).toString() + "% ["+QVariant(w).toString()+"x"+QVariant(h).toString()+"]" );
  //setWindowIcon( QIcon( *pm ) );
  resize( w, h );
  setMinimumSize( w, h );
  setMaximumSize( w, h );
  slotCenter();
  if( ! isVisible() ) show();
  update();

  //qDebug() << "REVIEW: [" + file_name + "] loaded:"+QVariant( loaded ).toString() << " visible:" << isVisible() << pm->width() << pm->height();

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

QvvMainWindow* QvvView::getMainWindow( QString fn )
{
  if( mw )
    return mw;
  else
    {
    if( fn == "" ) return NULL;

    mw = new QvvMainWindow();
    QFileInfo fi( fn );
    mw->loadDir( fi.absolutePath() );
    mw->tree->findNext( fi.fileName() );
    mw->views.append( this );
    return mw;
    }
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
      default: QWidget::keyPressEvent( e ); break;
      }
    }
  else
    {
    switch( e->key() )
      {
      case Qt::Key_F1: slotHelp(); break;

      case Qt::Key_Home  : move( 0, 0 ); break;

      case Qt::Key_Left         : moverel( -16, 0 ); break;
      case Qt::Key_Right        : moverel( +16, 0 ); break;
      case Qt::Key_Up           : moverel( 0, -16 ); break;
      case Qt::Key_Down         : moverel( 0, +16 ); break;

      case Qt::Key_F3    : if (getMainWindow(file_name)) mw->show(); break;
      case Qt::Key_Escape:
      case Qt::Key_Return:
      case Qt::Key_F4    : if( mw )
                             mw->closeView( this );
                           else
                             {
                             close();
                             delete( this );
                             }
                           break;

      case Qt::Key_Insert       : opt_center = ! opt_center;
      case Qt::Key_Tab          : slotCenter(); break;

      case Qt::Key_PageUp       :
      case Qt::Key_BracketLeft  : if(getMainWindow(file_name))
                                    {
                                    mw->setActiveView( this );
                                    mw->slotGoPrev();
                                    }
                                  break;

      case Qt::Key_Space        :
      case Qt::Key_PageDown     :
      case Qt::Key_BracketRight : if (getMainWindow(file_name))
                                    {
                                    mw->setActiveView( this );
                                    mw->slotGoNext();
                                    }
                                  break;

      case Qt::Key_Asterisk     : if (getMainWindow(file_name))
                                    {
                                    mw->setActiveView( this );
                                    mw->slotRandomItem();
                                    }
                                  break;

      case Qt::Key_Delete       : slotDelete(); break;

      default:
            if( e->text() == "" )
              QWidget::keyPressEvent( e );
            else
              switch( e->text().toAscii().at(0) )
              {
              case '+'  : reView( scale + 10 ); break;
              case '-'  : reView( scale - 10 ); break;
              //case '/'  : opt_fit = 0; reView( 100 ); break;

              case '0'  : opt_fit = 1; reView(  -1 ); break;
              case '1'  : reView( 100 ); break;
              case '2'  : reView( 200 ); break;
              case '3'  : reView( 300 ); break;
              case '4'  : reView( 400 ); break;
              case '5'  : reView( 500 ); break;

              /*
              case '<'  :
              case ','  : rotation -= 90; reView( scale ); break;
              case '>'  :
              case '.'  : rotation += 90; reView( scale ); break;
              */

              case 'z'  : reView( -2 ); break;
              case 'x'  : reView( -3 ); break;
              // case 'z'  : setDesktopBackground(); break;

              default   : QWidget::keyPressEvent( e ); break;
              }
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
      break;
      }
======================================================

      default: QWidget::keyPressEvent( e );
*/
      }
    }
};

void QvvView::popup( QPoint pos )
{
  if( popup_menu ) delete popup_menu;
  popup_menu = new QMenu( QString( "Image operations" ), this );

  QAction *action;
  //action = popup_menu->addAction( tr("&Center image on the screen"), this, SLOT(slotCenter()), Qt::Key_Insert );

  action = popup_menu->addAction( tr("&Fit image to screen if larger"), this, SLOT(slotViewFit()), '0' );
  action = popup_menu->addAction( tr("Show image at &100% (full size)"), this, SLOT(slotView100()), '1' );
  action = popup_menu->addAction( tr("Show image at &200%"), this, SLOT(slotView200()), '2' );
  action = popup_menu->addAction( tr("Show image at &300%"), this, SLOT(slotView300()), '3' );
  action = popup_menu->addAction( tr("Show image at &400%"), this, SLOT(slotView400()), '4' );
  action = popup_menu->addAction( tr("Show image at &500%"), this, SLOT(slotView500()), '5' );

  action = popup_menu->addAction( tr("Move to 'home' of the screen"), this, SLOT(slotViewHome()), Qt::Key_Home );
  if( opt_center )
    {
    action = popup_menu->addAction( tr("Disable &Center image"), this, SLOT(slotViewNoCenter()), Qt::Key_Insert );
    }
  else
    {
    action = popup_menu->addAction( tr("Enable &Center image"), this, SLOT(slotViewCenter()), Qt::Key_Insert );
    }

  popup_menu->popup( pos );
}

void QvvView::mousePressEvent ( QMouseEvent * e )
{

  if( e->button() == Qt::RightButton )
    popup( mapToGlobal( e->pos() ) );
  else
    {
    mouse_move = 1;
    mouse_move_mo = mapToGlobal( e->pos() );
    mouse_move_wo = this->pos();
    setCursor( Qt::OpenHandCursor );
    }

};

void QvvView::mouseReleaseEvent ( QMouseEvent * e )
{
  if( mouse_move )
    {
    mouse_move = 0;
    setCursor( Qt::ArrowCursor );
    }
};

void QvvView::mouseMoveEvent ( QMouseEvent * e )
{
  if( mouse_move )
    {
    QPoint diff = mapToGlobal( e->pos() ) - mouse_move_mo;
    QPoint newpos = mouse_move_wo + diff;
    move( newpos.x(), newpos.y() );
    }
};


void QvvView::wheelEvent ( QWheelEvent * e )
{
  if( e->delta() > 0 )
    {
    if (getMainWindow(file_name)) mw->slotGoPrev();
    }
  else
    {
    if (getMainWindow(file_name)) mw->slotGoNext();
    }
  e->accept();
};

void QvvView::slotHelp()
{
  display_help();
};
