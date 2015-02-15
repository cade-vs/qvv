/****************************************************************************
**
**  QVV Image Viewer
**  1999-2009 (c) Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <cade@biscom.net> <cade@epay.bg> <cade@datamax.bg>
**  http://cade.datamax.bg/qvv/
**
****************************************************************************/

#ifndef _QVV_HELP_H_
#define _QVV_HELP_H_

#include <QTextBrowser>

class QvvHelpBrowser : public QTextBrowser
{
    Q_OBJECT

  public:
    QvvHelpBrowser();

  protected:
    void keyPressEvent ( QKeyEvent * e );
};

extern QvvHelpBrowser *help_browser;

void display_help();

#endif
