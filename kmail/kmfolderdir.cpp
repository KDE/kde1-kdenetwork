// kmfolderdir.cpp

#include <qdir.h>

#include "kmfolderdir.h"
#include "kmfolder.h"
#include <kapp.h>

#include <assert.h>
#include <qfile.h>
#include <qfileinf.h>
#include <errno.h>


//=============================================================================
//=============================================================================
KMFolderRootDir::KMFolderRootDir(const char* path):
  KMFolderDir(NULL, path)
{
  initMetaObject();

  setPath(path);
}

//-----------------------------------------------------------------------------
KMFolderRootDir::~KMFolderRootDir()
{
  // WABA: We can't let KMFolderDir do this because by the time its
  // desctructor gets called, KMFolderRootDir is already destructed
  // Most notably the path.
  clear();
}


//-----------------------------------------------------------------------------
void KMFolderRootDir::setPath(const char* aPath)
{
  mPath = aPath;
  mPath.detach();
}


//-----------------------------------------------------------------------------
const QString& KMFolderRootDir::path(void) const
{
  return mPath;
}



//=============================================================================
//=============================================================================
KMFolderDir::KMFolderDir(KMFolderDir* parent, const char* name):
  KMFolderNode(parent,name), KMFolderNodeList()
{
  initMetaObject();

  setAutoDelete(TRUE);

  mType = "dir";
}


//-----------------------------------------------------------------------------
KMFolderDir::~KMFolderDir()
{
  clear();
}


//-----------------------------------------------------------------------------
KMFolder* KMFolderDir::createFolder(const char* aFolderName, bool aSysFldr)
{
  KMFolder* fld;
  int rc;

  assert(aFolderName != NULL);

  fld = new KMFolder(this, aFolderName);
  assert(fld != NULL);

  fld->setSystemFolder(aSysFldr);

  rc = fld->create();
  if (rc)
  {
    warning(i18n("Error while creating folder `%s':\n%s"),
	    aFolderName, strerror(rc));
    delete fld;
    return NULL;
  }

  KMFolderNode *node;
  int idx = 0;
  for(node = first(); node; node=next(), idx++)
  {
     if (strcasecmp(node->name().data(),aFolderName) > 0)  break;
  } 
  insert(idx, fld);

  return fld;
}


//-----------------------------------------------------------------------------
const QString& KMFolderDir::path(void) const
{
  static QString p;

  if (parent())
  {
    p = parent()->path();
    p.append("/");
    p.append(name());
  }
  else p = "";

  return p;
}


//-----------------------------------------------------------------------------
bool KMFolderDir::reload(void)
{
  QDir      dir;
  //KMFolderDir* folderDir;
  KMFolder* folder;
  QFileInfo* fileInfo;
  QFileInfoList* fiList;
  QString fname;
  QString fldPath;

  clear();
  
  fldPath = path();
  //dir.setFilter(QDir::Files | QDir::Hidden);
  dir.setNameFilter("*");
  
  if (!dir.cd(fldPath, TRUE))
  {
    warning("Cannot enter directory '" + fldPath + "'.\n");
    return FALSE;
  }

  if (!(fiList=(QFileInfoList*)dir.entryInfoList()))
  {
    warning("Directory '" + fldPath + "' is unreadable.\n");
    return FALSE;
  }

  for (fileInfo=fiList->first(); fileInfo; fileInfo=fiList->next())
  {
    fname = fileInfo->fileName();

    if (fname[0]=='.') // skip table of contents files
      continue;

    else if (fileInfo->isDir()) // a directory
    {
      // Ignore subfolders - they do not work anyway
      debug ("Subfolder ignored");
      continue;
      //folderDir = new KMFolderDir(this, fname);
      //append(folderDir);
    }

    else // all other files are folders (at the moment ;-)
    {
      folder = new KMFolder(this, fname);
      append(folder);
    }
  }
  //delete fiList;

  return TRUE;
}

#include "kmfolderdir.moc"
