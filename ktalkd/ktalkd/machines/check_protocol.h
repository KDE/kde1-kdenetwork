/* check_protocol.h */

#ifndef CHECK_PROTOCOL_H
#define CHECK_PROTOCOL_H

#include "../includ.h"

enum ProtocolType {noProtocol, talkProtocol, ntalkProtocol};

class CheckProtocol {

public:

  CheckProtocol (struct in_addr _localMachineAddr);   // IP address of local machine
  virtual ~CheckProtocol () {}

  int CheckHost (struct in_addr host, int retry = 3, int timeout = 2);
  int getProtocol () {return intern_protocol;}

  void timeout ();
  void found (ProtocolType protocol);

//  virtual void timerEvent (QTimerEvent *);

private:
  void put_messages ();

  OLD_CTL_MSG oldDefaultMsg;       // otalk control message with defaults
  NEW_CTL_MSG newDefaultMsg;       // ntalk control message with defaults

  struct in_addr localMachineAddr;   // IP address of local machine
  short int talkDaemonPort;          // Port number of talk demon  (517)
  short int ntalkDaemonPort;         // Port number of ntalk demon (518)

  in_addr hostAddr; // host checked
  int old_socket, new_socket;
  sockaddr_in old_addr, new_addr;

  ProtocolType intern_protocol;
  static ProtocolType old_talk_dummy = talkProtocol;
  static ProtocolType new_talk_dummy = ntalkProtocol;

  int retryCounter, retryMaximum, in_use, result_found;
};

#endif
