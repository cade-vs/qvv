/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

#ifndef _QVV_H_
#define _QVV_H_

#include <QString>
#include <QDesktopWidget>
#include <QTextBrowser>
#include <QSettings>

#define QVV_VERSION   "4.02"

#define DEFAULT_EXTENSIONS_FILTER   ".JPG.JPEG.PNG.GIF.BMP.XPM.TIF.TIFF."

const QString ITEM_TYPE_DIR( "[DIR]" );

extern QDesktopWidget *Desktop;
extern int DeskW;
extern int DeskH;

extern QString extensions_filter;

extern QSettings Settings;

extern int opt_thumbs_size;
extern int opt_create_thumbs;
extern int opt_create_smooth_thumbs;
extern int opt_create_jpeg_thumbs;
extern int opt_use_toolbar;


#endif
