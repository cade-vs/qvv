
#include <QApplication>

#include <qvv.h>

int main(int argc, char **argv)
{
     QApplication app(argc, argv);
     Q_INIT_RESOURCE(qvv);
     QvvMainWindow *main_win = new QvvMainWindow();
     main_win->show();
     return app.exec();
}
