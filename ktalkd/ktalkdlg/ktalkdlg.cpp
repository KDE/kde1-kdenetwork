#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
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
    // argv[1] : user@host (caller)
    // argv[2] (optionnal) : callee, non existent

    KApplication a (argc, argv, "ktalkannounce"); // we want to read ktalkannouncerc

    struct timeval clock;
    struct timezone zone; 
    gettimeofday (&clock, &zone);
    struct tm *localclock = localtime ((const time_t *) &clock.tv_sec);

    QString s;
    s.sprintf ("%d:%02d", localclock->tm_hour, localclock->tm_min);
    s = i18n ("Message from talk demon at ") + s + " ...\n" +
        i18n ("Talk connection requested by ") + argv [1];

    if ( argc==3 )
    {
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

    // don't erase this! - ktalkd waits for it!
    printf("#\n");
    fflush(stdout);
    
    KConfig *cfg = a.getConfig();
    cfg->setGroup ("ktalkannounce");
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
	cmd += " '";
        cmd += argv[1];
        cmd += "' &";

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
