/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" 1999-2009
 * http://soul.datamax.bg/~cade  <cade@biscom.net>  <cade@datamax.bg>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include <qobject.h>
#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qimage.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qevent.h>
#include <qlist.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qstring.h>
#include <qmenu.h>
#include <qinputdialog.h>
#include <qmenubar.h>
#include <qfont.h>
#include <qprogressdialog.h>
#include <qmenubar.h>
#include <qtoolbutton.h>

#include <fnmatch.h>

#include "qvv.h"

#define VER	"4.00"

#include "human_dreams_low.c"
#include "ft_folder.xpm"
#include "ft_image.xpm"
#include "ft_folder_big.xpm"
#include "ft_image_big.xpm"
#include "pattern.xpm"
#include "broken_img.xpm"

#include "stock_colorselector_24.xpm"
#include "stock_home_24.xpm"
#include "stock_index_24.xpm"
#include "stock_left_arrow_24.xpm"
#include "stock_new_24.xpm"
#include "stock_open_24.xpm"
#include "stock_refresh_24.xpm"
#include "stock_right_arrow_24.xpm"
#include "stock_trash_24.xpm"
#include "stock_up_arrow_24.xpm"


#define THUMB_W 80
#define THUMB_H 60

QApplication *app;

int ViewX = 0; // view x pos
int ViewY = 0; // view y pos

int DeskW = 0;   // actual desktop width
int DeskH = 0;   // actual desktop height

int optCenter = 0;  // pic view position on screen
int optFit = 1; // fit to screen
int invertMove = 1;

QString filters;

QPixmap *pm_folder;
QPixmap *pm_image;
QPixmap *pm_pattern;
QPixmap *pm_folder_big;
QPixmap *pm_image_big;
QPixmap *pm_broken_img;

int GetEnvInt( const char* s )
{
  int res;
  if (!getenv(s)) return -1;
  return atoi(getenv(s));
}

void CenterView( qvvView* w, int a_center = -1 )
{
  int x = ( DeskW - w->width () ) / 2;
  int y = ( DeskH - w->height() ) / 2;
  w->moveabs( x, y );
}

int load_xvpic( const char* file_name, QPixmap *pm )
{
int b,w,h,z;
char line[128];
unsigned char *buff;
unsigned char *buffp;

FILE *fi = fopen( file_name,"r" );
if ( !fi ) return 1;
fgets( line, 127, fi ); /* line1: magic `P7 332' or whatever */
while( fgets( line, 127, fi ) )
  if ( strncmp( line, "#END_OF_COMMENTS", 16 ) == 0 ) /* first 16 is enough... */
    break;
fscanf(fi,"%d %d %d\n",&w,&h,&b);
if ( feof( fi ) || w <= 0 || h <= 0 )
  {
  fclose( fi );
  return 2;
  }

buff = (unsigned char*)malloc( h * w * 4 ); /* RGB buffer */
buffp = buff;
memset( buff, 0, h * w * 4 );

z = w * h;
while( z-- )
  {
    b = fgetc( fi );
    if ( b == EOF ) break;
    *buffp++ = 64 * (  3 & b);
    *buffp++ = 32 * ( 28 & b) >> 2;
    *buffp++ = 32 * (224 & b) >> 5;
    buffp++;
  }
fclose( fi );
QImage im( buff, w, h, 32, QImage::Format_RGB32 );
pm->fromImage( im );
free( buff );
return 0;
}

int save_xvpic( const char* file_name, QPixmap *pm )
{
/*
FILE *fo = fopen( file_name,"w" );
if ( !fo ) return 1;

fprintf( fo, "P7 332\n");
fprintf( fo, "#Created by QVV Image Viewer" VER "\n" );
fprintf( fo, "#END_OF_COMMENTS\n");
fprintf( fo, "%d %d 255\n",x,y);
getchar();
for (i=0;i<x*y;i++) {
scanf("%d",&b);
b=(b*255)/dummy;
out=b&224;
out=out|((b&224)/8);
out=out|((b&192)/64);
fputc( fo, out);
*/
return 0;
}

