
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <QProgressDialog>

#include <QTreeWidgetItem>

#include <qdebug.h>

#include "qvv.h"
#include "qvv_view.h"
#include "qvv_main_win.h"
#include "qvv_help.h"

QString extensions_filter( ".JPG.JPEG.PNG.GIF.BMP.XPM." );

/*****************************************************************************/

QvvTreeWidget::QvvTreeWidget( QWidget *parent )
     : QTreeWidget( parent )
{
}

void QvvTreeWidget::findNext( QString str )
{
  QTreeWidgetItem *lwi = currentItem();

  int i;
  int start;

  int x = topLevelItemCount();

  i = indexOfTopLevelItem( lwi );
  start = i;

  if( x <= 1 or i < 0 ) return; // not found or empty list

  while(4)
    {
    i++;
    if( i >= x ) i = 0;
    if( i == start ) break;
    lwi = topLevelItem( i );
    if( lwi->text( 1 ).toUpper().indexOf( str.toUpper() ) == 0 ) break;
    lwi = NULL;
    }
  if( lwi )
    {
    setCurrentItem( lwi );
    }
}


void QvvTreeWidget::keyPressEvent ( QKeyEvent * e )
{

  e->ignore();
  int a = e->text() == "" ? 0 : e->text().toAscii().at(0);
  int m = e->modifiers();

  if( ( m == Qt::ShiftModifier || m == Qt::NoModifier ) && a >= '!' && a <= 'z' )
    {
    findNext( QString( QVariant( a ).toChar() ) );
    }
  else
    {
    QTreeWidget::keyPressEvent( e );
    }

}


/*****************************************************************************/

QvvMainWindow::QvvMainWindow()
    : QMainWindow()
{
    rand_seeded = 0;

    opt_thumbs    = 0;
    opt_dirs_only = 0;

    last_sort_col = 1;
    last_sort_ord = Qt::AscendingOrder;

    setAttribute( Qt::WA_DeleteOnClose );

    setObjectName( "QvvMainWindow" );
    setWindowTitle( "QVV/4" );

    // crashes ICEWM :(
    // setWindowIcon( QIcon( ":/images/Half_Face_by_uzorpatorica.jpg" ) );

    resize( 640, 400 );

    tree = new QvvTreeWidget( this );
    tree->setMinimumSize( 400, 200 );

    QTreeWidgetItem *header = new QTreeWidgetItem();
    header->setText( 0, "Type" );
    header->setText( 1, "Name" );
    header->setText( 2, "Size" );
    header->setText( 3, "Modify Time" );
    header->setTextAlignment( 2, Qt::AlignRight );

    tree->setColumnCount( 4 );
    tree->setHeaderItem( header );

    tree->setColumnWidth( 0,  70 );
    tree->setColumnWidth( 1, 300 );
    tree->setColumnWidth( 2,  80 );
    tree->setColumnWidth( 3, 120 );

    tree->setIconSize( QSize( 64, 64 ) );

    tree->setSortingEnabled( 1 );
    tree->sortByColumn( 1, Qt::AscendingOrder );

    tree->setDragEnabled( 1 );

    setCentralWidget( tree );

    setupMenuBar();

    //statusBar()->showMessage( "." );

    //loadDir( QString( "." ) );

    connect( tree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(slotItemActivated(QTreeWidgetItem *, int)));
}

QvvMainWindow::~QvvMainWindow()
{
  while( views.count() > 0 )
    {
    QvvView *v = views.takeFirst();
    delete v;
    }
}

/*****************************************************************************/

