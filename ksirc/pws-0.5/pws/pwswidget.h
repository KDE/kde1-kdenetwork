#ifndef PWSWIDGET_H 
#define PWSWIDGET_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <kapp.h>
#include <qwidget.h>
#include <qdict.h>
#include <qlistview.h>

#include "server.h"

class QListView;
class QListViewItem;
class QWidgetStack;
class PWSListView;

class PWSWidget : public QWidget
{
    Q_OBJECT
public:
    PWSWidget(QWidget *parent = 0, const char *name = 0);
    virtual ~PWSWidget();
    void loadServers();
    void createGeneralPage();
    void createServerPage(const char *name);
    PWSListView *list;
    QWidgetStack *stack;
    int increaser;
    QDict <QWidget> pages;
signals:
    void quitPressed(QObject *);
public slots:
    void quit();
    void accept();
    void addServer();
    void deleteServer();
    void restart();
    void flipPage(QListViewItem *item);
    void logWindow();
protected slots:
    void adjustStack(QWidget *w);
protected:
    virtual void resizeEvent(QResizeEvent *e);
private:
    PWSServer *server;
    QString current;
};

class PWSListView : public QListView
{
    Q_OBJECT
public:
    PWSListView ( QWidget * parent = 0, const char * name = 0 )
        : QListView(parent, name)
        {
        }
protected:
    virtual void resizeEvent(QResizeEvent *re);
};

#endif // PWSWIDGET_H 
