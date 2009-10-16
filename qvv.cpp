
#include <QApplication>
#include <QResource>

#include "qvv.h"
#include "qvv_view.h"
#include "qvv_main_win.h"

QDesktopWidget *Desktop;
int DeskW;
int DeskH;

QTextBrowser *help_browser;

void display_help()
{
  if( help_browser == NULL )
    help_browser = new QTextBrowser;
  help_browser->setHtml( QString( QVariant( (const char*)(QResource( ":/qvv_help.html" ).data()) ).toString() ) );

  help_browser->setObjectName( "QvvHelpWindow" );
  help_browser->setWindowTitle( "QVV/4 Help" );

  help_browser->resize( 640, 400 );
  help_browser->move( 100, 100 );

  help_browser->show();
};

int main(int argc, char **argv)
{
  QApplication app( argc, argv );
  Q_INIT_RESOURCE(qvv);

  help_browser = NULL;

  Desktop = QApplication::desktop();

  DeskW  = Desktop->width(); // get width of screen
  DeskH = Desktop->height(); // get height of screen

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
      //main_win->views.append( view );
      view->load( str );
      }
    }
  else
    {
    QvvMainWindow *main_win = new QvvMainWindow();
    main_win->loadDir( QString( "." ) );
    //main_win->move( view->width() + 10, view->height() - main_win->height() );
    main_win->show();

    QvvView *view = new QvvView( main_win );
    main_win->views.append( view );
    view->load( ":/images/Half_Face_by_uzorpatorica.jpg" );
    }

  return app.exec();
}