void QvvMainWindow::loadDir( QString path )
{
  QString last_path = cdir.absolutePath();

  cdir.cd( path );

  QString new_path = cdir.absolutePath();

  setWindowTitle( "QVV/4: " + new_path );

  QString save_item_name;
  if( tree->topLevelItemCount() > 0 ) save_item_name = tree->currentItem()->text( 1 );

  QStringList filters;
  filters.append( QString( "*" ) );

  QFileInfoList info_list = cdir.entryInfoList( filters );

  QTreeWidgetItem *current = NULL;

  tree->clear();
  for( int i = 0; i < info_list.count(); i++ )
    {
    QFileInfo fi = info_list.at( i );

    if( opt_dirs_only && ! fi.isDir() ) continue;

    if( fi.fileName() == "."  ) continue;
    if( fi.fileName() == ".." ) continue;

    QString ext = "." + fi.suffix() + ".";
    if( ! fi.isDir() && extensions_filter.indexOf( ext.toUpper() ) < 0 ) continue;

    QString file_name = fi.fileName();

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText( 0, fi.isDir() ? ITEM_TYPE_DIR : "" );
    item->setText( 1, file_name );
    item->setText( 2, QVariant( fi.size() ).toString() );
    item->setText( 3, fi.lastModified().toString( "yyyy-MM-dd hh:mm:ss" ) );
    item->setTextAlignment( 2, Qt::AlignRight );

    if( last_path == new_path + "/" + file_name ) current = item;

    tree->addTopLevelItem( item );
    }

  if( current )
    tree->setCurrentItem( current );
  else
    {
    tree->setCurrentItem( tree->topLevelItem( 0 ) );
    if( save_item_name != "" )
      tree->findNext( save_item_name );
    }

  statusBar()->showMessage( QString( "Loaded items: " ) + QVariant( tree->topLevelItemCount() ).toString() );

  if( opt_thumbs )
    loadThumbs();

  tree->resizeColumnToContents( 2 );
  tree->resizeColumnToContents( 3 );
};

/*****************************************************************************/

void QvvMainWindow::loadThumbs()
{
  QProgressDialog pd( "Loading thumbnails...", "Cancel", 0, tree->topLevelItemCount() - 1 );

  QString new_path = cdir.absolutePath();

  for( int i = 0; i < tree->topLevelItemCount(); i++ )
    {
    pd.setValue( i );
    if ( pd.wasCanceled() )
      break;

    QTreeWidgetItem *item = tree->topLevelItem( i );
    if( item->text( 0 ) == ITEM_TYPE_DIR ) continue;

    QString icon_fn = new_path + "/.thumbnails/" + item->text( 1 ) + ".png";
    if( QFile::exists( icon_fn ) )
      {
      QIcon icon( icon_fn );
      if( ! icon.isNull() ) item->setIcon( 1, icon );
      }
    }

}

/*****************************************************************************/

void QvvMainWindow::goToDir( int mode )
{
  mode = ! mode; // FIXME: for what??
  QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
/*
  if (!native->isChecked())
      options |= QFileDialog::DontUseNativeDialog;
*/
  QString new_dir = QFileDialog::getExistingDirectory( this,
                              tr( "Change directory" ),
                              cdir.absolutePath(),
                              options );
  if ( ! new_dir.isEmpty() )
      loadDir( new_dir );
};

/*****************************************************************************/

void QvvMainWindow::enter( QTreeWidgetItem *item )
{
  if( item->text( 0 ) == ITEM_TYPE_DIR )
    {
    loadDir( item->text( 1 ) );
    }
  else
    {
    QvvView *view;
    if( views.count() > 0 )
      {
      view = views[0];
      }
    else
      {
      view = new QvvView( this );
      views.append( view );
      }
    view->load( cdir.absolutePath() + "/" + item->text( 1 ) );
    }
};

/*****************************************************************************/

void QvvMainWindow::closeView( QvvView *view )
{
  view->close();
  views.removeOne( view );
  delete view;
};

/*****************************************************************************/

void QvvMainWindow::slotItemActivated( QTreeWidgetItem *item, int column )
{
  enter( item );
};

/*****************************************************************************/

void QvvMainWindow::slotNewWindow()
{
  QvvMainWindow *new_mainwin = new QvvMainWindow();
  new_mainwin->resize(600, 400);
  new_mainwin->show();
  new_mainwin->loadDir( cdir.absolutePath() );
};

/*****************************************************************************/

