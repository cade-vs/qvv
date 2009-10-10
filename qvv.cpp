
#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include <QTreeWidgetItem>



#include <qdebug.h>

#include <qvv.h>


QString extensions_filter( ".JPG.JPEG.PNG.GIF.BMP.XPM." );


QvvMainWindow::QvvMainWindow()
    : QMainWindow()
{
    setObjectName( "QvvMainWindow" );
    setWindowTitle( "Qt Main Window Demo" );

    tree = new QTreeWidget( this );
    tree->setMinimumSize( 400, 200 );

    QTreeWidgetItem *header = new QTreeWidgetItem();
    header->setText( 0, "Type" );
    header->setText( 1, "Name" );
    header->setText( 2, "Size" );
    header->setTextAlignment( 2, Qt::AlignRight );

    tree->setColumnCount( 3 );
    tree->setHeaderItem( header );

    tree->setColumnWidth( 0,  70 );
    tree->setColumnWidth( 1, 300 );
    tree->setColumnWidth( 2,  70 );


    setCentralWidget( tree );

    setupMenuBar();

    statusBar()->showMessage( "Status Bar" );

    loadDir( QString( "." ) );
}

void QvvMainWindow::loadDir( QString path )
{
  QString last_path = cdir.absolutePath();

  cdir.cd( path );

  QString new_path = cdir.absolutePath();

  QStringList filters;
  filters.append( QString( "*" ) );

  QFileInfoList info_list = cdir.entryInfoList( filters );

  QTreeWidgetItem *current = NULL;

  tree->clear();
  for( int i = 0; i < info_list.count(); i++ )
    {
    QFileInfo fi = info_list.at( i );

    if( fi.fileName() == "."  ) continue;
    if( fi.fileName() == ".." ) continue;

    QString ext = "." + fi.suffix() + ".";
    if( ! fi.isDir() && extensions_filter.indexOf( ext.toUpper() ) < 0 ) continue;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText( 0, fi.isDir() ? ITEM_TYPE_DIR : "" );
    item->setText( 1, fi.fileName() );
    item->setText( 2, QVariant( fi.size() ).toString() );
    item->setTextAlignment( 2, Qt::AlignRight );

    if( last_path == new_path + "/" + fi.fileName() ) current = item;

    tree->addTopLevelItem( item );
    }

  if( current )
    tree->setCurrentItem( current );
  else
    tree->setCurrentItem( tree->topLevelItem( 0 ) );
};

void QvvMainWindow::goToDir( int mode )
{
};

void QvvMainWindow::Enter( QTreeWidgetItem *item )
{
  if( item->text( 0 ) == ITEM_TYPE_DIR )
    {
    loadDir( item->text( 1 ) );
    }
  else
    {
    }
};

void QvvMainWindow::slotNewWindow()
{
  QvvMainWindow *new_mainwin = new QvvMainWindow();
  new_mainwin->resize(600, 400);
  new_mainwin->show();
};

void QvvMainWindow::slotGoUp()
{
  loadDir( ".." );
};

void QvvMainWindow::keyPressEvent ( QKeyEvent * e )
{

  if( e->modifiers() & Qt::ALT )
    {
    switch( e->key() )
      {
      case Qt::Key_X   : QApplication::quit(); break;
      }
    }
  else
    {
    switch( e->key() )
      {
      case Qt::Key_F3    : slotNewWindow(); break;
      case Qt::Key_Left  : slotGoUp(); break;
      case Qt::Key_Right : Enter( tree->currentItem() ); break;

/*
      case Qt::Key_F1    : closeAll();
                           views.append( new qvvView( this ) );
                           views.first()->load( "*logo*" );
                           break;

      case Qt::Key_F4    : close();
                           delete this;
                           break;

      case Qt::Key_F5    : loadDir( cdir.absolutePath() ); break;
      case Qt::Key_F6    : slotThumbs(); break;

      case Qt::Key_BracketLeft  : slotGoPrev(); break;
      case Qt::Key_BracketRight : slotGoNext(); break;
      case Qt::Key_Backspace    :
      case Qt::Key_Escape       : closeAll(); break;
      case Qt::Key_Insert       : optCenter = !optCenter; break;
      case Qt::Key_Delete       : slotDelete(); break;
*/
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
      }
    }


}


void QvvMainWindow::actionTriggered(QAction *action)
{
    qDebug("action '%s' triggered", action->text().toLocal8Bit().data());
}

void QvvMainWindow::setupMenuBar()
{
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    QAction *action;
/*
    action = menu->addAction(tr("Save layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

    action = menu->addAction(tr("Load layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));
*/
    action = menu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));

    menu->addSeparator();

    menu->addAction(tr("&Quit"), this, SLOT(close()));

    menu = menuBar()->addMenu(tr("Main window"));
/*
    action = menu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = menu->addAction(tr("Allow nested docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowNestedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = menu->addAction(tr("Allow tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = menu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = menu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));
*/
}

QAction *addAction(QMenu *menu, const QString &text, QActionGroup *group, QSignalMapper *mapper,
                    int id)
{
    bool first = group->actions().isEmpty();
    QAction *result = menu->addAction(text);
    result->setCheckable(true);
    result->setChecked(first);
    group->addAction(result);
    QObject::connect(result, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(result, id);
    return result;
}



void QvvMainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void QvvMainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}




int main(int argc, char **argv)
{
     QApplication app(argc, argv);
     QvvMainWindow mainWin;
     mainWin.resize(600, 400);
     mainWin.show();
     return app.exec();
}
