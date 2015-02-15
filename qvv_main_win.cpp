/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

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
#include <QImage>
#include <QPixmap>
#include <QCryptographicHash>

#include <QTreeWidgetItem>
#include <QAbstractItemView>

#include <qdebug.h>

#include "qvv.h"
#include "qvv_view.h"
#include "qvv_main_win.h"
#include "qvv_help.h"

/*****************************************************************************/

QvvConfirmDeleteDialog::QvvConfirmDeleteDialog()
     : QDialog()
{
  cd.setupUi( this );

  activated_button = QDialogButtonBox::NoButton;
  connect( cd.buttonBox->button( QDialogButtonBox::Yes      ), SIGNAL(clicked()), this, SLOT(buttonYes())      );
  connect( cd.buttonBox->button( QDialogButtonBox::YesToAll ), SIGNAL(clicked()), this, SLOT(buttonYesToAll()) );
  connect( cd.buttonBox->button( QDialogButtonBox::No       ), SIGNAL(clicked()), this, SLOT(buttonNo())       );
  connect( cd.buttonBox->button( QDialogButtonBox::Cancel   ), SIGNAL(clicked()), this, SLOT(buttonCancel())   );
}

int QvvConfirmDeleteDialog::exec()
{
  activated_button = QDialogButtonBox::NoButton;
  return QDialog::exec();
}

/*****************************************************************************/

QvvTreeWidget::QvvTreeWidget( QWidget *parent )
     : QTreeWidget( parent )
{
}

void QvvTreeWidget::findNext( QString str, int full_match )
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
    if(   full_match && lwi->text( 1 ).toUpper() == str.toUpper() ) break;
    if( ! full_match && lwi->text( 1 ).toUpper().indexOf( str.toUpper() ) == 0 ) break;
    lwi = NULL;
    }
  if( lwi )
    {
    setCurrentItem( lwi );
    scrollToItem( lwi );
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
    toolbar = NULL;

    rand_seeded = 0;

    opt_thumbs        = 0;
    opt_dirs_only     = 0;

    last_sort_col = 1;
    last_sort_ord = Qt::AscendingOrder;

    setAttribute( Qt::WA_DeleteOnClose );

    setObjectName( "QvvMainWindow" );
    setWindowTitle( QString() + " QVV " + QVV_VERSION );

    // crashes ICEWM :(
    //setWindowIcon( QIcon( QPixmap( ":/images/qvv_icon_128x128.png" ) ) );

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

    tree->setSelectionMode( QAbstractItemView::ExtendedSelection );

    setCentralWidget( tree );

    setupMenuBar();

    //statusBar()->showMessage( "." );

    //loadDir( QString( "." ) );

    connect( tree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(slotItemActivated(QTreeWidgetItem *, int)));
}

QvvMainWindow::~QvvMainWindow()
{
  closeAllViews();
}

void QvvMainWindow::closeAllViews()
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

  setWindowTitle( QString() + " QVV " + QVV_VERSION + ": " + new_path );

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

  statusBar()->showMessage( QString( tr( "Loaded items" ) ) + ": " + QVariant( tree->topLevelItemCount() ).toString() );

  if( opt_thumbs )
    loadThumbs();

  tree->resizeColumnToContents( 2 );
  tree->resizeColumnToContents( 3 );
};

/*****************************************************************************/

