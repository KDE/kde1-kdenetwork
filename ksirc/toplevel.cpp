/*
 * Basic toplevel widget control
 * Need items:
 * 1. QPopup menu controls for the listbox
 * 2. Lots of items
 * 3. Make it looks cooler
 *
 */

#include "toplevel.h"
#include "iocontroller.h"
#include "open_top.h"
#include "control_message.h"
#include "config.h"
#include <iostream.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <knewpanner.h>

extern KConfig *kConfig;
extern KApplication *kApp;

//QPopupMenu *KSircTopLevel::user_controls = 0L;
QList<UserControlMenu> KSircTopLevel::user_menu;
QList<QPopupMenu> KSircTopLevel::user_menu_list;
QPixmap *KSircTopLevel::pix_info = 0L;
QPixmap *KSircTopLevel::pix_star = 0L;
QPixmap *KSircTopLevel::pix_bball = 0L;
QPixmap *KSircTopLevel::pix_greenp = 0L;
QPixmap *KSircTopLevel::pix_bluep = 0L;
QPixmap *KSircTopLevel::pix_madsmile = 0L;

KSircTopLevel::KSircTopLevel(KSircProcess *_proc, char *cname=0L, const char * name=0) 
  : KTopLevelWidget(name),
    KSircMessageReceiver(_proc)
   
