
#include <QApplication>

#include <qvv.h>

int main(int argc, char **argv)
{
     QApplication app(argc, argv);
     Q_INIT_RESOURCE(qvv);

     QvvMainWindow *main_win = new QvvMainWindow();
     //main_win->move( view->width() + 10, view->height() - main_win->height() );
     main_win->show();

     QvvView *view = new QvvView( main_win );
     main_win->views.append( view );
     view->load( ":/images/Half_Face_by_uzorpatorica.jpg" );

     return app.exec();
}
