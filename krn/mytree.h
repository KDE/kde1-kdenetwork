#ifndef MY_TREE_H
#define MY_TREE_H

#include <ktreelist.h>

class MyTreeList : public KTreeList
{
public:
    MyTreeList(QWidget *parent = 0,
               const char *name = 0,
               WFlags f = 0) :
    KTreeList(parent , name , f) {};
    void setTopCell(int c) { KTreeList::setTopCell(c); }
};  

#endif