{

  /*
   * Setup window for operation.  We don't handle io!
   */

  /*
   * QPopup title bar, does not actually do anything at this time
   */

  proc = _proc;

  channel_name = cname;
  if(channel_name){
    QString s = channel_name;
    int pos2 = s.find(' ', 0);
    if(pos2 > 0)
      channel_name = qstrdup(s.mid(0, pos2).data());
    
    setCaption(channel_name);
    caption = channel_name;
  }
  else
    caption = "";

  LineBuffer = new QStrList();
  Buffer = FALSE;

  have_focus = 0;
  
  QPopupMenu *file = new QPopupMenu();
  file->insertItem("&User Menu...", this, SLOT(startUserMenuRef()));
  file->insertItem("&New Window...", this, SLOT(newWindow()), CTRL + Key_N);
  file->insertSeparator();
  file->insertItem("&Ticker Mode", this, SLOT(showTicker()), CTRL + Key_T);
  file->insertSeparator();
  file->insertItem("&Quit", this, SLOT(terminate()), CTRL + Key_Q );

  KMenuBar *menu = new KMenuBar(this, "menubar");
  menu->insertItem("&File", file, 2, -1);
  setMenu(menu);

  /*
   * Ok, let's look at the basic widge t "layout"
   * Everything belongs to q QFrame F, this is use so we
   * can give the KApplication a single main client widget, which is needs.
   *
   * A QVbox and a QHbox is used to ctronl the 3 sub widget
   * The Modified QListBox is then added side by side with the User list box.
   * The SLE is then fit bello.
   */

  QFrame *f = new QFrame(this, "frame");
  setView(f);  // Tell the KApplication what the main widget is.

  QVBoxLayout *gm = new QVBoxLayout(f, 5); // Main top layout
  QHBoxLayout *gm2 = new QHBoxLayout(10);   // Layout for users text and users box
  gm->addLayout(gm2);

  //  mainw = new QListBox(f, "mle");          // Make a flat QListBox.  I want the

  if(kSircConfig->colour_background == 0){
    kConfig->setGroup("Colours");
    kSircConfig->colour_background = new QColor(kConfig->readColorEntry("Background", new QColor(colorGroup().mid())));
  }

  pan = new KNewPanner(f, "knewpanner", KNewPanner::Vertical,
		       KNewPanner::Absolute, width()-1000);
  gm2->addWidget(pan, 10);

  mainw = new KSircListBox(pan, "mle");
  mainw->setFocusPolicy(QWidget::NoFocus); // Background and base colour of
  mainw->setEnabled(FALSE);                // the lb to be the same as the main
  mainw->setSmoothScrolling(TRUE);         // ColourGroup, but this is BAD BAD
  mainw->setFont(kSircConfig->defaultfont);// Since we don't use KDE requested

  connect(mainw, SIGNAL(updateSize()),
	  this, SIGNAL(changeSize()));
  QColorGroup cg = QColorGroup(colorGroup().foreground(), colorGroup().mid(), 
    			       colorGroup().light(), colorGroup().dark(),
  			       colorGroup().midlight(), 
  			       colorGroup().text(), *kSircConfig->colour_background); 
  mainw->setPalette(QPalette(cg,cg,cg));   // colours.  Font it also hard coded
  mainw->setMinimumWidth(width() - 100);
  //  gm2->addWidget(mainw, 10);               // which is bad bad.

  nicks = new aListBox(pan, "qlb");          // Make the users list box.
  //  nicks->setMaximumWidth(100);             // Would be nice if it was flat and
  //  nicks->setMinimumWidth(100);             // matched the main text window
  nicks->setPalette(QPalette(cg,cg,cg));   // HARD CODED COLOURS AGAIN!!!!
  //  gm2->addWidget(nicks, 0);

  pan->activate(mainw, nicks);
  pan->setAbsSeparatorPos(width() - 100);
  //  mainw->setMinimumWidth(0);             // matched the main text window

  //  linee = new QLineEdit(f, "qle");
  linee = new aHistLineEdit(f, "qle");     // aHistEdit is a QLineEdit with 
  linee->setMaximumHeight(this->fontMetrics().height()+5); // arrows for 
  linee->setFocusPolicy(QWidget::StrongFocus); // scroll back abillity
  linee->setPalette(QPalette(cg,cg,cg));   // HARD CODED COLOURS AGAIN!!!! (last time I hope!)
  connect(linee, SIGNAL(gotFocus()),
	  this, SLOT(gotFocus()));
  connect(linee, SIGNAL(lostFocus()),
	  this, SLOT(lostFocus()));
  gm->addWidget(linee);                    // No special controls are needed.

  connect(linee, SIGNAL(returnPressed()), // Connect return in sle to send
  	  this, SLOT(sirc_line_return()));// the line to dsirc
 
  linee->setFocus();  // Give SLE focus

  lines = 0;          // Set internal line counter to 0
  contents.setAutoDelete( TRUE ); // Have contents, the line holder nuke everything on exit

  ticker = new KSTicker(0, "ticker", WStyle_NormalBorder);
  ticker->setCaption(channel_name);
  kConfig->setGroup("TickerDefaults");
  ticker->setFont(kConfig->readFontEntry("font", new QFont("fixed")));
  ticker->setSpeed(kConfig->readNumEntry("tick", 30), 
		   kConfig->readNumEntry("step", 3));
  connect(ticker, SIGNAL(doubleClick()), 
	  this, SLOT(unHide()));
  connect(ticker, SIGNAL(closing()), 
	  this, SLOT(terminate()));

  /*
   * Set generic run time variables
   *
   */

  opami = FALSE;
  continued_line = FALSE;

  /*
   * Load basic pics and images
   * This should use on of the KDE finder commands
   * CHANGE THIS AWAY FROM HARD CODED PATHS!!!
   */

  if(pix_info == 0){
    pix_info = new QPixmap("./img/info.gif");
    pix_star = new QPixmap("./img/star.gif");
    pix_bball = new QPixmap("./img/blueball.gif");
    pix_greenp = new QPixmap("./img/greenpin.gif");
    pix_bluep = new QPixmap("./img/bluepin.gif");
    pix_madsmile = new QPixmap("./img/madsmiley.gif");
  }

  /* 
   * Create the user Controls popup menu, and connect it with the
   * nicks list on the lefthand side (highlighted()).
   * 
   */

  //  if(!user_controls)
  //    user_controls = new QPopupMenu();

  user_controls = new QPopupMenu();
  user_menu_list.append(user_controls);
  menu->insertItem("&Users", user_controls);

  if(user_menu_list.count() < 2)
    initPopUpMenu();

  connect(user_controls, SIGNAL(activated(int)), this, SLOT(UserParseMenu(int)));

  connect(nicks, SIGNAL(rightButtonPress(int)), this,
	  SLOT(UserSelected(int)));


  UserUpdateMenu();  // Must call after each change to either user_menu_list or user_controls

  accel = new QAccel(this, "accel");

  accel->connectItem(accel->insertItem(SHIFT + Key_PageUp),
		     this,
		     SLOT(AccelScrollUpPage()));
  accel->connectItem(accel->insertItem(SHIFT + Key_PageDown),
		     this,
		     SLOT(AccelScrollDownPage()));

  accel->connectItem(accel->insertItem(CTRL + Key_Enter),
		     this,
		     SLOT(AccelPriorMsgNick()));

  accel->connectItem(accel->insertItem(CTRL + SHIFT + Key_Enter),
		     this,
		     SLOT(AccelNextMsgNick()));

  accel->connectItem(accel->insertItem(CTRL + Key_Return),
		     this,
		     SLOT(AccelPriorMsgNick()));

  accel->connectItem(accel->insertItem(CTRL + SHIFT + Key_Return),
		     this,
		     SLOT(AccelNextMsgNick()));

}


