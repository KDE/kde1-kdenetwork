
// 0 is special "invalid character"
// Value > 0 indicates from dsirc -> ksirc
// Value < 0 indicates from ksirc -> dsirc

// Desc: INVALID command
// iWinId: not defined
// iArg: not defined
// cArg: not defined
#define PUKE_INVALID 0

// Desc: associantes server name with fd.
// iWinId: not defined, but pass unchanged
// iArg: not defined
// cArg: name of the server
#define PUKE_SETUP 1

// Desc: replies to make sure association was valid
// iWinId: not defined, but pass unchanged
// iArg: size of Message
// cArg: not defined
#define PUKE_SETUP_ACK -1


// Desc: sends the ack back.  Used for doing actions after returning to select.
// iWinId: window id
// iArg: not defined
// cArg: not define
#define PUKE_ECHO 5

// Desc: sends the ack back.  Used for doing actions after returning to select.
// iWinId: window id
// iArg: not defined
// cArg: not define
#define PUKE_ECHO_ACK -5


// From ksirc to sirc, event command unkown.
// There should be somewhere better for this!!!

#define PUKE_EVENT_UNKOWN -999


// Widget commands starts at 1000 and end at 10000

// Desc: create new widget
// iWinId: parent widget id
// iArg: widget type as defined by PWIDGET_*
// cArg: Must be return unchanged
#define PUKE_WIDGET_CREATE 1000

// Desc: ack for newly created new widget
// iWinId: new widget Id, 0 if failed
// iArg: widget type as defined by PWIDGET_*
// cArg: Returned unchanged
#define PUKE_WIDGET_CREATE_ACK -1000


// Desc: shows requested widget
// iWinId: widget to show
// iArg: not defined
// cArg: not define
#define PUKE_WIDGET_DELETE 1001

// Desc: ack for show requested widget
// iWinId: widget to show
// iArg: not defined
// cArg: not define
#define PUKE_WIDGET_DELETE_ACK -1001

// Desc: shows requested widget
// iWinId: widget to show
// iArg: not defined
// cArg: not define
#define PUKE_WIDGET_SHOW 1002

// Desc: ack for show requested widget
// iWinId: widget to show
// iArg: not defined
// cArg: not define
#define PUKE_WIDGET_SHOW_ACK -1002

// Desc: hidess requested widget
// iWinId: widget to hide
// iArg: not defined
// cArg: not define
#define PUKE_WIDGET_HIDE 1003

// Desc: ack for hide requested widget
// iWinId: widget to hide
// iArg: not defined
// cArg: not define
#define PUKE_WIDGET_HIDE_ACK -1003

// Desc: repaint widget
// iWinId: widget to repaint
// iArg: erase, 0 for false, 1 for true
// cArg: no defines
#define PUKE_WIDGET_REPAINT 1005

// Desc: repaint widget ack
// iWinId: widget to repaint
// iArg: not defined
// cArg: not defined
#define PUKE_WIDGET_REPAINT_ACK -1005

// Desc: update widget on next event loop
// iWinId: widget to repaint
// iArg: erase, 0 for false, 1 for true
// cArg: no defines
#define PUKE_WIDGET_UPDATE 1010

// Desc: repaint widget ack
// iWinId: widget to repaint
// iArg: not defined
// cArg: not defined
#define PUKE_WIDGET_UPDATE_ACK -1010

// Desc: resize the widget
// iWinId: widget to repaint
// iArg: width in the low 16 bit, height in next 16 bits
// cArg: not defined
#define PUKE_WIDGET_RESIZE 1015

// Desc: repaint widget ack
// iWinId: widget to repaint
// iArg: new widget in lower 16 bit, new height in lower 16 bits
// cArg: not defined
#define PUKE_WIDGET_RESIZE_ACK -1015