int load_xvpic( const char* path, const char* file_name, QPixmap *pm )
{
char tfname[512];
sprintf( tfname, "%s/.xvpics/%s", path, file_name );
// printf( "loading xvpic: %s\n", tfname );
return load_xvpic( tfname, pm );
}

int make_xvpic( const char* file_name, QPixmap *pm )
{
  QImage img;
  if (!img.load( file_name )) return 1;

  int w = img.width();
  int h = img.height();

  if ( w > h )
    {
    h = ( THUMB_W * h ) / w;
    w = THUMB_W;
    }
  else
    {
    w = ( THUMB_H * w ) / h;
    h = THUMB_H;
    }

  pm->fromImage( img.scaled( QSize( w, h ) ) );
  return 0;
}

int make_xvpic( const char* path, const char* file_name, QPixmap *pm )
{
  QString fn;
  fn = fn + path + "/" + file_name;
  return make_xvpic( fn, pm );
}

int load_thumb( const char* path, const char* file_name, QPixmap *pm, const char* ext )
{
  QString th;
  th = th + path + "/.thumbnails/" + file_name + "." + ext;
  QImage img;
  if (!img.load( th )) return 1;
  pm->fromImage( img );
  return 0;
}

int get_xvpic( const char* path, const char* file_name, QPixmap *pm )
{
if ( load_thumb( path, file_name, pm, "png" ) == 0 ) return 0;
if ( load_thumb( path, file_name, pm, "png" ) == 0 ) return 0;
if ( load_xvpic( path, file_name, pm ) == 0 ) return 0;
if ( make_xvpic( path, file_name, pm ) == 0 ) return 0;
return 1;
}

int qfile_copy( const char* src_file, const char* dst_file, int move = 0 )
{
  const int bufSize = 1024*1024;

  QFile s( src_file );
  QFile d( dst_file );
  char *buf;
  int len;

  int e = 0;

  s.open( QIODevice::ReadOnly  );
  d.open( QIODevice::WriteOnly );
  buf = new char[bufSize];
  len = s.read( buf, bufSize);
  do
    {
    if( d.write(buf, len) != len )
      {
      e = 1;
      break;
      };
    int len1 = s.read(buf, len);
    if ( len1 < 0 )
      {
      e = 2;
      break;
      }
    else
      {
      len = len1;
      }
    } while ( len > 0 );
  s.close();
  d.close();
  delete[] buf;

  if ( e )
    {
    d.remove( dst_file );
    return e;
    }

  if ( move )
    s.remove( src_file );
  return 0;
}

/****************************************************************************
**
**
**
****************************************************************************/


/****************************************************************************
**
**
**
****************************************************************************/