KSircTopLevel::~KSircTopLevel()
{

  // Cleanup and shutdown
  writePopUpMenu();
  //  if(this == proc->getWindowList()["default"])
  //    write(sirc_stdin, "/quit\n", 7); // tell dsirc to close
  //
  //  if(proc->getWindowList()[channel_name])
  //    proc->getWindowList().remove(channel_name);

  if((channel_name[0] == '#') || (channel_name[0] == '&'))
    emit outputLine(QString("/part ") + channel_name + "\n");

  int tick, step;
  ticker->speed(&tick, &step);
  kConfig->setGroup("TickerDefaults");
  kConfig->writeEntry("font", ticker->font());
  kConfig->writeEntry("tick", tick);
  kConfig->writeEntry("step", step);
  kConfig->sync();

  //  close(sirc_stdin);  // close all the pipes
  //  close(sirc_stdout); // ditto
  //  close(sirc_stderr); // duh... ;)

}

//void KSircTopLevel::sirc_stop(bool STOP = FALSE)
//{
//  if(STOP == TRUE){
//    Buffer = TRUE;
//  }
//  else{
//    Buffer = FALSE;
//    if(LineBuffer->isEmpty() == FALSE)
//      sirc_receive(QString(""));
//  }
//}
  
void KSircTopLevel::sirc_receive(QString str)
{

  /* 
   * read and parse output from dsirc.
   * call reader, split the read input into lines, prase the lines
   * then print line by line into the main text area.
   *
   * PROBLEMS: if a line terminates in mid line, it will get borken oddly
   *
   */

  /*
   * If we have to many lines, nuke the top 100, leave us with 100
   */

  int lines = 0;

  if(Buffer == FALSE){
    if(LineBuffer->count() >= 2){
      mainw->setAutoUpdate(FALSE);
    }
    
    if(str.isEmpty() == FALSE){
      LineBuffer->append(str);
    }

    ircListItem *item = 0;
    char *pchar;
    QString string;
    bool update = FALSE;

    for(pchar = LineBuffer->first(); pchar != 0; pchar=LineBuffer->next()){
      // Get the need list box item, with colour, etc all set
      string = pchar;
      item = parse_input(string);
      // If we shuold add anything, add it.
      // Item might be null, if we shuoold ingore the line
      
      if(item){
	// Insert line to the end
	connect(this, SIGNAL(changeSize()),
		item, SLOT(updateSize()));
	mainw->insertItem(item, -1);
	ticker->mergeString(item->getText() + " // ");
	lines++; // Mode up lin counter
	update = TRUE;
      }
//      if(mainw->count() > 100)
//	mainw->removeItem(0);
    }
    LineBuffer->clear(); // Clear it since it's been added

    if(mainw->count() > 200){
        mainw->setAutoUpdate(FALSE);
        update = TRUE;
        while(mainw->count() > 100)
           mainw->removeItem(0);
	mainw->update();
    }

    // If we need to scroll, we, scroll =)
    mainw->scrollToBottom();

    if(mainw->autoUpdate() == FALSE){
      mainw->setAutoUpdate(TRUE);
      mainw->repaint();
      //      mainw->repaint(TRUE);
      //mainw->update();
    }
    
  }
  else{
    LineBuffer->append(str);
  }
}


void KSircTopLevel::sirc_line_return()
{

  /* Take line from SLE, and output if to dsirc */

  QString s = linee->text();

  if(s.length() == 0)
    return;

  s += '\n'; // Append a need carriage return :)

  /*
   * Parse line forcommand we handle
   */

  if(strncmp(s, "/join ", 6) == 0){
    s = s.lower();
    int pos2 = s.find(' ', 6);
    if(pos2 == -1)
      pos2 = s.length() - 1;
    if(pos2 > 6){
      QString name = s.mid(6, pos2 - 6); // make sure to remove line feed
      //cerr << "New channel: " << name << endl;
      emit open_toplevel(name);
      if(name[0] != '#'){
	linee->setText("");
	return;
      }
    }
  }

  // 
  // Look at the command, if we're assigned a channel name, default
  // messages, etc to the right place.  This include /me, etc
  //

  if((uint) nick_ring.at() < (nick_ring.count() - 1))
    nick_ring.next();
  else
    nick_ring.last();

  sirc_write(s);

  linee->setText("");
  
}