// -1020 to -1040 defines QEvent types
// All arguments are 0 unless otherwise stated
#define PUKE_WIDGET_EVENT_NONE -1020
// iArg: timerId
#define PUKE_WIDGET_EVENT_TIMER -1021
// All EVENT_MOUSE are:
// cArg: cast to int[25], int[0] = x(), int[1] = y(), int[2] = button()
//       int[3] = state()
#define PUKE_WIDGET_EVENT_MOUSEBUTTONPRESS -1022
#define PUKE_WIDGET_EVENT_MOUSEBUTTONRELEASE -1023
#define PUKE_WIDGET_EVENT_MOUSEDBLCLICK -1024
#define PUKE_WIDGET_EVENT_MOUSEMOVE -1025
// All EVENT_KEY
// cArg: cast to int[25], int[0] = key(), int[1] = ascii(), int[2] = state()
#define PUKE_WIDGET_EVENT_KEYPRESS -1026
#define PUKE_WIDGET_EVENT_KEYRELEASE -1027
// All EVENT_FOCUS
// cArg[0] = gotFocus()
// cArg[1] = lostFocus()
#define PUKE_WIDGET_EVENT_FOCUSIN -1028
#define PUKE_WIDGET_EVENT_FOCUSOUT -1029
#define PUKE_WIDGET_EVENT_ENTER -1030
#define PUKE_WIDGET_EVENT_LEAVE -1031
// Paint event
#define PUKE_WIDGET_EVENT_PAINT -1032
// cArg: int[0] = pos()->x() int[1] = pos->y()
//       int[2] = oldPos->x() int[3] = oldPos->y()
#define PUKE_WIDGET_EVENT_MOVE -1033
// cArg: int[0] = size()->width() int[1] = size()->height()
//       int[2] = oldSize()->width() int[3] = oldSize()->height()
#define PUKE_WIDGET_EVENT_RESIZE -1034
#define PUKE_WIDGET_EVENT_CREATE -1035 
#define PUKE_WIDGET_EVENT_DESTROY -1036
#define PUKE_WIDGET_EVENT_SHOW -1037   
#define PUKE_WIDGET_EVENT_HIDE -1038   
#define PUKE_WIDGET_EVENT_CLOSE -1039  

// Desc: move widget
// iWinId: widget id
// iArg: new location, lower short is x, upper short is y
// cArg: not define
#define PUKE_WIDGET_MOVE 1050

// Desc: move widget
// iWinId: widget id
// iArg: new location, lower short is x, upper short is y
// cArg: not define
#define PUKE_WIDGET_MOVE_ACK -1050

// Desc: open and load library file
// iWinid: not defined
// iArg: not defined
// cArg: file name
#define PUKE_WIDGET_LOAD 1055

// Desc: ack the open library file
// iWinid: not defined
// iArg: widget number
// cArg: not defined
#define PUKE_WIDGET_LOAD_ACK -1055

// 1100 defines QFrame
// Desc: set Frame style
// iWinId: widget to chanse
// iArg: frame style to set.
// cArg: no define
#define PUKE_QFRAME_SET_FRAME 1100

// Desc: get/ack Frame style
// iWinId: widget to changed
// iArg: frame style.
// cArg: no define
#define PUKE_QFRAME_SET_FRAME_ACK -1100

// Desc: set Frame line width
// iWinId: widget to chanse
// iArg: newline width.
// cArg: no define
#define PUKE_QFRAME_SET_LINEWIDTH 1105

// Desc: get/ack Frame line width
// iWinId: widget to changed
// iArg: line width.
// cArg: no define
#define PUKE_QFRAME_SET_LINEWIDTH_ACK -1105

// 
// Base commands are done, next describes Widget's
//

// WIDGET defines a base QWidget class
#define PWIDGET_WIDGET 2

// FRAME defines a base class
#define PWIDGET_FRAME 3



// Group layout commands exist between 10000 and 11000

// Desc: create new box layout
// iWinId: PWidget parent
// iArg: 2 shorts, short[0] direction, short[1] border
// cArg: random character string
#define PUKE_LAYOUT_NEW 11000
// Desc: ack for box layout
// iWinId: Layout ID.
// iArg: not define
// cArg: same random m character string as PUKE_LAYOUT_NEW
#define PUKE_LAYOUT_NEW_ACK -11000

#define PUKE_LAYOUT_ADDLAYOUT 11005
#define PUKE_LAYOUT_ADDLAYOUT_ACK -11005

// Desc: add widget into layout manager
// iWinId: Layout Manager to add widget too
// iArg: Widget Id to be added
// cArg: 2 characters, char[0] strech, char[1] alignement
#define PUKE_LAYOUT_ADDWIDGET 11010
// Desc: ack for add widget
// iWinId: Layout manager 
// iArg: not defined
// cArg: not define
#define PUKE_LAYOUT_ADDWIDGET_ACK -11010


