#ifndef RULES_H
#define RULES_H

#include <qregexp.h>
#include "NNTP.h"

class Rule
{
public:
    enum Field {Sender,Subject,Header,Body,All,Header2,Body2,All2};
    Rule(const char *name=0,const char *expr=0,Field field=Sender,
        bool casesen=false,bool wildmode=false);
    Rule(const char *name=0,const char *expr=0,const char *field=0,
         bool casesen=false,bool wildmode=false);
    ~Rule();
    bool match (const Article art,NNTP *server=0);
    MessageParts missingParts();
    QRegExp regex;
    Field field;
    QString name;
};

#endif