void QvvMainWindow::slotGoUp()
{
  loadDir( ".." );
};

/*****************************************************************************/

void QvvMainWindow::slotGoPrev()
{
  goPrevNext( -1 );
};

void QvvMainWindow::slotGoNext()
{
  goPrevNext( +1 );
}

void QvvMainWindow::goPrevNext( int r )
{
  QTreeWidgetItem *lwi = tree->currentItem();

  int i;
  int start;

  int x = tree->topLevelItemCount();

  i = tree->indexOfTopLevelItem( lwi );
  start = i;

  //qDebug() << "CurrentItemStart: " << i << " " << lwi->text( 0 ) << " " << lwi->text( 1 );

  if( x <= 1 or i < 0 ) return; // not found or empty list

  while(4)
    {
    i += r;
    lwi = NULL;
    if( i >= x ) i = 0;
    if( i <  0 ) i = x - 1;
    if( i == start ) break;
    lwi = tree->topLevelItem( i );
    //qDebug() << "Searching: " << i << " " << lwi->text( 0 ) << " " << lwi->text( 1 );
    if( lwi->text( 0 ) == ITEM_TYPE_DIR ) continue;
    break;
    }
  if( lwi )
    {
    //qDebug() << "CurrentItemFound: " << i << " " << lwi->text( 0 ) << " " << lwi->text( 1 );
    tree->setCurrentItem( lwi );
    enter( lwi );
    //lwi = tree->currentItem();
    //qDebug() << "CurrentItemFound: " << i << " " << lwi->text( 0 ) << " " << lwi->text( 1 );
    }
};

/*****************************************************************************/

void QvvMainWindow::slotThumbs()
{
  opt_thumbs = ! opt_thumbs;
  if( opt_thumbs ) loadThumbs();

  statusBar()->showMessage( opt_thumbs ? tr( "Thumbnails enabled" ) : tr( "Thumbnails disabled" ) );
};

void QvvMainWindow::slotChangeDir()
{
  goToDir( 0 );
}

void QvvMainWindow::slotHomeDir()
{
  loadDir( cdir.homePath() );
}

void QvvMainWindow::slotReloadDir()
{
  loadDir( cdir.absolutePath() );
}

void QvvMainWindow::slotShowDirsOnly()
{
  opt_dirs_only = ! opt_dirs_only;
  loadDir( cdir.absolutePath() );

  statusBar()->showMessage( opt_dirs_only ? tr( "Show directories only" ) : tr( "Show all directories and files" ) );
}

void QvvMainWindow::sortColumn( int n )
{
  if( last_sort_col == n )
    {
    last_sort_ord = last_sort_ord == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    }
  else
    {
    last_sort_ord = Qt::AscendingOrder;
    }
  last_sort_col = n;
  tree->sortByColumn( last_sort_col, last_sort_ord );
};

void QvvMainWindow::slotSortColumn1()
{
  sortColumn( 1 );
};

void QvvMainWindow::slotSortColumn3()
{
  sortColumn( 3 );
};

void QvvMainWindow::slotRandomItem()
{
  int x = tree->topLevelItemCount();
  if( rand_seeded == 0 ) qsrand( QDateTime::currentDateTime().toTime_t() );
  rand_seeded = 1;
  int r = qrand();

  int n = int( r % x );

  tree->setCurrentItem( tree->topLevelItem( n ) );
  slotGoNext();
};

void QvvMainWindow::slotHelp()
{
  display_help();
};

/*****************************************************************************/