void KSircTopLevel::sirc_write(QString &str)
{
  if(channel_name[0] != '!'){
    if(str[0] != '/'){
      str.prepend(QString("/msg ") + channel_name + QString(" "));
    }
    else if(strnicmp(str, "/me", 3) == 0){
      str.remove(0, 3);
      str.prepend(QString("/de ") + channel_name);
    }
  }
  
  // Write out line

  //  proc->stdin_write(str);
  emit outputLine(str);

}

ircListItem *KSircTopLevel::parse_input(QString &string)
{

  /* 
   * welcome to the twilight zone
   * Big time parsing, and no docs
   * Variables are reused unsafly, big EWWW!
   */

  /* 
   * Parsing routines are broken into 3 majour sections 
   *
   * 1. Search and replace evil characters. The string is searched for
   * each character in turn (evil[i]), and the character string in
   * evil_rep[i] is replaced.  
   *
   * 2. Parse control messages, add pixmaps, and colourize required
   * lines.  Caption is also set as required.
   *
   * 3. Create and return the ircListItem.
   *
   */

  /*
   * c: main switch char, used to id *'s and ssfe commands
   * s2: temporary string pointer, used is parsing title string
   * evil[]: list of eveil characters that should be removed from 
   *         the data stream.
   * evil_rep[][], list of characters to fill replace evil[] with
   * s3: temp Qstring
   * s4: temp QString
   * pos: start position indicator, used for updating nick info
   * pos2: end position indicator, tail end for nick info
   * color: colour for ListBox line entry
   * pixmap: pixmap for left hand side of list box
   * */

  char c,*s2;

  // \n: clear any line feeds                              -> " "
  // \r: clear stray carriage returns                      -> ""
  // \002: control character of some kinda, used for bold? -> ""
  // \037: bold or underline, evil messy char              -> Not Used
  // \000: terminating null
  char evil[] = {'\n', '\r', '\002', '\037', '\000'};
  char *evil_rep[] = {
    " ",
    "",
    "",
    ""
  };
  QString s3, s4, channel;
  int pos, pos2;
  QColor *color = kSircConfig->colour_text;
  QPixmap *pixmap = NULL;

  /*
   * No-output get's set to 1 if we should ignore the line
   */

  int no_output = 0;

  for(int i = 0; evil[i] != 0; i++){
    pos = string.find(evil[i], 0, FALSE);  // look for first occurance
    while(pos >= 0){                       // If found, start stepping
      string.remove(pos, 1);               // Remove evil char
      string.insert(pos, evil_rep[i]);     // insert replacement
      pos = string.find(evil[i], pos+strlen(evil_rep[i]), FALSE);
					   // find next
    }
  }

  //  if(string[0] == '~'){
  //    pos = 1;
  //    pos2 = string.find("~", pos);
  //    if(pos2 > pos){
  //      channel = string.mid(pos, pos2-pos); // s3 now holds the channel name
  //      channel = channel.lower();           // lower case s3
  //      string.remove(pos-1, pos2-pos+2);
  //    }
  //  }
  //
  //  if(channel.isEmpty() == FALSE)
  //    cerr << "Channel Specefic: " << channel << endl;

  //  strncpy(&c, string.left(1), 1);          // Copy the first char into
					   // c, and then do a switch
					   // on it

  switch(string[0]){
  case '`':                                // ` is an ssfe command
    s2 = strstr(string, "#ssfe#");
    if(s2 > 0){
      s2+=6;                               // move ahead character end
					   // of `ssfe control
					   // message, switch on end
					   // of control char
      //      cerr << "s2: " << s2;
      switch(s2[0]){
      case 's':                            // moved [sirc] message
	s2+=10;                            // set the rest of the line
					   // to the caption
	if(strcmp(s2, caption) != 0){
	  if(s2[0] == '@')                 // If we're an op,, 
	                                   // update the nicks popup menu
	    opami = TRUE;                  // opami = true sets us to an op
	  else
	    opami = FALSE;                 // FALSE, were not an ops
	  UserUpdateMenu();                // update the menu
	  setCaption(s2);
	  ticker->setCaption(s2);
	  caption = qstrdup(s2);           // Make copy so we're not 
	                                   // constantly changing the title bar
	}
	no_output = 1;                     // Don't print caption
	break;
      case 'i':
	string.truncate(0);                // truncate string... set
					   // no output, what's i?
	no_output = 1;
	break;
      case 't':
	no_output = 1;
	pos = string.find("t/m ", 6);
	if(pos >= 0){
	  pos += 4;
	  pos2 = string.find(" ", pos);
	  if(pos2 == -1)
	    pos2 = string.length();
	  if(pos2 > pos){
	    if(!nick_ring.contains(string.mid(pos, pos2-pos))){
	      nick_ring.append(string.mid(pos, pos2-pos));
	      //cerr << "Appending: " << string.mid(pos, pos2-pos) << endl;
	      if(nick_ring.count() > 10)
		nick_ring.removeFirst();
	    }
	  }
	  break;
	}
      case 'o':
	no_output = 1;
	string.truncate(0);
	break;
      default:
	cerr << "Unkown ssfe command: " << string << endl;
	string.truncate(0);                // truncate string... set
	no_output = 1;
      }
    }
    break;                                 // stop ssfe controls...
  case '*': // Parse and control ssfe control messages...
    if(string.length() > 2){ // Chack string, make sure it's valid
      strncpy(&c, string.mid(2,1), 1); // We double check that the value isn't the nasty control char that seems to slip through the evil char check
      if(c == '\002'){
	string.remove(2,1);
	strncpy(&c, string.mid(2,1), 1);
      }
      string.remove(0, 1);

      if(string[0] != '#')                  // It's not a users line, so we're not on continuing
	continued_line = FALSE;             // a long user list

      c = string[0];
      switch(c){
      case '*':                             // * is an info message
	string.remove(0, 2);                // takes off the junk
	if(string.contains("Talking to")){
	  cerr << "Removing Talking to\n";
	  string.truncate(0);
	  no_output = 1;
        }
	pixmap = pix_info;                 // Use the I/blue cir pixmap
	color = kSircConfig->colour_info;   // Colour is blue for info
	break;
      case 'E':                            // E's an error message
	string.remove(0, 2);               // strip the junk
	pixmap = pix_madsmile;             // use the mad smiley
	color = kSircConfig->colour_error;  // set the clour to red
	break;
      case '#':                             // Channel listing of who's in it
	nicks->setAutoUpdate(FALSE);        // clear and update nicks
	if(continued_line == FALSE)
	  nicks->clear();
	continued_line = TRUE;
	pos = string.find(": ", 0, FALSE) + 1; // Find start of nicks
	while(pos > 0){                     // While there's nick to go...
	  pos2 = string.find(" ", pos + 1, FALSE); // Find end of nick
	  if(pos2 < pos)
	    pos2 = string.length();         // If the end's not found, 
	                                    // set to end of the string
	  s3 = string.mid(pos+1, pos2 - pos - 1); // Get nick
	  if(s3[0] == '@'){    // Remove the op part if set
	    s3.remove(0, 1);
	    ircListItem *irc = new ircListItem(s3, &red, nicks);
	    irc->setWrapping(FALSE);
	    nicks->inSort(irc);
	  }
	  else{
	    nicks->inSort(s3);
	  }
	  pos = string.find(" ", pos2, FALSE); // find next nick
	}
	nicks->setAutoUpdate(TRUE);         // clear and repaint the listbox
	nicks->repaint(TRUE);
	color = kSircConfig->colour_info;    // set to cyan colouring
	break;
      case '<':
	string.remove(0, 2);                // clear junk
	pixmap = pix_greenp;                // For joins and leave use green
	color = kSircConfig->colour_chan;    // Pin gets for joins
	
	// Multiple type of parts, a signoff or a /part
	// Each get's get nick in a diffrent localtion
	// Set we search and find the nick and the remove it from the nick list
	// 1. /quit, signoff, nick after "^Singoff: "
	// 2. /part, leave the channek, nick after "has left \w+$"
	// 3. /kick, kicked off the channel, nick after "kicked off \w+$"
	//

	if(string.contains("Signoff: ")){   // Nick is right after the ": "
	  pos = string.find("Signoff: ") + 9;
	  s3 = string.mid(pos, string.find(' ', pos) - pos);
	}
	else if(string.contains("has left")) // part
	  s3 = string.mid(1, string.find(' ', 1) - 1);
	else if(string.contains("kicked off")) // kick
	  s3 = string.mid(1, string.find(' ', 1) - 1);
	else{                                // uhoh, something we missed?
	  cerr << "String sucks: " << string << endl;
	  s3 = "";
	}
	no_output = 1;
	for(uint i = 0; i < nicks->count(); i++){ // Search and remove the nick
	  if(strcmp(s3, nicks->text(i)) == 0){
	    nicks->removeItem(i);
	    no_output = 0;
	  }
	}
	break;
      case '>':
	string.remove(0, 2);                   // remove junk 
	pixmap = pix_greenp;                   // set green pin
	color =   kSircConfig->colour_chan;     // set green
	s3 = string.mid(1, string.find(' ', 1) - 1); // only 1 type of join
	//	nicks->insertItem(s3, 0);      // add the sucker
	nicks->inSort(s3);
	break;
      case 'N':
	string.remove(0, 2);                   // remove the junk
	pixmap = pix_greenp;                   // set green pin
	color = kSircConfig->colour_chan;       // set freen
	s3 = string.mid(1, string.find(' ', 1) - 1); // find the old know
	pos = string.find("known as ") + 9;    // find the new nick
	s4 = string.mid(pos, string.length() - pos);
	//	cerr << s3 << "-" << s4 << endl;
	// search the list for the nick and remove it
	// since the list is source we should do a binary search...
	no_output = 1;            // don't display unless we find the nick
	for(uint i = 0; i < nicks->count(); i++){
	  if(strcmp(s3, nicks->text(i)) == 0){
	    no_output = 0;        // nick is in out list, so print the message
	    nicks->removeItem(i); // remove old nick
	    nicks->inSort(s4);    // add new nick in sorted poss
	                          // can't use changeItem since it
				  // doesn't maintain sort order
	  }
	}
	break;
      case ' ':
	string.remove(0, 1);      // * <something> use fancy * pixmap
	pixmap = pix_star;        // why? looks cool for dorks
	break;
      case '+':
	pos = string.find("Mode change \"+o ", 0);
	if(pos > 0){
	  pos += 16;
	  pos2 = string.find("\"", pos);
	  s3 = string.mid(pos, pos2-pos);
	  for(uint i = 0; i < nicks->count(); i++){
	    if(strcmp(s3, nicks->text(i)) == 0){
	      nicks->setAutoUpdate(FALSE);
	      nicks->removeItem(i);           // remove old nick
	      ircListItem *irc = new ircListItem(s3, &red, nicks);
	      irc->setWrapping(FALSE);
	      nicks->inSort(irc);    // add new nick in sorted pass,with colour
	      nicks->setAutoUpdate(TRUE);
	      nicks->repaint();
	    }
	  }
	}
	pos = string.find("Mode change \"-o ", 0);
	if(pos > 0){
	  pos += 16;
	  pos2 = string.find("\"", pos);
	  s3 = string.mid(pos, pos2-pos);
	  for(uint i = 0; i < nicks->count(); i++){
	    if(strcmp(s3, nicks->text(i)) == 0){
	      nicks->setAutoUpdate(FALSE);
	      nicks->removeItem(i);           // remove old nick
	      nicks->inSort(s3);    // add new nick in sorted pass,with colour
	      nicks->setAutoUpdate(TRUE);
	      nicks->repaint();
	    }
	  }
	}
      default:
	string.remove(0, 3);      // by dflt remove junk, and use a ball
	pixmap = pix_bball;       // ball isn't used else where so we
				  // can track down unkonws and add them
	color = kSircConfig->colour_info;
	//	cerr << "Unkoown control: " << c << endl;
      }
    }
    break;
  }

  // Go searching for URLs and highlight them


  if(no_output)                    // is no_output is null,return
				   // anull pointer
    return NULL;
  else                             // otherwise create a new IrcListItem...
    return new ircListItem(string,color,mainw,pixmap, TRUE);

  return NULL; // make compiler happy or else it complans about
	       // getting to the end of a non-void func
}

