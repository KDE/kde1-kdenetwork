#ifndef RULES_H
#define RULES_H

#include <qregexp.h>

class Rule
{
public:
    enum {Sender,Subject} Field;
    Rule(const char *name);
    ~Rule();
    QRegExp expr;
};

#endif