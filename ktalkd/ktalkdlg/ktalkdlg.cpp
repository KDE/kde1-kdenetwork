#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <qstring.h>

#include <kapp.h>
#include <kmsgbox.h>

extern "C" {
#include <mediatool.h>
} 
#include <kaudio.h>

#define RING_WAIT 30
#define MAX_DLG_LIFE RING_WAIT
/* so that the dialog lasts exactly the time of an announce */

class TimeoutDialog : public KMsgBox {
  public:
    TimeoutDialog (int timeout_ms, QWidget *parent=0, const char *caption=0,
                   const char *message=0, int flags=INFORMATION, 
                   const char *b1text=0, const char *b2text=0,
                   const char *b3text=0, const char *b4text=0):
            KMsgBox (parent, caption, message, flags, 
                     b1text, b2text, b3text, b4text)
            {startTimer (timeout_ms);}

    ~TimeoutDialog () 
            {killTimers ();}

    virtual void timerEvent (QTimerEvent *)
            {killTimers (); done (Rejected);}
};

int main (int argc, char **argv) {
    // if one arg :
    // argv[1] : user@host (caller)

    // if three args :
    // argv[1] : --user or whatever
    // argv[2] : callee, non existent
    // argv[3] : user@host (caller)

    KApplication a (argc, argv, "ktalkd"); // we want to read ktalkdrc

    struct timeval clock;
    struct timezone zone; 
    gettimeofday (&clock, &zone);
    struct tm *localclock = localtime ((const time_t *) &clock.tv_sec);

    QString s;
    s.sprintf ("%d:%02d", localclock->tm_hour, localclock->tm_min);
    s = i18n ("Message from talk demon at ") + s + " ...\n" +
        i18n ("Talk connection requested by ") + argv [argc-1];

    if ( argc==4 )		// called with a set-user-name option by announce.c
    {				// we assume, the username is at 2nd position ...
      s += '\n';
      s += i18n ("for user ");
      
      if ( *argv[ 2 ] == '\000' ) s += "<nobody>";
      else                        s += argv[ 2 ];
    }
    
    s += ".";

    TimeoutDialog dialog (MAX_DLG_LIFE * 1000, 0,
                          i18n ("Talk requested..."), s,
                          TimeoutDialog::INFORMATION,
                          i18n ("Respond"), i18n ("Let it be"));

    a.setTopWidget (&dialog);

    debug ("#"); // don't erase this! - ktalkd waits for it!
    

// debug code, to print locale found :
#if 0
    const char *g_lang = getenv("LANG"); 
    char syscmd[200];
    if (!g_lang)
        sprintf ( syscmd, "echo LANG : not set... >/tmp/atdlg_debug"); 
    else
        sprintf ( syscmd, "echo LANG : %s >/tmp/atdlg_debug", g_lang); 
    system(syscmd);
    sprintf ( syscmd, "echo language : %s >>/tmp/atdlg_debug", 
              (const char *) a.getLocale()->language() );
    system(syscmd);
#endif

    KConfig *cfg = a.getConfig();
    cfg->setGroup ("ktalkd");
    bool bSound = cfg->readNumEntry ("Sound", 0);
    KAudio *audio = 0;

    if (bSound) {
      QString soundFile = cfg->readEntry ("SoundFile");
      if (soundFile[0] != '/') soundFile.prepend(a.kde_sounddir () + "/");

      if (!soundFile.isEmpty ()) {
        audio = new KAudio ();
        if (audio->serverStatus() == 0) {
            audio->play ((char *) (const char *) soundFile);             
        } else {
            delete audio;                  
            audio = 0;
        }
      }
    }
    if (!audio) a.beep ();  // If no audio is played (whatever reason), beep!

    int result = dialog.exec ();
    if (result == 1) {
	dialog.killTimers ();
        debug ("Running talk client...");

	                              // KDEBINDIR is set by ktalkd
        QString cmd0 = cfg->readEntry ("talkprg", "$KDEBINDIR/kvt -e talk");
	
	QString cmd = cmd0.stripWhiteSpace();
	for ( int i=1; i<argc; ++i ) 
	{
	  cmd += " '";
	  cmd += argv[i];
	  cmd += '\'';
	}
        cmd += " &";
	
        debug(cmd);

        // Open /dev/null for stdin, stdout and stderr:
        int fd=open("/dev/null", O_RDWR);
        for (int i = 0; i <= 2; i++) {
            dup2(fd, i);
        }
        system (cmd.data ());
        kapp->quit();
    }

    if (audio) {
        audio->sync ();  
        delete audio;
    }
    return 0;
}