void KSircTopLevel::URLSelected(const char*, int)
{

}

void KSircTopLevel::UserSelected(int index)
{
  if(index >= 0){
    popup_have_control = TRUE;
    user_controls->popup(this->cursor().pos());
  }
  else{
    popup_have_control = FALSE;
  }
}

void KSircTopLevel::UserParseMenu(int id)
{
  if(nicks->currentItem() < 0){
    cerr << "Warning, dork at the helm Captain!\n";
    return;
  }
  QString s;
  s.sprintf(user_menu.at(id)->action, nicks->text(nicks->currentItem()));
  s.append("\n");
  sirc_write(s);
}

void KSircTopLevel::UserUpdateMenu()
{
  int i = 0;
  UserControlMenu *ucm;
  //  QPopupMenu *umenu;

  user_controls->clear();
  for(ucm = user_menu.first(); ucm != 0; ucm = user_menu.next(), i++){
    if(ucm->type == UserControlMenu::Seperator){
      user_controls->insertSeparator();
    }
    else{
      user_controls->insertItem(ucm->title, i);
      if(ucm->accel)
	user_controls->setAccel(i, ucm->accel);
      if((ucm->op_only == TRUE) && (opami == FALSE))
	user_controls->setItemEnabled(i, FALSE);
    }
  }
  /*
  for(umenu = user_menu_list.first(); umenu != 0; 
      umenu = user_menu_list.next()){
    umenu->clear();
    for(ucm = user_menu.first(), i=0; ucm != 0; ucm = user_menu.next(), i++){
      if(ucm->type == UserControlMenu::Seperator){
	umenu->insertSeparator();
      }
      else{
	umenu->insertItem(ucm->title, i);
	if(ucm->accel)
	  umenu->setAccel(i, ucm->accel);
	if((ucm->op_only == TRUE) && (opami == FALSE)){
	  umenu->setItemEnabled(i, FALSE);
	}
	cerr << channel_name << " " << opami << endl;
      }
    }
  }
  */
  writePopUpMenu();
}