void QvvMainWindow::keyPressEvent ( QKeyEvent * e )
{

  e->accept();
  if( e->modifiers() & Qt::ALT )
    {
    switch( e->key() )
      {
      default: e->ignore(); QMainWindow::keyPressEvent( e ); break;
      }
    }
  else
    {
    switch( e->key() )
      {
      case Qt::Key_F1: slotHelp(); break;
/*
      case Qt::Key_F3    : slotNewWindow(); break;
      case Qt::Key_F4    : close();
                           delete this;
                           break;
*/
      case Qt::Key_Left  : slotGoUp(); break;
      case Qt::Key_Right :
      case Qt::Key_Return: enter( tree->currentItem() ); break;

      case Qt::Key_BracketLeft  : slotGoPrev(); break;
      case Qt::Key_Space        :
      case Qt::Key_BracketRight : slotGoNext(); break;

/*
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
*/
      default:
            if( e->text() == "" )
              QWidget::keyPressEvent( e );
            else
              switch( e->text().toAscii().at(0) )
              {
              case '[' : slotGoPrev(); break;
              case ']' : slotGoNext(); break;
              case '~' : slotHomeDir();   break;
              case '`' : slotChangeDir();   break;

              default: e->ignore(); QMainWindow::keyPressEvent( e ); break;
              }
      }
    }
}

/*****************************************************************************/

void QvvMainWindow::actionTriggered(QAction *action)
{
    qDebug("action '%s' triggered", action->text().toLocal8Bit().data());
}

/*****************************************************************************/

void QvvMainWindow::setupMenuBar()
{
    /*--------------------------------------------------------------------*/

    QMenu *menu = menuBar()->addMenu( tr("&File") );

    QAction *action;
/*
    action = menu->addAction(tr("Save layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

    action = menu->addAction(tr("Load layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));
    action = menu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));

    menu->addSeparator();
*/

    action = menu->addAction( tr("&Reload directory"), this, SLOT(slotReloadDir()), Qt::Key_F5 );
    action->setIcon( QIcon( ":/images/view-refresh.png" ) );

    action = menu->addAction( tr("&Quit"), this, SLOT(close()), Qt::AltModifier + Qt::Key_X );
    action->setIcon( QIcon( ":/images/system-log-out.png" ) );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&View"));

    action = menu->addAction( tr("Enable &thumbnails") );
    action->setCheckable( true );
    action->setChecked( opt_thumbs );
    action->setShortcut( Qt::Key_F6 );
    connect( action, SIGNAL( toggled(bool) ), this, SLOT(slotThumbs()) );

    action = menu->addAction( tr("Show d&irectories only") );
    action->setCheckable( true );
    action->setChecked( opt_dirs_only );
    action->setShortcut( Qt::AltModifier + Qt::Key_I );
    connect( action, SIGNAL( toggled(bool) ), this, SLOT(slotShowDirsOnly()) );

    menu->addSeparator();

    action = menu->addAction( tr("Sort by &Name"),        this, SLOT(slotSortColumn1()), Qt::AltModifier + Qt::Key_N );
    action = menu->addAction( tr("Sort by &Modify Time"), this, SLOT(slotSortColumn3()), Qt::AltModifier + Qt::Key_M );


    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&Go"));

    action = menu->addAction( tr("Go to p&arent directory"), this, SLOT(slotGoUp()), Qt::AltModifier + Qt::Key_A );
    action->setIcon( QIcon( ":/images/go-up.png" ) );

    menu->addAction( tr("Change &directory"), this, SLOT(slotChangeDir()), Qt::AltModifier + Qt::Key_D );

    action = menu->addAction( tr("Go to &home directory"), this, SLOT(slotHomeDir()), Qt::AltModifier + Qt::Key_Home );
    action->setIcon( QIcon( ":/images/go-home.png" ) );

    menu->addSeparator();

    action = menu->addAction( tr("Go to &Random image"),        this, SLOT(slotRandomItem()), Qt::Key_Asterisk );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&Window"));

    action = menu->addAction( tr("&New window"), this, SLOT(slotNewWindow()), Qt::Key_F3 );
    action->setIcon( QIcon( ":/images/window-new.png" ) );

    menu->addAction( tr("&Close window"), this, SLOT(close()), Qt::Key_F4 );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&Help") );

    action = menu->addAction( tr("&Contents"), this, SLOT(slotHelp()), Qt::Key_F1 );
    //action = menu->addAction( tr("&About"),  this, SLOT(slotAbout()), Qt::AltModifier + Qt::Key_M );

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

/*
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
*/


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
