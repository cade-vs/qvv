/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

#include <QApplication>

#include "qvv.h"
#include "qvv_view.h"
#include "qvv_main_win.h"
#include "qvv_help.h"

QDesktopWidget *Desktop;
int DeskW;
int DeskH;

QString extensions_filter;

QSettings Settings( "CSA", "QVV4" );

int opt_thumbs_size;
int opt_create_thumbs;
int opt_create_smooth_thumbs;
int opt_create_jpeg_thumbs;
int opt_use_toolbar;

int main(int argc, char **argv)
{
  QApplication app( argc, argv );
  Q_INIT_RESOURCE(qvv);

  help_browser = NULL;

  Desktop = QApplication::desktop();

  DeskW = Desktop->width();  // get width of screen
  DeskH = Desktop->height(); // get height of screen

  opt_thumbs_size          = Settings.value( "thumbs_size", 128 ).toInt();
  opt_create_thumbs        = Settings.value( "create_thumbs", 0 ).toInt();
  opt_create_jpeg_thumbs   = Settings.value( "create_jpeg_thumbs", 0 ).toInt();
  opt_create_smooth_thumbs = Settings.value( "create_smooth_thumbs", 1 ).toInt();
  opt_use_toolbar          = Settings.value( "use_toolbar", 1 ).toInt();

  extensions_filter        = Settings.value( "extensions_filter", QString( DEFAULT_EXTENSIONS_FILTER ) ).toString();

  QStringList args = app.arguments();
  QStringList imgs;

  args.takeFirst(); // remove app name
  while( args.count() > 0 )
    {
    QString str = args.takeFirst();
    if( str.indexOf( "-" ) == 0 ) continue;
    imgs.append( str );
    }

  if( imgs.count() > 0 )
    {
    while( imgs.count() > 0 )
      {
      QString str = imgs.takeFirst();
      QvvView *view = new QvvView( NULL );
      // FIXME: TODO:
      //main_win->views.append( view );
      view->load( str );
      }
    }
  else
    {
    QvvMainWindow *main_win = new QvvMainWindow();
    main_win->loadDir( QString( "." ) );
    //main_win->move( view->width() + 10, view->height() - main_win->height() );
    main_win->move( 100, 100 );
    main_win->show();

    /*
    QvvView *view = new QvvView( main_win );
    main_win->views.append( view );
    //view->load( ":/images/Half_Face_by_uzorpatorica.jpg" );
    view->load( ":/images/Look_To_The_Sun_by_K3win_sm.png" );
    */
    }

  return app.exec();
}