void KSircTopLevel::startUserMenuRef()
{
   UserMenuRef *umr = new UserMenuRef(&user_menu);
   connect(umr, SIGNAL(updateMenu()), this, SLOT(UserUpdateMenu()));
   umr->show();
}

void KSircTopLevel::AccelScrollDownPage()
{
    mainw->pageDown();
}

void KSircTopLevel::AccelScrollUpPage()
{
    mainw->pageUp();
}
void KSircTopLevel::AccelPriorMsgNick()
{
  linee->setText(QString("/msg ") + nick_ring.current() + " ");

  if(nick_ring.at() > 0)
    nick_ring.prev();

}

void KSircTopLevel::AccelNextMsgNick()
{
  if(nick_ring.at() < ((int) nick_ring.count() - 1) )
    linee->setText(QString("/msg ") + nick_ring.next() + " ");
}

void KSircTopLevel::initPopUpMenu()
{

  kConfig->setGroup("UserMenu");

  int items = kConfig->readNumEntry("Number");

  user_menu.clear();

  if(items == 0){

    user_menu.setAutoDelete(TRUE);
    user_menu.append(new UserControlMenu("Abuse", 
					 "/me slaps %s arround with a small 50lb Unix Manual",
					 0, UserControlMenu::Text));
    user_menu.append(new UserControlMenu); // Defaults to a seperator
    user_menu.append(new UserControlMenu("Kick",
					 "/sk %s",
					 CTRL + Key_K,
					 UserControlMenu::Text,
					 TRUE));
    user_menu.append(new UserControlMenu("Ban",
					 "/ban %s",
					 CTRL + Key_B,
					 UserControlMenu::Text,
					 TRUE));
    user_menu.append(new UserControlMenu("UnBan",
					 "/unban %s",
					 CTRL + Key_B,
					 UserControlMenu::Text,
					 TRUE));
    user_menu.append(new UserControlMenu());
    user_menu.append(new UserControlMenu("Op",
					 "/op %s",
					 CTRL + Key_O,
					 UserControlMenu::Text,
					 TRUE));
    user_menu.append(new UserControlMenu("Deop",
					 "/deop %s",
					 CTRL + Key_B,
					 UserControlMenu::Text,
					 TRUE));
  }
  else{
    QString key;
    QString cindex;
    QString title;
    QString action;
    int accel;
    int type;
    int oponly;
    for(int i = 0; i < items; i++){
      cindex.sprintf("%d", i);
      key = "MenuType-" + cindex;
      type = kConfig->readNumEntry(key);
      if(type == UserControlMenu::Seperator)
	user_menu.append(new UserControlMenu());
      else if(type == UserControlMenu::Text){
	key = "MenuTitle-" + cindex;
	title = kConfig->readEntry(key);
	key = "MenuAction-" + cindex;
	action = kConfig->readEntry(key);
	key = "MenuAccel-" + cindex;
	accel = kConfig->readNumEntry(key); 
	key = "MenuOpOnly-" + cindex;
	oponly = kConfig->readNumEntry(key); 
	
	user_menu.append(new UserControlMenu(qstrdup(title.data()), qstrdup(action.data()), accel, type, (bool) oponly));
      }
    }
  }
}

