#ifndef _REQUESTER_H_
#define _REQUESTER_H_

class Requester {

public:
  Requester(int);
  ~Requester();

  int  openModem(const char *dev);
  int  openLockfile(const char *dev, int flags = 0);
  bool removeLockfile();
  int  openResolv(int flags);
  int  openSysLog();
  bool setPAPSecret(const char *name, const char *password);
  bool setCHAPSecret(const char *name, const char *password);
  bool removeSecret(int authMethode);
  bool stop();

public:
  static Requester *rq;

private:
  bool sendRequest(struct RequestHeader *request, int len);
  bool recvResponse();
  int  recvFD(char *filename, int size);
  int  indexDevice(const char *dev);
  int  socket;
  char buffer[100];
  bool expect_alarm;
};

void recv_timeout(int);

#endif
