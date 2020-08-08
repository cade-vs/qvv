/****************************************************************************
**
**  QVV Image Viewer
**  Copyright (c) 1999-2020 Vladi Belperchinov-Shabanski
**  <cade@bis.bg> <shabanski@gmail.com> <cade@cpan.org>
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