/****************************************************************************
**
**
**
****************************************************************************/

  qvvMainWindow::qvvMainWindow( const char *startdir )
    : QMainWindow()
  {
    optThumbs = 0;

    vb = new qvvBrowser( this );
    setCentralWidget( vb );

    setGeometry( 200, 250, 620, 300 );

    sb = statusBar();
    sb->showMessage( "QVV Started" );

    tb = new QToolBar( this );
    tb->setSizePolicy( QSizePolicy::ExpandFlag );

    QToolButton *bu;

    bu = new QToolButton( tb );
    bu->setIcon( QIcon( QPixmap( stock_new_24 ) ) );
    bu->setText( "New window (F3)" );


/*
    new QToolButton( QIconSet( QPixmap( stock_new_24 ) ), "New window (F3)", QString::null,
            this, SLOT( slotNewWindow() ), tb, "new window" );
    new QToolButton( QIconSet( QPixmap( stock_open_24 ) ), "Go to directory (``)", QString::null,
            this, SLOT( slotGoToDir() ), tb, "go to dir" );
    new QToolButton( QPixmap( stock_up_arrow_24 ), "Go to parent directory (LeftArrow)", QString::null,
            this, SLOT( slotGoUp() ), tb, "go up" );
    new QToolButton( QPixmap( stock_refresh_24 ), "Reload directory (F5)", QString::null,
            this, SLOT( slotReload() ), tb, "reload" );
    new QToolButton( QPixmap( stock_left_arrow_24 ), "Previous image ([)", QString::null,
            this, SLOT( slotGoPrev() ), tb, "previous image" );
    new QToolButton( QPixmap( stock_right_arrow_24 ), "Next image (])", QString::null,
            this, SLOT( slotGoNext() ), tb, "next image" );
    new QToolButton( QPixmap( stock_home_24 ), "Go to home directory (~)", QString::null,
            this, SLOT( slotGoHome() ), tb, "go home" );
    new QToolButton( QPixmap( stock_colorselector_24 ), "Toggle thumbnails (F6)", QString::null,
            this, SLOT( slotThumbs() ), tb, "thumbs" );
    new QToolButton( QPixmap( stock_trash_24 ), "Delete file(s) (Del)", QString::null,
            this, SLOT( slotDelete() ), tb, "delete" );
*/
    connect( vb, SIGNAL(returnPressed( QTreeWidgetItem * )), this, SLOT(slotEnter( QTreeWidgetItem * )) );
    connect( vb, SIGNAL(doubleClicked( QTreeWidgetItem * )), this, SLOT(slotEnter( QTreeWidgetItem * )) );
    connect( vb, SIGNAL(rightButtonClicked( QTreeWidgetItem *, const QPoint &, int )), this, SLOT(slotRClick( QTreeWidgetItem *, const QPoint &, int )));

    loadDir( startdir );

    show();
  }

  qvvMainWindow::~qvvMainWindow()
  {
    closeAll();
    delete vb;
  }

  void qvvMainWindow::loadDir( const char* path )
  {
    cdir.cd( path );

    QString str = "QVV: ";
    if ( str != "/" ) str += cdir.absolutePath() + "/";
    setCaption( str );

    QTreeWidgetItem *lvi = vb->currentItem();
    QString save_pos;
    if( lvi )
      save_pos = lvi->text( 1 );

    while( lvi = vb->topLevelItem() )
      delete lvi;

    cdir.setSorting( QDir::DirsFirst | QDir::Name );
    const QFileInfoList *fil = cdir.entryInfoList( "*" );

    long count = 0;
    long size = 0;

    QFileInfo *fi = fil.begin();
    while( fi != fil.End() )
      {
      if ( strcmp( ".", fi->fileName()) == 0 || strcmp("..", fi->fileName()) == 0 )
        {
        fi++;
        continue;
        }

      QString ext = fi->extension();
      int lastdot = ext.lastIndexOf('.');
      if ( lastdot != -1 )
        ext = ext.mid( lastdot+1, ext.length() );
      ext = ext.lower();
      if ( fi->isFile() && filters.indexOf( "." + ext + "." ) == -1 )
        {
        fi++;
        continue;
        }

      char sizestr[64];
      sprintf( sizestr, "%10d", fi->size());

      char timestr[64];
      QDateTime mod = fi->lastModified();
      QDate     modd = mod.date();
      QTime     modt = mod.time();

      sprintf( timestr, "%4d.%02d.%02d %02d:%02d:%02d",
               modd.year(), modd.month(), modd.day(),
               modt.hour(), modt.minute(), modt.second()
               );

      QTreeWidgetItem *lvi = new QTreeWidgetItem( vb, fi->isDir() ? "[DIR]" : "(IMG)", fi->fileName(), sizestr, timestr );
      //QTreeWidgetItem *lvi = new QTreeWidgetItem( vb, "", fi->fileName(), sizestr, timestr );
      lvi->setPixmap( 0, fi->isDir() ? *pm_folder : *pm_image );
      //lvi->setPixmap( 1, fi->isDir() ? *pm_folder : *pm_image );

      count++;
      size += fi->size();

      fi++;
      }

    QString bstr;
    bstr.setNum( size );
    int sl = bstr.length() - 3;
    while( sl > 0 )
      {
      bstr.insert( sl, ',' );
      sl -= 3;
      }
    char msg[128];
    sprintf( msg, "Files found: %d (%s bytes)", count, bstr.data() );
    sb->showMessage( msg );

/*
    vb->sort();

    lvi = vb->firstChild();
    if ( save_pos != "" )
      {
      while( lvi )
        {
        if ( save_pos == lvi->text( 1 ) ) break;
        lvi = lvi->nextSibling();
        }
      if ( !lvi )
        lvi = vb->firstChild();
      }
    vb->clearSelection();
    vb->setSelected( lvi, true );
    vb->ensureItemVisible( lvi );
    vb->setCurrentItem( lvi );
*/

    if ( optThumbs )
      loadThumbs();
  };

  void qvvMainWindow::loadThumbs()
  {
    QPixmap pm;

    QProgressDialog pd( "Loading thumbnails...", "Cancel", vb->childCount(),
                        NULL, NULL, true );

    QTreeWidgetItem *lvi = vb->currentItem();
    lvi = vb->firstChild();
    int c = 0;
    while( lvi )
      {
      if ( lvi->text( 0 ) == "[DIR]" )
        {
        if ( get_xvpic( cdir.absolutePath() + "/" + lvi->text( 1 ), "directory", &pm ) == 0 )
          lvi->setPixmap( 1, pm );
        //else
        //  lvi->setPixmap( 1, *pm_folder_big );
        }
      else
        {
        if ( get_xvpic( cdir.absolutePath(), lvi->text( 1 ), &pm ) == 0 )
          lvi->setPixmap( 1, pm );
        else
          lvi->setPixmap( 1, *pm_broken_img );
        }
      lvi = lvi->nextSibling();

      pd.setProgress( c );
      if ( pd.wasCancelled() )
        break;
      c++;
      }
  }

  void qvvMainWindow::goToDir( int mode )
  {
    bool ok;
    QString str = cdir.absolutePath();
    switch( mode )
      {
      case '~' : loadDir( cdir.home() ); break;
      case '`' :
                 ok = FALSE;
                 str = QInputDialog::getText( tr( "QVV Image Viewer" ),
                                              tr( "Change current directory to:" ),
                                              QLineEdit::Normal,
                                              str, &ok );
                 if ( ok && !str.isEmpty() )
                   loadDir( str );
                 break;
                 /*
                 QFileDialog* fd = new QFileDialog( this, "Change current directory to:", TRUE );
                 fd->setMode( QFileDialog::Directory );
                 // fd->setViewMode( QFileDialog::Detail );
                 if ( fd->exec() == QDialog::Accepted )
                    loadDir( fd->selectedFile() );
                 */
      }
  };

  void qvvMainWindow::Enter( QTreeWidgetItem *item )
  {
    if ( !item ) return;
    QApplication::setOverrideCursor( waitCursor );
    if ( strcmp( item->text( 0 ), "[DIR]" ) == 0 )
      {
      loadDir( item->text( 1 ) );
      }
    else
      {
      if (!views.first())
        views.append( new qvvView( this ) );
      views.first()->load( cdir.absolutePath() + "/" + item->text( 1 ) );
      }
    QApplication::restoreOverrideCursor();
  };

  void qvvMainWindow::removeItem( QString name )
  {
    QTreeWidgetItem* lvi;

    lvi = vb->firstChild();
    while( lvi )
      {
      if ( lvi->text( 1 ) == name )
        {
	vb->removeItem( lvi );
	return;
	}
      lvi = lvi->nextSibling();
      }
  };

  void qvvMainWindow::closeAll()
  {
    qvvView *view;
    while( view = views.first() )
      {
      views.removeFirst();
      delete view;
      }
  };

  void qvvMainWindow::slotNewWindow()
  {
    qvvMainWindow *vm = new qvvMainWindow( cdir.absolutePath() );
    vm->resize( size() );
    vm->move( x()+32, y()+32 );
  };

  void qvvMainWindow::slotGoUp()
  {
    cdir = cdir.absolutePath();
    QString dname = cdir.dirName();
    if ( dname == "" ) return;
    cdir.cdUp();
    loadDir( cdir.absolutePath() );
    QTreeWidgetItem *lvi = vb->currentItem();
    while( lvi = lvi->nextSibling() )
      {
      if ( dname != lvi->text( 1 ) ) continue;
      vb->clearSelection();
      vb->setSelected( lvi, true );
      vb->ensureItemVisible( lvi );
      vb->setCurrentItem( lvi );
      }
  };

  void qvvMainWindow::slotGoNext()
  {
    QTreeWidgetItem *lvi = vb->currentItem();
    QTreeWidgetItem *lvi_hook = lvi;

    if (!lvi) return; // empty list really

    while(1)
      {
      lvi = lvi->itemBelow();
      if (!lvi)
        lvi = vb->firstChild();
      if (!lvi) return;
      if (lvi == lvi_hook) return;
      if (strcmp(lvi->text( 0 ), "[DIR]")) break;
      }
    vb->clearSelection();
    vb->setSelected( lvi, true );
    vb->ensureItemVisible( lvi );
    vb->setCurrentItem( lvi );
    //if (strcmp(lvi->text( 0 ), "[DIR]"))
      Enter( lvi );
  };

  void qvvMainWindow::slotGoPrev()
  {
    QTreeWidgetItem *lvi = vb->currentItem();
    QTreeWidgetItem *lvi_hook = lvi;

    if (!lvi) return; // empty list really

    while(1)
      {
      lvi = lvi->itemAbove();
      if (!lvi)
        {
        lvi = vb->firstChild();
        while( lvi->itemBelow() )
          lvi = lvi->itemBelow();
        }
      if (!lvi) return;
      if (lvi == lvi_hook) return;
      if (strcmp(lvi->text( 0 ), "[DIR]")) break;
      }
    vb->clearSelection();
    vb->setSelected( lvi, true );
    vb->ensureItemVisible( lvi );
    vb->setCurrentItem( lvi );
    //if (strcmp(lvi->text( 0 ), "[DIR]"))
      Enter( lvi );
  };

  void qvvMainWindow::slotReload()
  {
    loadDir( cdir.absolutePath() );
  };

  void qvvMainWindow::slotGoHome()
  {
    goToDir( '~' );
  };

  void qvvMainWindow::slotDelete()
  {
    QTreeWidgetItem *lvi = vb->currentItem();
    if( lvi == NULL )
      return;

    QTreeWidgetItem *new_current = NULL;
    new_current = lvi->itemBelow();
    if ( new_current == NULL )
      new_current = lvi->itemAbove();

    QString dst_name = cdir.home();
    dst_name += "/.qvv_trash";

    if ( !cdir.exists( dst_name ) )
      cdir.mkdir( dst_name );
    if ( !cdir.exists( dst_name ) )
      {
      printf( "qvv: cannot create trash path\n" );
      return;
      }
    dst_name += "/";
    dst_name += lvi->text( 1 );
    if ( cdir.exists( dst_name ) )
      {
      QString new_dst_name;
      int z = 0;
      do
        {
        z++;
        new_dst_name = "";
        new_dst_name.setNum( z );
        new_dst_name = dst_name + "." + new_dst_name;
        }
      while( cdir.exists( new_dst_name ) );
      dst_name = new_dst_name;
      }

    QString src_name = cdir.absolutePath();
    src_name += "/";
    src_name += lvi->text( 1 );

    if( cdir.rename( src_name, dst_name ) || qfile_copy( src_name, dst_name, 1 ) == 0 )
      {
      delete lvi;
      if ( new_current != NULL )
        {
        vb->clearSelection();
        vb->setSelected( new_current, true );
        vb->ensureItemVisible( new_current );
        vb->setCurrentItem( new_current );
        }
      }
    else
      printf( "qvv: cannot rename to: %s\n", dst_name.data() );
  };

  void qvvMainWindow::slotGoToDir()
  {
    goToDir( '`' );
  };

  void qvvMainWindow::slotThumbs()
  {
    optThumbs = !optThumbs;
    loadDir( cdir.absolutePath() );
  };

  void qvvMainWindow::slotRClick( QTreeWidgetItem *item, const QPoint &point, int col )
  {
    QMenu menu;

    menu.insertItem( "Go to parent dir \t<-", '.' );
    menu.insertItem( "Go to home dir \t~", '~' );
    menu.insertItem( "Change directory \t`", '`' );
    menu.insertSeparator();
    menu.insertItem( "Display logo\tF1", 'l' );
    menu.insertItem( "Open new browser\tF3", 'n' );
    menu.insertItem( "Close this browser\tF4", 'N' );
    menu.insertItem( "Reload current dir\tF5", 'r' );
    menu.insertItem( "Annotate image/dir\tF7", 'e' );
    menu.insertSeparator();
    menu.insertItem( "Auto Fit", 'f' );
    menu.insertItem( "Auto Center", 'c' );
    menu.insertItem( "Show Thumbnails\tF6", 't' );

    menu.setItemChecked( 'f', optFit );
    menu.setItemChecked( 'c', optCenter );
    menu.setItemChecked( 't', optThumbs );

    #define HANDKEY( k, a ) { QKeyEvent ke( QEvent::KeyPress, k, a, 0 ); keyPressEvent( &ke ); }
    switch(menu.exec( point ))
      {
      case 'f' : optFit = !optFit; break;
      case 'c' : optCenter = !optCenter; break;
      case 't' : slotThumbs(); break;
      case '~' : HANDKEY( 0, '~' ); break;
      case '`' : HANDKEY( 0, '`' ); break;
      case '.' : HANDKEY( Qt::Key_Left, 0 ); break;
      case 'l' : HANDKEY( Qt::Key_F1, 0 ); break;
      case 'n' : HANDKEY( Qt::Key_F3, 0 ); break;
      case 'N' : HANDKEY( Qt::Key_F4, 0 ); break;
      case 'r' : HANDKEY( Qt::Key_F5, 0 ); break;
      case 'e' : HANDKEY( Qt::Key_F7, 0 ); break;
      }
    #undef HANDKEY
  };

  void qvvMainWindow::keyPressEvent ( QKeyEvent * e )
  {
    qvvMainWindow *vm;

    switch( e->key() +
            ALT*((e->modifiers() & Qt::AltButton) != 0) +
            SHIFT*((e->modifiers() & Qt::ShiftButton) != 0) +
            CTRL*((e->modifiers() & Qt::ControlButton) != 0) )
      {

      case Qt::Key_F3    : slotNewWindow();
                       break;

      case Qt::Key_F1    : closeAll();
                       views.append( new qvvView( this ) );
                       views.first()->load( "*logo*" );
                       break;
      case ALT+Qt::Key_X : app->quit(); break;
      case Qt::Key_F4    : close();
                       delete this;
                       break;

      case Qt::Key_F5    : loadDir( cdir.absolutePath() ); break;
      case Qt::Key_F6    : slotThumbs(); break;

      case Qt::Key_BracketLeft  : slotGoPrev(); break;
      case Qt::Key_BracketRight : slotGoNext(); break;
      case Qt::Key_Backspace    :
      case Qt::Key_Left         : slotGoUp(); break;
      case Qt::Key_Right        : Enter( vb->currentItem() ); break;
      case Qt::Key_Escape       : closeAll(); break;
      case Qt::Key_Insert       : optCenter = !optCenter; break;
      case Qt::Key_Delete       : slotDelete(); break;
      default:
        if ( e->text().toAscii().at(0) && isalnum(etext().toAscii().at(0)) )
          find( e->text().toAscii().at(0) );
        else
          switch( e->text().toAscii().at(0) )
            {
            case 13  : Enter( vb->currentItem() ); break;
            case '~' : goToDir( '~' ); break;
            case '`' : goToDir( '`' ); break;
            default: QMainWindow::keyPressEvent( e );
            }
      }

  }