void KSircTopLevel::writePopUpMenu()
{

  kConfig->setGroup("UserMenu");

  int items = (int) user_menu.count();

  kConfig->writeEntry("Number", items);

  QString key;
  QString cindex;
  UserControlMenu *ucm;
  int type;

  for(int i = 0; i < items; i++){
    ucm = user_menu.at(i);
    cindex.sprintf("%d", i);
    key = "MenuType-" + cindex;
    type = ucm->type;
    kConfig->writeEntry(key, (int) type);
    // Do nothing for a seperator since it defaults accross
    if(type == UserControlMenu::Text){
      key = "MenuTitle-" + cindex;
      kConfig->writeEntry(key, ucm->title);
      key = "MenuAction-" + cindex;
      kConfig->writeEntry(key, ucm->action);
      key = "MenuAccel-" + cindex;
      kConfig->writeEntry(key, (int) ucm->accel);
      key = "MenuOpOnly-" + cindex;
      kConfig->writeEntry(key, (int) ucm->op_only);
    }
  }
  kConfig->sync();

}

void KSircTopLevel::newWindow() 
{ 
  open_top *w = new open_top(); 
  connect(w, SIGNAL(open_toplevel(QString)),
	  this, SIGNAL(open_toplevel(QString)));
  w->show();
}

