#include "rules.h"


Rule::Rule(const char *_name=0,const char *expr=0,Field _field,
           bool casesen,bool wildmode)
{
    regex=QRegExp(expr,casesen,wildmode);
    name=_name;
    field=_field;
}

Rule::Rule(const char *_name=0,const char *expr=0,const char *_field,
           bool casesen,bool wildmode)
{
    regex=QRegExp(expr,casesen,wildmode);
    name=_name;
    if (!strcmp(_field,"Sender"))
        field=Sender;
    else if (!strcmp(_field,"Subject"))
        field=Subject;
    else if (!strcmp(_field,"Cached Header"))
        field=Header;
    else if (!strcmp(_field,"Header"))
        field=Header2;
    else if (!strcmp(_field,"Cached Body"))
        field=Body;
    else if (!strcmp(_field,"Body"))
        field=Body2;
    else if (!strcmp(_field,"Cached Article"))
        field=All;
    else if (!strcmp(_field,"Article"))
        field=All2;
}

Rule::~Rule()
{
}


MessageParts Rule::missingParts()
{
    int parts=PART_NONE;

    if (field==Header2)
        parts=parts|PART_HEAD;
    else if (field==Body2)
        parts=parts|PART_BODY;
    else if (field==All2)
        parts=parts|PART_ALL;
    return (MessageParts)parts;
}


bool Rule::match(const Article art,NNTP *server)
{
    bool matches=false;
    if (server)
        server->getMissingParts(missingParts(),art.ID);

    debug ("missingparts-->%d",missingParts());

    if (field==Subject)
    {
        if (regex.match(art.Subject.data())>-1)
        {
            matches=true;
        }
    }
    else if (field==Sender)
    {
        if (regex.match(art.From.data())>-1)
        {
            matches=true;
        }
    }
    else if ((field==Header && (server->isCached(art.ID)&PART_HEAD)) ||
            (field==Header2))
    {
        QString *data=server->head(art.ID);
        if (regex.match(data->data())>-1)
        {
            matches=true;
        }
    }

    else if ((field==Body && (server->isCached(art.ID)&PART_BODY)) ||
            (field==Body2))
    {
        QString *data=server->body(art.ID);
        if (regex.match(data->data())>-1)
        {
            matches=true;
        }
    }

    else if ((field==All && (server->isCached(art.ID)&PART_ALL)) ||
            (field==All2))
    {
        QString *data=server->article(art.ID);
        if (regex.match(data->data())>-1)
        {
            matches=true;
        }
    }
    
    return matches;
}
