#ifndef DBCONNECTION_H
#define DBCONNECTION_H

//
//  Copyright (C) 2009 - Bernd H Stramm
//
#include "satview-config.h"

#include <string>
#if SATVIEW_USE_MYSQL
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#endif

#if SATVIEW_USE_GNUSOCK
#include <cc++/socket.h>
#include <cc++/address.h>
#endif

#if SATVIEW_USE_WINSOCK
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#endif

#include "berndsutil.h"


namespace satview {

  using namespace std;

class IndexRecord {

 public:
  unsigned long int    ident;
  string               picname;
  string               remark;
  string               storetime;

  string String() {
    return string (berndsutil::toString(ident)
                   + " " 
                   + picname + " " + remark + " " + storetime);
  }

};

/** @brief DBConnection provides an abstration of the database containing
 * the images. It handles communication with the database implementation.
 * Currently, the alternatives are a MySql database using Connector/C++,
 * and a Web database using the SATSERV protocol. The Web interface uses
 * GNU common sockets or WinSock2.
 */

class DBConnection {
 public:
  DBConnection();
  ~DBConnection();



  enum Method {
    Con_None,
    Con_WebSock,
    Con_MySqlCPP,
    Con_Bad
  };

  void SetMethod (Method m);
  Method GetMethod() { return mMeth; }

  void SetServer (string server);

  bool ConnectDB (string server, string db, string user, string pass);
  void Disconnect ();

  bool    LoadIndex (string pic);              // ReadIndexRec wont work before this
  bool    ReadIndexRec  (IndexRecord &r);  // read the index entries
  size_t  ReadImageData (IndexRecord &r, string & data);    // fill in image data
  bool  InsertRec (const IndexRecord &r, const string &data);
  bool  InsertRec (const IndexRecord &r, const char* data, const size_t len);

 private:

  Method   mMeth;

  string     mServer;
  string     mUser;
  string     mPass;
  string     mDBname;
  string     mPicname;

  bool ConnectDB_MYSQL (string server, string db, string user, string pass);
  bool Start_MYSQL_Index ();
  bool Start_Web_Index();

  bool Attempt_Web_Connect();
  void Web_Close();

  bool ReadIndexRec_Web (IndexRecord &r);
  bool ReadIndexRec_MYSQL (IndexRecord &r);
  size_t  ReadImageData_Web (IndexRecord &r, string & data);
  size_t  ReadImageData_MYSQL (IndexRecord &r, string & data);

  bool  InsertRec_MYSQL (const IndexRecord &r,
                         const string      &data);
 
  void  hex_to_chars (string & result, const string &hex);
  void  hex_to_chars (string & result, const char * buf, int numchars);
  void  chars_to_hex (string & result, const string &chars);

#if SATVIEW_USE_MYSQL
  sql::Connection* pDBCon;
  sql::Driver    * pDBDriver;
  sql::ResultSet * pIndexRes;
#endif

  istringstream * mWebResult;
  int    mWebIndex;
  bool   mHaveWebData;
  int    mWebBytes;

#if SATVIEW_USE_GNUSOCK
  ost::SimpleTCPStream * pWebStream;
#endif

#if SATVIEW_USE_WINSOCK

  WSADATA          mWsaData;
  struct addrinfo* mWinAddr;
  string           mWebPort;
  bool             mWSAisup;
  SOCKET           mWinSock;

#endif
  
  static const int mWebBufMax = 4*1024*1024;

  char mWebBuf[mWebBufMax];


};

}


#endif