/****************************************************************************
**
**
**
****************************************************************************/

  qvvBrowser::qvvBrowser( QWidget * parent )
    : QTreeWidget( parent )
  {
    // setup qvv main window -- listbox
    addColumn( "type",  80 );
    addColumn( "file", 240 );
    addColumn( "size",  90 );
    addColumn( "time", 170 );

    setColumnAlignment( 0, Qt::AlignCenter );
    setColumnAlignment( 1, Qt::AlignLeft   );
    setColumnAlignment( 2, Qt::AlignRight  );
    setColumnAlignment( 3, Qt::AlignRight  );

    setAllColumnsShowFocus( true );
    setSorting( 1, true );
    setShowSortIndicator( true );
    setVScrollBarMode( AlwaysOn );

    setSelectionMode( Extended );

    setAcceptDrops( true );
    viewport()->setAcceptDrops( TRUE );

    mw = getMainWindow();

  };

  qvvBrowser::~qvvBrowser()
  {
  };

  void qvvBrowser::keyPressEvent ( QKeyEvent * e )
  {
    if ( e->text().toAscii().at(0) && isalnum( e->text().toAscii().at(0) ) )
      {
      find( e->text().toAscii().at(0) );
      e->accept();
      }
    else
      {
      if ( !e->text().toAscii().at(0) )
        QTreeWidget::keyPressEvent( e );
      mw->handleKeyPressEvent( e );
      }
  }

  void qvvBrowser::find( int c )
  {
    QTreeWidgetItem *lvi = currentItem();
    QTreeWidgetItem *lvi_hook = lvi;

    if (!lvi) return; // empty list really

    while(1)
      {
      lvi = lvi->itemBelow();
      if (!lvi)
        lvi = firstChild();
      if (!lvi) return;
      if (lvi == lvi_hook) return;
      const char *t = lvi->text( 1 );
      if ( toupper(t[0]) == toupper(c) ) break;
      }
    clearSelection();
    setSelected( lvi, true );
    ensureItemVisible( lvi );
    setCurrentItem( lvi );
    //if (strcmp(lvi->text( 0 ), "[DIR]"))
    //  slotEnter( lvi );
  };

  void qvvBrowser::statusMsg( const char* msg )
  {
    if ( mw ) mw->statusMsg( msg );
  };