void KSircTopLevel::closeEvent(QCloseEvent *)
{
  emit closing(this, channel_name);
  delete this;
}

void KSircTopLevel::resizeEvent(QResizeEvent *e)
{
  mainw->setAutoUpdate(FALSE);
  KTopLevelWidget::resizeEvent(e);
//  cerr << "Updating list box\n";
  mainw->setTopItem(mainw->count()-1);
  pan->setAbsSeparatorPos(width()-100);
  mainw->setMinimumWidth(width() - 100);
  emit changeSize();
  mainw->scrollToBottom();
  mainw->setAutoUpdate(TRUE);
  repaint(TRUE);
}

void KSircTopLevel::gotFocus()
{
  if(have_focus == 0){
    if(channel_name[0] == '#')
      emit outputLine("/join " + QString(channel_name) + "\n");
    have_focus = 1;
    emit currentWindow(this);
    //    cerr << channel_name << " got focusIn Event\n";
  }

}

void KSircTopLevel::lostFocus()
{
  if(have_focus == 1){
    have_focus = 0;
    //    cerr << channel_name << " got focusIn Event\n";
  }

}

void KSircTopLevel::control_message(int command, QString str)
{
  switch(command){
  case CHANGE_CHANNEL: // 001 is defined as changeChannel
    channel_name = qstrdup(str.data());
    have_focus = 0;
    setCaption(channel_name);
    emit changeChannel("!default", channel_name);
    mainw->scrollToBottom();
    break;
  case STOP_UPDATES:
    Buffer = TRUE;
    break;
  case RESUME_UPDATES:
    Buffer = FALSE;
    if(LineBuffer->isEmpty() == FALSE)
      sirc_receive(QString(""));
    break;
  case REREAD_CONFIG:
    mainw->setFont(kSircConfig->defaultfont);
    emit changeSize();
    {
      QColorGroup cg = QColorGroup(colorGroup().foreground(), colorGroup().mid(), 
				   colorGroup().light(), colorGroup().dark(),
				   colorGroup().midlight(), 
				   colorGroup().text(), *kSircConfig->colour_background); 
      mainw->setPalette(QPalette(cg, cg, cg));
      nicks->setPalette(QPalette(cg, cg, cg));
      linee->setPalette(QPalette(cg, cg, cg));
    }
    repaint(TRUE);
    break;
  default:
    cerr << "Unkown control message: " << str << endl;
  }
}

void KSircTopLevel::showTicker()
{
  myrect = geometry();
  mypoint = pos();
  this->hide();
  if(tickerrect.isEmpty() == TRUE)
    ticker->show();
  else{
    ticker->setGeometry(tickerrect);
    ticker->recreate(0, 0, tickerpoint, TRUE);
  }
}

void KSircTopLevel::unHide()
{
  tickerrect = ticker->geometry();
  tickerpoint = ticker->pos();
  ticker->hide();
  this->setGeometry(myrect);
  this->recreate(0, getWFlags(), mypoint, TRUE);
  this->show();
  linee->setFocus();  // Give SLE focus
}