void QvvMainWindow::loadThumbs()
{
  QProgressDialog pd( tr( "Loading thumbnails..." ), tr( "Cancel" ), 0, tree->topLevelItemCount() - 1 );
  pd.move( x() + ( ( width() - pd.width() ) / 2 ), y() + ( ( height() - pd.height() ) / 2) );

  QString new_path = cdir.absolutePath();

  for( int i = 0; i < tree->topLevelItemCount(); i++ )
    {
    pd.setValue( i );
    if ( pd.wasCanceled() )
      break;
    QApplication::processEvents();

    QTreeWidgetItem *item = tree->topLevelItem( i );
    if( item->text( 0 ) == ITEM_TYPE_DIR ) continue;

    QString icon_dir = new_path + "/.thumbnails";
    if( opt_create_thumbs )
      {
      if( ! cdir.exists( icon_dir ) )
        cdir.mkdir( icon_dir );
      }

    QStringList icon_fns;
    icon_fns.append( new_path + "/.thumbnails/" + item->text( 1 ) + ".jpg" ); // JPEG thumb idex is 0!
    icon_fns.append( new_path + "/.thumbnails/" + item->text( 1 ) + ".png" ); // PNG  thumb idex is 1!

    int found = -1;
    for( int z = 0; z < icon_fns.count(); z++ )
      {
      if( QFile::exists( icon_fns[z] ) )
        {
        found = z;
        break;
        }
      }

    if( found == -1 && opt_create_thumbs )
      {
      QString file_name = new_path + "/" + item->text( 1 );
      QImage im;
      im.load( file_name );
      if( im.width() > opt_thumbs_size || im.height() > opt_thumbs_size )
        im = im.scaled( QSize( opt_thumbs_size, opt_thumbs_size ) , Qt::KeepAspectRatio, opt_create_smooth_thumbs ? Qt::SmoothTransformation : Qt::FastTransformation );
      int thumb_format_index = 1; // use PNG index!
      if( opt_create_jpeg_thumbs )
        {
        QFileInfo fi( file_name );
        QString ext = fi.suffix().toLower();
        if( ext == "jpg" || ext == "jpeg" )
          thumb_format_index = 0; // use JPEG index! but only for jpeg files
        }
      im.save( icon_fns[ thumb_format_index ] );
      found = thumb_format_index;
      }

    if( found > -1 )
      {
      QIcon icon( icon_fns[ found ] );
      if( ! icon.isNull() ) item->setIcon( 1, icon );
      }

    } // for tree

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
  if( ! item ) return;
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

void QvvMainWindow::enterCurrent()
{
  enter( tree->currentItem() );
}

void QvvMainWindow::enterAllSelected()
{
  QList<QTreeWidgetItem *> selected = tree->selectedItems();
  if( selected.count() > 1 )
    {
    closeAllViews();
    QTreeWidgetItem *item;
    for( int i = 0; i < selected.count(); i++ )
      {
      item = selected[i];
      if( item->text( 0 ) == ITEM_TYPE_DIR ) continue;
      QvvView *view;
      view = new QvvView( this );
      views.prepend( view );
      view->load( cdir.absolutePath() + "/" + item->text( 1 ) );
      }
    }
  else
    {
    statusBar()->showMessage( tr( "Error: More than one selected image required" ) );
    }
}

/*****************************************************************************/

void QvvMainWindow::setActiveView( QvvView *view )
{
  if( views.count() < 1 ) return;
  int z = views.indexOf( view );
  if( z > 0 )
    {
    views.swap( 0, z );
    QFileInfo fi( views[0]->getFileName() );
    tree->findNext( fi.fileName(), 1 );
    }
};

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

  if( x == 0 )
    {
    closeAllViews();
    return;
    }

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

void QvvMainWindow::slotCreateThumbs()
{
  opt_create_thumbs = ! opt_create_thumbs;
  Settings.setValue( "create_thumbs", opt_create_thumbs );

  statusBar()->showMessage( opt_create_thumbs ? tr( "Thumbnails creation enabled" ) : tr( "Thumbnails creation disabled" ) );
};

void QvvMainWindow::slotJPEGThumbs()
{
  opt_create_jpeg_thumbs = ! opt_create_jpeg_thumbs;
  Settings.setValue( "create_jpeg_thumbs", opt_create_jpeg_thumbs );

  statusBar()->showMessage( opt_create_jpeg_thumbs ? tr( "JPEG Thumbnails creation for JPEGs enabled" ) : tr( "PNG Thumbnails creation for all image types enabled" ) );
};

void QvvMainWindow::slotSmoothThumbs()
{
  opt_create_smooth_thumbs = ! opt_create_smooth_thumbs;
  Settings.setValue( "create_smooth_thumbs", opt_create_smooth_thumbs );

  statusBar()->showMessage( opt_create_smooth_thumbs ? tr( "Smooth Thumbnails creation enabled" ) : tr( "Fast Thumbnails creation enabled" ) );
};

void QvvMainWindow::slotUseToolbar()
{
  opt_use_toolbar = ! opt_use_toolbar;
  Settings.setValue( "use_toolbar", opt_use_toolbar );

  if( toolbar )
    {
    toolbar->setVisible( opt_use_toolbar ? 1 : 0 );
    if( toolbar->isVisible() )
      toolbar->show();
    else
      toolbar->hide();
    }

  statusBar()->showMessage( opt_use_toolbar ? tr( "Toolbar use enabled" ) : tr( "Toolbar use enabled" ) );
};

void QvvMainWindow::slotToggleToolbar()
{
  if( toolbar )
    {
    toolbar->setVisible( toolbar->isVisible() ? 0 : 1 );
    if( toolbar->isVisible() )
      toolbar->show();
    else
      toolbar->hide();
    }
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

void QvvMainWindow::slotAbout()
{
  QvvView *view;

  if( views.count() > 0 )
    view = views[0];
  else
    {
    view = new QvvView( this );
    views.append( view );
    }
  view->load( ":/images/Green_woman_by_ValentinaKallias_edit2.png" );
  view->setWindowTitle( QString() + " QVV " + QVV_VERSION );
};

void QvvMainWindow::slotDeleteSelected()
{
  deleteItems( 0 );
}

void QvvMainWindow::slotDeleteCurrent()
{
  deleteItems( 1 );
}

int QvvMainWindow::deleteItems( int current_only )
{
  QDir trashdir;

  QString trash_can = trashdir.homePath();
  trash_can += "/.qvv4_trash_can";

  if( ! trashdir.exists( trash_can ) )
    trashdir.mkdir( trash_can );
  if( ! trashdir.exists( trash_can ) )
    {
    statusBar()->showMessage( tr( "Error: Cannot find/create trash can path:" ) + " " + trash_can );
    return -1;
    }

  QList<QTreeWidgetItem *> selected;
  if( current_only )
    selected.append( tree->currentItem() );
  else
    selected = tree->selectedItems();
  int sel_count = selected.count();

  if( sel_count < 1 )
    {
    statusBar()->showMessage( tr( "Error: No files selected for delete" ) );
    return -1;
    };

  QvvConfirmDeleteDialog *confirm = new QvvConfirmDeleteDialog;
  confirm->move( x() + ( ( width() - confirm->width() ) / 2 ), y() + ( ( height() - confirm->height() ) / 2) );
  confirm->setWindowTitle( sel_count > 1 ? tr( "Delete multiple files: " ) + sel_count + tr( "?" ) : tr( "Delete one file?" ) );

  int yes_to_all = 0;

  int not_moved = 0;
  int moved_ok_count = 0;

  QTreeWidgetItem *item;
  for( int i = 0; i < sel_count; i++ )
    {
    item = selected[i];
    if( item->text( 0 ) == ITEM_TYPE_DIR ) continue;

    QString file_name = cdir.absolutePath() + "/" + item->text( 1 );

    if( ! yes_to_all )
      {
      QImage im;
      im.load( file_name );
      im = im.scaled( QSize( 256, 256 ), Qt::KeepAspectRatio );
      QPixmap pm( 256, 256 );
      pm = pm.fromImage( im );

      confirm->cd.file_name->setText( item->text( 1 ) );
      confirm->cd.pixmap->setPixmap( pm );
      //confirm->cd.buttonBox->button( QDialogButtonBox::YesToAll )->setEnabled( sel_count > 1 );
      confirm->exec();

      //qDebug() << "RESBUTTON: " << QVariant( confirm->activated_button ).toString();

      if( confirm->activated_button == QDialogButtonBox::NoButton ) break;
      if( confirm->activated_button == QDialogButtonBox::Cancel   ) break;
      if( confirm->activated_button == QDialogButtonBox::No       ) continue;
      if( confirm->activated_button == QDialogButtonBox::YesToAll )
        yes_to_all = 1;
      }

    //qDebug() << "DELETE YES: " << file_name << " RESBUTTON: " << QVariant( confirm->activated_button ).toString();

    QString path_hash = QVariant( QCryptographicHash::hash( cdir.absolutePath().toAscii(), QCryptographicHash::Sha1 ).toHex() ).toString();
    QString trash_file_name = trash_can + "/" + path_hash + "." + item->text( 1 );

    int moved_ok = 0;
    if( trashdir.exists( trash_file_name ) )
      QFile::remove( trash_file_name );
    if( trashdir.rename( file_name, trash_file_name ) )
      {
      moved_ok = 1;
      }
    else
      if( QFile::copy( file_name, trash_file_name ) )
        {
        QFile::remove( file_name );
        moved_ok = 1;
        }
      else
        {
        not_moved++;
        }

    if( moved_ok )
      {
      tree->takeTopLevelItem( tree->indexOfTopLevelItem( item ) );
      moved_ok_count++;
      }

    //qDebug() << "DELETE FILE: " << file_name;
    //qDebug() << "TRASH  FILE: " << trash_file_name;
    //qDebug() << "--------------------------------------------------";
    }

  delete confirm;

  return moved_ok_count;
};


/*****************************************************************************/

void QvvMainWindow::keyPressEvent ( QKeyEvent * e )
{

  e->accept();
  if( e->modifiers() & Qt::CTRL )
    {
    switch( e->key() )
      {
      case Qt::Key_Right : enterAllSelected(); break;
      default: e->ignore(); QMainWindow::keyPressEvent( e ); break;
      }
    }
  else if( e->modifiers() & Qt::ALT )
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
      case Qt::Key_Right : enter( tree->currentItem() ); break;

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
    QMenu    *menu;
    toolbar = new QToolBar( this );

    toolbar->setVisible( opt_use_toolbar ? 1 : 0 );

    menu = menuBar()->addMenu( tr("&File") );

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

    action = menu->addAction( tr("&Delete selected images"), this, SLOT(slotDeleteSelected()), Qt::Key_Delete );
    //action->setIcon( QIcon( ":/images/view-refresh.png" ) );

    action = menu->addAction( tr("&Quit"), this, SLOT(close()), Qt::AltModifier + Qt::Key_X );
    action->setIcon( QIcon( ":/images/system-log-out.png" ) );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&View"));

    action = menu->addAction( tr("&Toggle toolbar"), this, SLOT(slotToggleToolbar()) );

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

    action = menu->addAction( tr("Go to p&arent directory"), this, SLOT(slotGoUp()), Qt::Key_Backspace );
    action->setIcon( QIcon( ":/images/go-up.png" ) );
    toolbar->addAction( action );

    action = menu->addAction( tr("Change &directory"), this, SLOT(slotChangeDir()), Qt::AltModifier + Qt::Key_D );
    action->setIcon( QIcon( ":/images/folder.png" ) );
    toolbar->addAction( action );

    action = menu->addAction( tr("Go to &home directory"), this, SLOT(slotHomeDir()), Qt::AltModifier + Qt::Key_Home );
    action->setIcon( QIcon( ":/images/go-home.png" ) );
    toolbar->addAction( action );

    menu->addSeparator();

    action = menu->addAction( tr("Go to &Random image"),   this, SLOT(slotRandomItem()), Qt::Key_Asterisk );
    action = menu->addAction( tr("Activate current item"),  this, SLOT(enterCurrent()), Qt::Key_Right );

    action = menu->addAction( tr("Display all &selected images"),  this, SLOT(enterAllSelected()), Qt::ControlModifier + Qt::Key_Right );
    action->setIcon( QIcon( ":/images/view_all.png" ) );
    toolbar->addAction( action );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&Window"));

    action = menu->addAction( tr("&New browser window"), this, SLOT(slotNewWindow()), Qt::Key_F3 );
    action->setIcon( QIcon( ":/images/window-new.png" ) );
    toolbar->addAction( action );

    menu->addAction( tr("&Close window"), this, SLOT(close()), Qt::Key_F4 );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&Settings"));

    action = menu->addAction( tr("Create thumbnails if needed") );
    action->setCheckable( true );
    action->setChecked( opt_create_thumbs );
    connect( action, SIGNAL( toggled(bool) ), this, SLOT(slotCreateThumbs()) );

    action = menu->addAction( tr("Create smooth thumbnails") );
    action->setCheckable( true );
    action->setChecked( opt_create_smooth_thumbs );
    connect( action, SIGNAL( toggled(bool) ), this, SLOT(slotSmoothThumbs()) );

    action = menu->addAction( tr("Create new thumbnails in JPEG for JPEGs") );
    action->setCheckable( true );
    action->setChecked( opt_create_jpeg_thumbs );
    connect( action, SIGNAL( toggled(bool) ), this, SLOT(slotJPEGThumbs()) );

    menu->addSeparator();

    action = menu->addAction( tr("Use toolbar upon startup") );
    action->setCheckable( true );
    action->setChecked( opt_use_toolbar );
    connect( action, SIGNAL( toggled(bool) ), this, SLOT(slotUseToolbar()) );

    /*--------------------------------------------------------------------*/

    menu = menuBar()->addMenu( tr("&Help") );

    action = menu->addAction( tr("&Help Contents"), this, SLOT(slotHelp()), Qt::Key_F1 );
    action->setIcon( QIcon( ":/images/help-browser.png" ) );
    toolbar->addAction( action );

    action = menu->addAction( tr("&About"),  this, SLOT(slotAbout()), Qt::AltModifier + Qt::Key_A );
    action->setIcon( QIcon( ":/images/face-glasses.png" ) );
    toolbar->addAction( action );

    /*--------------------------------------------------------------------*/

    addToolBar( toolbar );
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
