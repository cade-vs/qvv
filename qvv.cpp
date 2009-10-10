
#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QDir>
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



QvvMainWindow::QvvMainWindow()
    : QMainWindow()
{
    setObjectName( "QvvMainWindow" );
    setWindowTitle( "Qt Main Window Demo" );

    tree = new QTreeWidget( this );
    tree->setMinimumSize( 400, 205 );

    QTreeWidgetItem *header = new QTreeWidgetItem();
    header->setText( 0, "Type" );
    header->setText( 1, "Name" );
    header->setText( 2, "Size" );
    header->setTextAlignment( 2, Qt::AlignRight );

    tree->setColumnCount( 3 );
    tree->setHeaderItem( header );

    setCentralWidget( tree );

    setupMenuBar();

    statusBar()->showMessage( "Status Bar" );

    loadDir( "." );
}

void QvvMainWindow::loadDir( const char* path )
{
  cdir.cd( path );

  QStringList filters;
  filters.append( QString( "*" ) );

  QFileInfoList info_list = cdir.entryInfoList( filters );


  for( int i = 0; i < info_list.count(); i++ )
    {
    QFileInfo fi = info_list.at( i );

    if( fi.fileName() == "."  ) continue;
    if( fi.fileName() == ".." ) continue;

    QTreeWidgetItem *item = new QTreeWidgetItem( tree );
    item->setText( 0, fi.isDir() ? "[DIR]" : "" );
    item->setText( 1, fi.fileName() );
    item->setText( 2, QVariant( fi.size() ).toString() );
    item->setTextAlignment( 2, Qt::AlignRight );
    }

  tree->setCurrentItem( tree->topLevelItem( 0 ) );
};

void QvvMainWindow::goToDir( int mode )
{

};

void QvvMainWindow::Enter( QTreeWidgetItem * )
{
};

void QvvMainWindow::slotNewWindow()
{
  QvvMainWindow *new_mainwin = new QvvMainWindow();
  new_mainwin->resize(600, 400);
  new_mainwin->show();
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
      case Qt::Key_F3    : slotNewWindow();
                           break;

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
      case Qt::Key_Left         : slotGoUp(); break;
      case Qt::Key_Right        : Enter( vb->currentItem() ); break;
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

    QAction *action = menu->addAction(tr("Save layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

    action = menu->addAction(tr("Load layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));

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


void QvvMainWindow::saveLayout()
{
    QString fileName
        = QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok)
        ok = file.write(geo_data) == geo_data.size();
    if (ok)
        ok = file.write(layout_data) == layout_data.size();

    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void QvvMainWindow::loadLayout()
{
    QString fileName
        = QFileDialog::getOpenFileName(this, tr("Load layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok)
        ok = restoreGeometry(geo_data);
    if (ok)
        ok = restoreState(layout_data);

    if (!ok) {
        QString msg = tr("Error reading %1")
                        .arg(fileName);
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
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