/****************************************************************************
**
**
**
****************************************************************************/

  qvvView::qvvView( qvvMainWindow* a_mw )
  {
    mw = a_mw;
    loaded = 0;
    mouse_pan = 0;
    rotation = 0;
    moveabs( ViewX, ViewY );
  }

  qvvView::~qvvView()
  {
    close();
  }

  void qvvView::moverel( int dx, int dy )
  {
    xpos += dx;
    ypos += dy;
    ViewX = xpos;
    ViewY = ypos;
    move( xpos, ypos );
  };

  void qvvView::moveabs( int ax, int ay )
  {
    ViewX = xpos = ax;
    ViewY = ypos = ay;
    move( xpos, ypos );
  };

  void qvvView::load( const char *a_fname )
  {
    if ( strcmp( fname, "*logo*" ) == 0 ) hide();
    strcpy( fname, a_fname );
    reView( optFit ? -1 : 100 );
  }

  void qvvView::reView( int a_scale )
  {
    QImage  img;

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

/*
    if ( rotation )
      {
      QWMatrix m;
      m.rotate( double(rotation) );
      pm = pm.xForm(m);

      w = pm.width();
      h = pm.height();
      }
*/
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
  };

  void qvvView::statusMsg( const char* msg )
  {
    if ( mw ) mw->statusMsg( msg );
  };


  void qvvView::mousePressEvent ( QMouseEvent * e )
  {
    if ( e->modifiers() & Qt::ControlButton )
      {
      mouse_pan = 1;
      mouse_pan_x = e->globalX() - xpos;
      mouse_pan_y = e->globalY() - ypos;
      QApplication::setOverrideCursor( sizeAllCursor );
      }
    QWidget::mousePressEvent( e );
  }

  void qvvView::mouseReleaseEvent ( QMouseEvent * e )
  {
    if ( mouse_pan )
      {
      mouse_pan = 0;
      QApplication::restoreOverrideCursor();
      }
    else
    switch( e->button() )
      {
      case Qt::LeftButton   : if (mw) mw->slotGoNext(); break;
      case Qt::RightButton  : if (mw) mw->slotGoPrev(); break;
      case Qt::MidButton    : if (mw) mw->views.removeOne( this );
                          close(); delete this;
                          break;
      }
    QWidget::mouseReleaseEvent( e );
  }

  void qvvView::mouseMoveEvent ( QMouseEvent * e )
  {
    if ( mouse_pan )
      {
      moveabs( e->globalX() - mouse_pan_x, e->globalY() - mouse_pan_y );
      }

  }

  void qvvView::keyPressEvent ( QKeyEvent *e )
  {
    switch( e->key() )
      {
      case ALT+Qt::Key_X        : app->quit(); break;
      case Qt::Key_Home         : moveabs(  0,  0 ); break;

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
  };

  void qvvView::paintEvent( QPaintEvent * pe )
  {
    if ( !loaded ) return;
    bitBlt( this, 0, 0, &pm, 0, 0, pm.width(), pm.height(), CopyROP );
  };

/****************************************************************************
**
**
**
****************************************************************************/

int main( int argc, char * argv[] )
{
  QApplication a( argc, argv );
  app = &a;

  /*
  if( ! getenv( "QVV_NOAQUA" ) )
    {
    app->setStyle( new QPlatinumStyle2 );
    }
  */

  /* uncomment these if you want fancy menu and tooltips...
  app->setEffectEnabled( Qt::UI_FadeMenu );
  app->setEffectEnabled( Qt::UI_AnimateMenu );
  app->setEffectEnabled( Qt::UI_FadeTooltip );
  app->setEffectEnabled( Qt::UI_AnimateTooltip );
  */

  pm_folder      = new QPixmap( (const char**)ft_folder_xpm  );
  pm_image       = new QPixmap( (const char**)ft_image_xpm   );
  pm_folder_big  = new QPixmap( (const char**)ft_folder_big_xpm  );
  pm_image_big   = new QPixmap( (const char**)ft_image_big_xpm   );
  pm_pattern     = new QPixmap( (const char**)pattern );
  pm_broken_img  = new QPixmap( (const char**)broken_img_xpm );

  if ( getenv( "QVV_FILTERS" ) )
    filters = getenv( "QVV_FILTERS" );
  else
    filters = ".jpg.jpeg.gif.png.xpm.bmp.ppm.";

  filters = filters.lower();

  DeskW = a.desktop()->width();
  DeskH = a.desktop()->height();

  if (getenv("QVV_REDUCE_HEIGHT"))
    DeskH -= atoi(getenv("QVV_REDUCE_HEIGHT"));

  a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

  if ( argc < 2 )
    {
    qvvMainWindow *vm = new qvvMainWindow( "." );
    qvvView *vv = new qvvView( vm );
    vv->load( "*logo*" );
    vm->views.append( vv );
    vm->raise();
    }
  else if( argc == 3 && strcasecmp( argv[1], "-z" ) == 0 )
    {
    QImage img;
    if( img.load( argv[2] ) )
      {
      QPixmap pm;
      if ( strcmp( argv[1], "-Z" ) == 0 )
        {
        int w = app->desktop()->width();
        int h = app->desktop()->height();
        if ( img.width() == w && img.height() == h )
          pm.fromImage( img );
        else
          pm.fromImage( img.smoothScale( w, h ) );
        }
      else
        {
        pm.fromImage( img );
        }
      //app->desktop()->setBackgroundPixmap( pm );
      }
    else
      {
      // FIXME: report error
      }
    return 0;
    }
  else
    for ( int z = 1; z < argc; z++ )
      {
      qvvView *v = new qvvView( NULL );
      v->load( argv[z] );
      }
  return a.exec();
}

