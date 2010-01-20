#ifndef DBCONNECTION_H
#define DBCONNECTION_H

//
//  Copyright (C) 2009 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

//
#if DO_COPYDB
#include "copydb-config.h"
#else
#include "satview-config.h"
#endif

#include <string>
#if SATVIEW_USE_MYSQL
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#endif

#if SATVIEW_USE_QNET || SATVIEW_USE_QSQL
#include <QObject>
#endif

#if SATVIEW_USE_QNET
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#endif

#if SATVIEW_USE_QSQL
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QVariant>
#include <QStringList>
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

  class ControlPanel;
  class SatPicBuf;
  class SatPicList;

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

class DBConnection 

#if SATVIEW_USE_QNET || SATVIEW_USE_QSQL
  : public QObject
#endif
                    {

#if SATVIEW_USE_QNET || SATVIEW_USE_QSQL
   Q_OBJECT
#endif

 public:
  DBConnection();
  ~DBConnection();

  bool Waiting ();   // some request not finished

  enum Method {
    Con_None,
    Con_Web,
    Con_MySql,
    Con_Bad
  };

  void SetMethod (Method m);
  Method GetMethod() { return mMeth; }

  void SetServer (string server) {mServer = server; }
  
  void SetWebPath (string path) { mPathOnServer = path; }

  bool ConnectDB (string server, string db, string user, string pass);
  void Disconnect ();


  // ReadIndexRec wont work before LoadIndex

  bool    LoadIndex (const string            pic, 
                     const bool              everything = true,
                     const unsigned long int minIdent = 0,
                     const unsigned long int maxIdent = 0);   
                     
  bool    ReadIndexRec  (IndexRecord &r);  // read the index entries
  size_t  ReadImageData (IndexRecord &r, string & data);    // fill in image data
  bool  InsertRec (const IndexRecord &r, const string &data);
  bool  InsertRec (const IndexRecord &r, const char* data);

#if SATVIEW_USE_QSQL
  void  SetDBType (const string type);
#endif
  

#if SATVIEW_USE_QNET

  public slots:

    void GetIndexReply (QNetworkReply *reply = 0);
    void GetImageReply (QNetworkReply *reply = 0, bool timedout = false);
    void NoImageReply ();
    
  signals:
    void IndexArrival ();
    void BlobArrival (char * data, quint64 len);

#endif

 private:

  Method   mMeth;

  string     mServer;
  string     mUser;
  string     mPass;
  string     mDBname;
  string     mPicname;
  
  string     mPathOnServer;

  bool ConnectDB_MYSQL (string server, string db, string user, string pass);
  bool Start_MYSQL_Index (unsigned long int min, unsigned long int max);
  bool Start_Web_Index (unsigned long int min, unsigned long int max);

#if SATVIEW_USE_QNET

  void DeliverBlob (char * data, qint64 len);
#endif
#if SATVIEW_USE_QSQL
  void OpenDB ();
#endif

  bool Attempt_Web_Connect();
  void Web_Close();

  bool    ReadIndexRec_Web (IndexRecord &r);
  bool    ReadIndexRec_MYSQL (IndexRecord &r);
  size_t  ReadImageData_Web (IndexRecord &r, string & data);
  size_t  ReadImageData_MYSQL (IndexRecord &r, string & data);

  bool  InsertRec_MYSQL (const IndexRecord &r,
                         const string      &data);
 
  void  hex_to_chars (string & result, const string &hex);
  void  hex_to_chars (string & result, const char * buf, int numchars);
  void  hex_to_chars (char * result, const char * buf, int numchars);
  void  chars_to_hex (string & result, const string &chars);

#if SATVIEW_USE_MYSQL
  sql::Connection* pDBCon;
  sql::Driver    * pDBDriver;
  sql::ResultSet * pIndexRes;
#endif

#if SATVIEW_USE_QSQL
  QSqlDatabase     mQDB;
  QSqlQuery        mIndexQuery;
  QString          mQConnection;
  QString          mDBType;      /* QMYSQL, QSQLITE,... */
  QString          mConnectName;
#endif

  istringstream * mWebResult;
  int    mWebIndex;
  bool   mHaveWebData;
  int    mWebBytes;

#if SATVIEW_USE_QNET
  QNetworkAccessManager  *mQMgr;
  QNetworkReply          *mExpectIndexReply;
  QNetworkReply          *mExpectImgReply;
  bool                    mWaitForIndex;
  bool                    mWaitForImage;
  
  QTimer                  mGetTimeout;
#endif

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
  
  static const int mWebBufMax = 8*1024*1024;

  char mWebBuf[mWebBufMax];
  char mImgWebBuf[mWebBufMax];


};

}


#endif
