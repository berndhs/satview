#include "dbconnect.h"
#include "berndsutil.h"

#include <string.h>
#include <stdio.h>
#if SATVIEW_USE_MYSQL
#include <cppconn/prepared_statement.h>
#endif
#if SATVIEW_USE_QNET
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#endif
#include "satpicbuf.h"
#include "satpiclist.h"
#include <stdlib.h>

#include <limits>

#include "satview-debug.h"
#include "satview-defaults.h"

#include <QDebug>

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

namespace satview {


  DBConnection::DBConnection ()
    :mMeth(Con_None),
     mWebIndex(0),
     mHaveWebData(false),
     mWebBytes(0)
 
  {
  
    mPathOnServer = string(SATVIEW_DEFAULT_PATH);
#if SATVIEW_USE_QNET
    mWaitForIndex = false;
    mWaitForImage = false;
    mQMgr = new QNetworkAccessManager (this);

#endif
#if SATVIEW_USE_MYSQL
    pDBCon = 0;
    pDBDriver = 0;
    pIndexRes = 0;
#endif
#if SATVIEW_USE_GNUSOCK
    pWebStream = 0;
#endif
#if SATVIEW_USE_WINSOCK
	mWSAisup = false;
	mWebPort = "80";
	mWinAddr = 0;
	mWinSock = INVALID_SOCKET;
#endif
#if SATVIEW_USE_QSQL
   mQDB = QSqlDatabase::addDatabase("QMYSQL","satview_QDB");
   mQConnection = mQDB.connectionNames()[0];
   
   if (mQDB.isOpen()) {
     mIndexQuery = QSqlQuery(mQDB);
   }

#endif
 	mWebResult = 0;
  }

  DBConnection::~DBConnection ()
  {
    /** @todo clean up sockets or DB connections
     */
#if SATVIEW_USE_QSQL
    if (mIndexQuery.isActive()) {
      mIndexQuery.clear();
    }
    mQDB.close();
#endif
  }

  void
  DBConnection::Disconnect()
  {
    /** @todo do something, in case they want to start on a 
     * different server
     */
#if SATVIEW_USE_MYSQL
    if (pIndexRes) {
      delete pIndexRes;
      pIndexRes = 0;
    }
#endif
#if SATVIEW_USE_QSQL
    if (mIndexQuery.isActive()) {
      mIndexQuery.clear();
    }
    if (mQDB.isOpen()) {
      mQDB.close();
    }

#endif
    if (mWebResult) {
      delete mWebResult;
      mWebResult = 0;
    }
  }

  void 
  DBConnection::SetMethod (Method m)
  {
    Disconnect();
#if SATVIEW_USE_MYSQL
    if (pIndexRes) {
      delete pIndexRes;
      pIndexRes = 0;
    }
#endif
#if SATVIEW_USE_QSQL
    if (mIndexQuery.isActive()) {
      mIndexQuery.clear();
    }
#endif
    if (mWebResult) {
      delete mWebResult;
      mWebResult = 0;
    }
    mWebIndex = 0;
    mMeth = m;
  }

  bool
  DBConnection::ConnectDB (string server, string db, string user, string pass)
  {
    mServer = server;
    mUser = user;
    mPass = pass;
    mDBname = db;
    if (mMeth == Con_MySql) {
      return ConnectDB_MYSQL(server,db,user,pass);
    }
    if (mMeth == Con_Web) {
      // connect socket now, or when loading index?
      // it could time out if we do it here
#if SATVIEW_USE_QSQL
      if (!mQDB.isOpen()) {
         mQDB = QSqlDatabase::addDatabase("QMYSQL");
         mQConnection = mQDB.connectionNames()[0];
      }
      if (mIndexQuery.isActive()) {
        mIndexQuery.clear();      
      }
#endif
      return true; 
    }
    return false;
  }

  bool
  DBConnection::ConnectDB_MYSQL (string server, string db, 
                                 string user, string pass)
  {
#if SATVIEW_USE_MYSQL
    if (!pDBDriver) {
      pDBDriver = get_driver_instance(); 
    }
    try {
       pDBCon = pDBDriver->connect(server, user, pass);
    } catch (std::exception &e) {
      std::cout << e.what() << __LINE__ << std::endl;
       std::cout << " no DB driver, can't get images " << std::endl;
	pDBCon = 0;
    }  
    if (pDBCon) {
      pDBCon->setSchema(db);
      return true;
    } 
#endif
#if SATVIEW_USE_QSQL
     mQDB.setDatabaseName(db.c_str());
     mQDB.setHostName(server.c_str());
     mQDB.setUserName(user.c_str());
     mQDB.setPassword(pass.c_str());
     bool ok = mQDB.open();
     return ok;
#endif
    // if we get here, something failed, perhaps all SQL flags zero
    mServer="";
    mUser="";
    mPass="";
    mDBname="";
    return false;
  }


  bool
  DBConnection::LoadIndex (const string pic,
                           const bool              everything,
                           const unsigned long int minIdent,
                           const unsigned long int maxIdent) 
  {
    unsigned long int useMinIdent(minIdent);
    unsigned long int useMaxIdent(maxIdent);
    mPicname = pic;
    if (everything) {
       useMinIdent = 0;
       useMaxIdent = std::numeric_limits<unsigned long int>::max();
    }
    switch (mMeth) {
    case Con_MySql: 
      return Start_MYSQL_Index(useMinIdent, useMaxIdent);
    case Con_Web:
      return Start_Web_Index(useMinIdent, useMaxIdent);
    default:
      break;
    }
    return false;
  }


  bool
  DBConnection::Start_MYSQL_Index(unsigned long int min, unsigned long int max)
  {
#if SATVIEW_USE_MYSQL
    try {
      string q_str = 
	string ("SELECT ident, storetime, remark, picname FROM `satpics` WHERE `picname`=")
        + string ("'") + mPicname + string ("'");
      q_str.append (string(" AND `ident` >= ") + berndsutil::toString(min));
      q_str.append (string(" AND `ident <= ") + berndsutil::toString(max));
      if (!pDBCon) {
        return false;
      }
      sql::Statement * pStmt = pDBCon->createStatement();
      pIndexRes = pStmt->executeQuery (q_str);
    } catch (sql::SQLException& e) {
      std::cout << e.what() << __LINE__ << std::endl;
      pIndexRes = 0;
      return false;
    }
    return true;
#endif
#if SATVIEW_USE_QSQL
    if (mIndexQuery.isActive()) {
      mIndexQuery.clear();
    }
    QString queryString
        ("SELECT ident, storetime, remark, picname FROM `satpics` WHERE `picname` ='");
    queryString.append(mPicname.c_str());
    queryString.append("'");
    queryString.append (" AND `ident` >= ");
    QString minmax;
    minmax.setNum(min);
    queryString.append (minmax);
    queryString.append (" AND `ident` <= ");
    minmax.setNum(max);
    queryString.append (minmax);
    QSqlQuery q(mQDB);
    q.setForwardOnly(true);
    q.prepare(queryString);
    bool ok = q.exec();
    
    mIndexQuery = q;
    return ok;
#endif
    return false;
  }

  bool
  DBConnection::Attempt_Web_Connect ()
  {
#if SATVIEW_USE_QNET
    return true;   // and pray that it works later
#endif
#if SATVIEW_USE_GNUSOCK
     try {

       ost::IPV4Host  myhost (mServer.c_str());
       int the_port = 80;

       pWebStream = new ost::SimpleTCPStream (myhost,the_port);
       return pWebStream != 0; 
     } catch (ost::Socket* psock) {
       std::cout << " weirdo socket layer throws pointer " << psock << endl;
       if (psock) {
	 cout << " error number " << psock->getErrorNumber() << endl;
         cout << " error msg " << psock->getErrorString() << endl;
	 cout << "or maybe system error: " << endl;
	 cout << psock->getSystemError() << endl;
       }
     } catch (std::exception &se) {
       cout << se.what () << endl;
       return false;
     }
#endif
#if SATVIEW_USE_WINSOCK
	 if (!mWSAisup) {
	   int wsaErr = WSAStartup (MAKEWORD(2,2), &mWsaData);
	   if (wsaErr == 0) {
	     mWSAisup = true;
	   }
	 }
	 if (!mWSAisup) {
	   return false;
	 }
	 struct addrinfo * result = 0;
	 struct addrinfo   hints;
	 memset (&hints,0,sizeof(hints));
	 hints.ai_family = AF_INET;
	 hints.ai_socktype = SOCK_STREAM;
	 hints.ai_protocol = IPPROTO_TCP;
	 int stat = getaddrinfo (mServer.c_str(), mWebPort.c_str(), &hints, &result);
	 if (stat != 0) {
		 cout << "can't get address " << WSAGetLastError() << endl;
	   WSACleanup();
	   mWSAisup = false;
	   return false;
	 }
	 mWinSock = socket (result->ai_family, result->ai_socktype, result->ai_protocol);
	 if (mWinSock == INVALID_SOCKET) {
	   cout << " error at Win socket: " << WSAGetLastError() << endl;
       freeaddrinfo (result);
	   WSACleanup();
	   mWSAisup = false;
	   return false;
	 }
	 stat = connect (mWinSock, result->ai_addr, static_cast<int>(result->ai_addrlen));
	 if (stat == SOCKET_ERROR) {
	   closesocket (mWinSock);
	   mWinSock = INVALID_SOCKET;
	   WSACleanup();
	   mWSAisup = false;
	   return false;
	   // we could try the next address
	 }
     // if we get here, mWinSock has a valid socket address, and the socket is live
	 return true;
#endif
     return false;
  }

  void
  DBConnection::Web_Close ()
  {
#if SATVIEW_USE_GNUSOCK
    if (pWebStream) {
      delete pWebStream;
      pWebStream = 0;
    }
#endif
  }

  bool
  DBConnection::Start_Web_Index(unsigned long int min, unsigned long int max)
  {
    mHaveWebData = false;
    if (!Attempt_Web_Connect()) {
      return false;
    }
#if SATVIEW_USE_QNET
    if (Waiting()) {
      return false;
    }
    QUrl url;
    url.setScheme("http");
    url.setHost(mServer.c_str());
    url.setPath(mPathOnServer.c_str());
    url.setEncodedQuery("fn=index");     // setEncodedQuery adds the '?'
    QString minmax;
    minmax.setNum(min);
    url.addQueryItem("min",minmax);
    minmax.setNum(max);
    url.addQueryItem("max",minmax);
    QNetworkRequest req;
    req.setRawHeader("User-Agent","Maxwell Smart");
    req.setUrl(url);
    if (mQMgr) {
      if (mWaitForIndex || mWaitForImage) {  // one at a time !
        return false;
      }
      mExpectIndexReply = mQMgr->get(req);
      mWaitForIndex = true;
      connect (mExpectIndexReply, SIGNAL(finished()),
               this, SLOT(GetIndexReply()));
      return true;
    } 
    return false;    
#endif
    string client_msg;
    string proto (" HTTP/1.0 ");
    client_msg = mPathOnServer + "?fn=index" 
       + string("&min=") + berndsutil::toString(min)
       + string("&max=") + berndsutil::toString(max)
       + proto 
       + string("\nUser-Agent: Maxwell Smart \n") 
       + string ("Accept: */*\n")
       + string("Host: ") + mServer + "\n"
       + string("Connection: Keep-Alive\n")
			 +  string("\n\n\n");
    int numbytes = 0;
#if SATVIEW_USE_GNUSOCK
    numbytes = pWebStream->write (client_msg.c_str(), client_msg.length());
#endif
#if SATVIEW_USE_WINSOCK
	int win_send_stat = send (mWinSock, client_msg.c_str(), client_msg.length(), 0);
	if (win_send_stat == SOCKET_ERROR) {
	   WSACleanup();
	   mWSAisup = false;
	   return false;
	}
	numbytes = client_msg.length();
#endif
    if (numbytes > 0) {
       memset (mWebBuf, 0, mWebBufMax);
	   numbytes = 0;
#if SATVIEW_USE_GNUSOCK
       numbytes = pWebStream->read (mWebBuf, mWebBufMax);
#endif
#if SATVIEW_USE_WINSOCK
       int chunk_read = 0;
	   int chunk_start = 0;
	   int buf_remain = mWebBufMax;
	   do {
		  chunk_read = recv (mWinSock, mWebBuf+chunk_start, buf_remain, 0);
		  chunk_start += chunk_read;
		  buf_remain -= chunk_read;
		  numbytes += chunk_read;
	   } while (chunk_read > 0 && buf_remain > 0);
#endif
       if (numbytes > 0) {
      	 mWebBytes = numbytes;
         mWebIndex = 0;
         mHaveWebData = true;
      	 memset (mWebBuf+numbytes,0,mWebBufMax - numbytes);
         if (mWebResult) {
      	   delete mWebResult;
         }
         mWebResult = new istringstream(string(mWebBuf)); // copied TWICE, yuck
         /** to have the istringstream ready for reading index entries,
	  * eat up everything until we have seen SATVIEW-INDEX
	  */
         string word;
      	 while (!mWebResult->eof()) {
           (*mWebResult) >> word;
           if (word == "SATVIEW-INDEX") {
      	     break;
      	   }
      	 }
       }
    }
    Web_Close();
    return mHaveWebData;
  }
    

  bool
  DBConnection::ReadIndexRec (IndexRecord &r)
  {
    switch (mMeth) {
    case Con_Web:
      return ReadIndexRec_Web(r);
    case Con_MySql:
      return ReadIndexRec_MYSQL(r);
    default:
      break;
    }
    return false;
  }

  size_t
  DBConnection::ReadImageData (IndexRecord &r, string & data)
  {
    switch (mMeth) {
    case Con_Web:
      return ReadImageData_Web(r,data);
    case Con_MySql:
      return ReadImageData_MYSQL(r, data);
    default:
      break;
    }
    return 0;
  }

  bool
  DBConnection::ReadIndexRec_MYSQL (IndexRecord &r)
  {
#if SATVIEW_USE_MYSQL
    if (!pIndexRes) {
      return false;
    }
    if (pIndexRes->next()) {
      r.storetime = pIndexRes->getString("storetime");
      r.ident = pIndexRes->getInt("ident");
      r.remark = pIndexRes->getString("remark");
      r.picname = pIndexRes->getString("picname");
      return true;
    }
#endif
#if SATVIEW_USE_QSQL
      bool ok = mIndexQuery.next();
      if (!ok) {
         return false;
      }
      QSqlRecord rec = mIndexQuery.record();
      QVariant v;
      v = mIndexQuery.value(rec.indexOf("storetime"));
      r.storetime = string(v.toByteArray().data());
      v = mIndexQuery.value(rec.indexOf("ident"));
      r.ident = v.toLongLong();
      v = mIndexQuery.value(rec.indexOf("remark"));
      r.remark = string(v.toByteArray().data());
      v = mIndexQuery.value(rec.indexOf("picname"));
      r.picname = string(v.toByteArray().data());
      return true;
#endif
    return false;
  }

  size_t
  DBConnection::ReadImageData_MYSQL (IndexRecord &r, string & simage)
  {
#if SATVIEW_USE_MYSQL
    if (!pDBCon) {
      return 0;
    }
    try {
      string q_str = "SELECT ident, image FROM `satpics` WHERE ident="
	+ berndsutil::toString(r.ident)
        + " AND picname ='"
        + mPicname
        + "' ORDER BY ident DESC";

      sql::Statement * pStmt = pDBCon->createStatement();
      sql::ResultSet * pRes = pStmt->executeQuery (q_str);    
      int image_index = pRes->findColumn("image");
     
      if (pRes) {
        pRes->next();
        simage = pRes->getString(image_index);
	if (pStmt) {
          delete pStmt;
	}
	if (pRes) {
	  delete pRes;
	}
        return simage.length();
      }
     
    } catch (sql::SQLException &e) {
      std::cout << e.what() << __LINE__ <<  std::endl;
      simage = "";    
    }
#endif
#if SATVIEW_USE_QSQL
   QSqlQuery ImgQuery(mQDB);
   QString q_str ("SELECT ident, image FROM `satpics` WHERE ident=");
   QString num;
   num.setNum(r.ident);
   q_str.append(num);
   q_str.append (" AND picname ='");
   q_str.append (mPicname.c_str() );
   q_str.append ("' ORDER BY ident DESC");
   bool ok = ImgQuery.exec(q_str);
   if (!ok) {
     return 0;
   }
   ImgQuery.next();
   QVariant v;
   v = ImgQuery.value(1);
   simage.clear();
   int len = v.toByteArray().size();
   simage = string(v.toByteArray().data(), len);
   return len;
#endif
    return 0;
  }  
  

  void
  DBConnection::chars_to_hex (string &result, const string &chars)
  {
    result.clear();
  
    unsigned int len = chars.length();
    unsigned int byte, i;
    char hexnum[4];
    memset (hexnum, 0, 4);
    for (i=0;i<len; i++) {
      byte = chars[i];
#ifdef _MSC_VER
      sprintf_s(hexnum,4,"%02x", byte);
#else
      sprintf (hexnum,"%02x", byte);
#endif
      result.append(hexnum, 2);
    }
  }

  void
  DBConnection::hex_to_chars (string &result, const string &hex)
  {
    unsigned char byte;
    int numchars = hex.length() ;
    char *hexbuf= new char[numchars+4];
    char *hexnum;
    memcpy (hexbuf,hex.c_str(),numchars);
    unsigned char nibble, nibble_val;
    result.clear();
    int b = 0;
    for (int i=0; i<numchars; i+=2) {
      hexnum = hexbuf + i;
      byte = 0;
      for (int j=0;j<2;j++) {
      	nibble = hexnum[j];
        if (nibble >= '0' && nibble <= '9') {
      	  nibble_val = nibble - '0';
        } else if (nibble >= 'A' && nibble <= 'F') {
          nibble_val = 10 + nibble - 'A';
        } else if (nibble >= 'a' && nibble <= 'f') {
          nibble_val = 10 + nibble - 'a';
        } else {
          nibble_val = 0;
        }

        byte = (byte << 4) | (nibble_val & 0x0f);     
      }
      hexbuf[b] = byte;
      b++;
    }
    result.append(hexbuf,b);
    delete[] hexbuf;
  }

  void
  DBConnection::hex_to_chars (string &result, 
                              const char * inbuf, 
                              int numchars)
  {
    unsigned char byte;
    char hexnum[4];
    memset (hexnum, 0, 4);
    unsigned char nibble, nibble_val;
    result.clear();
    for (int i=0; i<numchars; i+=2) {
      hexnum[0] = inbuf[i];
      hexnum[1] = inbuf[i+1];
      byte = 0;
      for (int j=0;j<2;j++) {
      	nibble = hexnum[j];
        if (nibble >= '0' && nibble <= '9') {
      	  nibble_val = nibble - '0';
        } else if (nibble >= 'A' && nibble <= 'F') {
          nibble_val = 10 + nibble - 'A';
        } else if (nibble >= 'a' && nibble <= 'f') {
          nibble_val = 10 + nibble - 'a';
        } else {
          nibble_val = 0;
        }

        byte = (byte << 4) | (nibble_val & 0x0f);     
      }
      result.append(1,byte);
    }

  }

  void
  DBConnection::hex_to_chars (char * result, 
                              const char * inbuf, 
                              int numchars)
  {
    unsigned char byte;
    char hexnum[4];
    memset (hexnum, 0, 4);
    unsigned char nibble, nibble_val;
    int b(0);
    for (int i=0; i<numchars; i+=2) {
      hexnum[0] = inbuf[i];
      hexnum[1] = inbuf[i+1];
      byte = 0;
      for (int j=0;j<2;j++) {
	      nibble = hexnum[j];
        if (nibble >= '0' && nibble <= '9') {
	        nibble_val = nibble - '0';
        } else if (nibble >= 'A' && nibble <= 'F') {
          nibble_val = 10 + nibble - 'A';
        } else if (nibble >= 'a' && nibble <= 'f') {
          nibble_val = 10 + nibble - 'a';
        } else {
          nibble_val = 0;
        }

        byte = (byte << 4) | (nibble_val & 0x0f);     
      }
      result[b] = byte;
      b++;
    }

  }

  bool
  DBConnection::ReadIndexRec_Web (IndexRecord &r)
  { // should be in the mWebBuf somewhere, starting at mWebIndex
#if SATVIEW_USE_QNET
    if (mWaitForIndex) {
      return false;
    }
#endif
    string keyword;
    (*mWebResult) >> keyword;
    if (keyword != "record") {
      return false;
    }
    (*mWebResult) >> keyword; // record number, who cares
    string dash;
    string val_in_hex;
    string val_chars;
    int i;
    for (i=0;i<4;i++) {
      (*mWebResult) >> keyword;
      (*mWebResult) >> dash;
      (*mWebResult) >> val_in_hex;
      hex_to_chars(val_chars,val_in_hex);
      if (keyword == "ident") {
	r.ident = berndsutil::fromString<unsigned long int>(val_chars);
      } else if (keyword == "picname") {
        r.picname = val_chars;
      } else if (keyword == "remark") {
        r.remark = val_chars;
      } else if (keyword == "storetime") {
        r.storetime = val_chars;
      }
    }  
    return true; // until we write it
  }

  size_t
  DBConnection::ReadImageData_Web (IndexRecord &r, string & simage)
  {
    string key1, key2;
    string raw_key1 = berndsutil::toString(r.ident);
    string raw_key2 = r.picname;
    chars_to_hex (key1, raw_key1);
    chars_to_hex (key2, raw_key2);

#if SATVIEW_USE_QNET
    if (Waiting()) {
      return 0;
    }
    string longUrl = "http://" + mServer 
                     + mPathOnServer + "?fn=item&k1="
                     + key1
                     + "&k2="
                     + key2;
    QNetworkRequest req;
    req.setUrl(QUrl(longUrl.c_str()));
    req.setRawHeader("User-Agent","Maxwell Smart");
    if (mQMgr) {
      if (mWaitForImage) {
        return false;
      }
      mExpectImgReply = mQMgr->get(req);
      mWaitForImage = true;
      connect (mExpectImgReply, SIGNAL(finished()),
               this, SLOT(GetImageReply()));
      connect (&mGetTimeout, SIGNAL(timeout()),this,SLOT(NoImageReply()));
      mGetTimeout.setSingleShot(true);
      mGetTimeout.start(60000);
      return 0;
    }
    return 0;
#endif
    string client_msg;
    string proto (" HTTP/1.0 ");
    client_msg = "GET " + mPathOnServer + "?fn=item"
      + string ("&k1=") + key1
      + string ("&k2=") + key2
      + string (" ")
       + proto 
       + string("\nUser-Agent: Maxwell Smart \n") 
       + string ("Accept: */*\n")
       + string("Host: ") + mServer + "\n"
       + string("Connection: Keep-Alive\n")
			 +  string("\n\n\n");
    if (!Attempt_Web_Connect()) {
      cout << "Can't connect to Internet" << endl;
      return false;
    }
    int numbytes = 0;
#if SATVIEW_USE_GNUSOCK
    numbytes = pWebStream->write (client_msg.c_str(), client_msg.length());
#endif
#if SATVIEW_USE_WINSOCK
	int win_send_stat = send (mWinSock, client_msg.c_str(), client_msg.length(), 0);
	if (win_send_stat == SOCKET_ERROR) {
	   WSACleanup();
	   mWSAisup = false;
	   return false;
	}
	numbytes = client_msg.length();
#endif
    if (numbytes > 0) {
      
      memset (mImgWebBuf, 0, mWebBufMax);
	  numbytes = 0;
#if SATVIEW_USE_GNUSOCK
      numbytes = pWebStream->read (mImgWebBuf, mWebBufMax);
#endif
#if SATVIEW_USE_WINSOCK
      int chunk_read = 0;
	  int chunk_start = 0;
	  int buf_remain = mWebBufMax;
	  do {
	    chunk_read = recv (mWinSock, mImgWebBuf+chunk_start, buf_remain, 0);
	    chunk_start += chunk_read;
	    buf_remain -= chunk_read;
	    numbytes += chunk_read;
	  } while (chunk_read > 0 && buf_remain > 0);
#endif
       if (numbytes > 0) {
         // eat the header
         string wb(mImgWebBuf);
         istringstream * istr = new istringstream(wb); // copied TWICE, yuck
         /** to have the istringstream ready for reading index entries,
	  * eat up everything until we have seen SATVIEW-INDEX
	  */
         string word;
	 while (!istr->eof()) {
           (*istr) >> word;
           if (word == "SATVIEW-ITEM") {
	     break;
	   }
	 }
         (*istr) >> word;
         int len;
         char x;
         if (word == "LEN") {
           (*istr) >> word;
           len = berndsutil::fromString<int>(word);     
           // eat characters until we saw an 'x'
           x = 'a';
           do {
             istr->get(x);
           } while (x != 'x');

	  /** len is the image length in bytes, have 2 hex digits per byte */
	  hex_to_chars(simage, mImgWebBuf+istr->tellg(), len*2); 
          delete istr;
         }
       }
       Web_Close();
      return simage.length(); // until we write it
    }
    Web_Close();
    return 0;
  }

  bool
  DBConnection::InsertRec (const IndexRecord &r, const string & data)
  {
    switch (mMeth) {
    case Con_Web:
      return false;   // can't do it
    case Con_MySql:
      return InsertRec_MYSQL(r,data);
    default:
      break;
    }
    return false;
  }

  bool
  DBConnection::InsertRec (const IndexRecord &r, 
                        const char* data)
  {
    switch (mMeth) {
    case Con_Web:
      return false;   // can't do it
    case Con_MySql:
      return InsertRec_MYSQL(r, string(data));
    default:
      break;
    }
    return false;
  }

  bool
  DBConnection::InsertRec_MYSQL (const IndexRecord &r,
                                 const string      &data)
  {
#if SATVIEW_USE_MYSQL
    bool ok(false);
    if (!pDBCon) {
      return 0;
    }
    sql::Statement *pStmt(0);
    try {
      string quote ("'");
      string q_str = string
       
	("INSERT INTO `satpics` ( ident, picname, storetime, remark, image ) VALUES (?,?,?,?,?) ");

      istringstream tmp_blob(data);
      pStmt = pDBCon->createStatement();
      pStmt->execute (string("USE " + mDBname));
      sql::PreparedStatement *pPrep;
      pPrep = pDBCon->prepareStatement(q_str);
      pPrep->setUInt(1,r.ident);
      pPrep->setString(2,r.picname);
      pPrep->setNull(3,0);
      pPrep->setString(4,r.remark);
      pPrep->setBlob(5, &tmp_blob);
      pPrep->execute();
      ok = true;  // well, didn't crash
    } catch (sql::SQLException &e) {
      cout << " MySql insert failed " << e.what() << endl;
      ok = false;
    } catch (std::exception &e) {
      cout << " exception caught " << e.what() << endl;
      ok = false;
    } catch (...) {
      cout << " unknown exception for " << r.ident << endl;
      ok = false;
    }
    
    if (pStmt) {
      delete pStmt;
    }
    return ok;
#endif
#if SATVIEW_USE_QSQL
    QSqlQuery InsertQuery(mQDB);
    QString q_str
  	("INSERT INTO `satpics` ( ident, picname, storetime, remark, image ) VALUES (?,?,?,?,?) ");
	  InsertQuery.prepare(q_str);
    QVariant v0, v1, v2, v3, v4;
    v0.setValue(r.ident);
    InsertQuery.bindValue (0,v0);
    v1.setValue(QString(r.picname.c_str()));
    InsertQuery.bindValue (1,v1);
    v2.setValue (QString(r.storetime.c_str()));
    InsertQuery.bindValue (2,v2);
    v3.setValue (QString(r.remark.c_str()));
    InsertQuery.bindValue (3,v3);
    quint64 len = data.length();
    QByteArray bytes (data.c_str(), len);
    v4.setValue (bytes);
    InsertQuery.bindValue(4,v4);
    bool ok = InsertQuery.exec();
    return ok;
   
#endif
    return false;    
  }

  bool
  DBConnection::Waiting()
  {
#if SATVIEW_USE_QNET
    bool waiting = mWaitForIndex || mWaitForImage;

    return waiting;
#else
    return false;
#endif
  }

#if SATVIEW_USE_QNET

  void
  DBConnection::GetIndexReply (QNetworkReply *reply)
  {
    if (reply == 0) {
      reply = mExpectIndexReply;
    }
    if (mWaitForIndex && reply) {
      if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        qint64  nbytes = bytes.size();
        nbytes = ((nbytes < mWebBufMax) ? nbytes : mWebBufMax) ;
        memcpy (mWebBuf,bytes.data(),nbytes);
        mWaitForIndex = false;
        mWebBytes = nbytes;
        mWebIndex = 0;         // index into mWebBuf
        mHaveWebData = true;
        if (nbytes < mWebBufMax) {
	        memset (mWebBuf+nbytes,0, mWebBufMax - nbytes);
	      }
        reply->deleteLater();
        mWebResult = new istringstream(string(mWebBuf)); // copied TWICE, yuck
        /** to have the istringstream ready for reading index entries,	  
         * eat up everything until we have seen SATVIEW-INDEX
	  */
        string word;
	      while (!mWebResult->eof()) {
          (*mWebResult) >> word;
          if (word == "SATVIEW-INDEX") {
	          break;
	        }
	      }
	      emit IndexArrival();
      }
    }
  }

  void
  DBConnection::NoImageReply ()
  {
    GetImageReply(0,true);
  }

  void
  DBConnection::GetImageReply (QNetworkReply *reply, bool timedout)
  {
    // two parts: get the data, and parse them
    // Part 1: retrieve data
    
    bool badimage = true;
    if (reply == 0) {
       reply = mExpectImgReply;
       disconnect (&mGetTimeout,SIGNAL(timeout()),this,SLOT(NoImageReply()));
       mGetTimeout.stop();
       disconnect (mExpectImgReply,SIGNAL(finished()),
               this, SLOT(GetImageReply()));
    }
    if (timedout) {
       mWaitForImage = false;
    }
    if (!timedout && mWaitForImage && reply) {
      mWaitForImage = false;
      if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        qint64  nbytes = bytes.size();
        nbytes = ((nbytes < mWebBufMax) ? nbytes : mWebBufMax) ;
        memcpy (mImgWebBuf,bytes.data(),nbytes);
        mWaitForImage = false;
        mWebBytes = nbytes;
        mWebIndex = 0;         // index into mImgWebBuf
        mHaveWebData = true;
        if (nbytes < mWebBufMax) {
      	  memset (mImgWebBuf+nbytes,0, mWebBufMax - nbytes);
      	}
        reply->deleteLater();
        mExpectImgReply = 0;

    // Part 2: deal with the raw data
       if (nbytes > 0) {
         // eat the header
         string wb(mImgWebBuf);
         istringstream * istr = new istringstream(wb); // copied TWICE, yuck
         string word;
      	 while (!istr->eof()) {
           (*istr) >> word;
           if (word == "SATVIEW-ITEM") {
      	     break;
      	   }
      	 }
        (*istr) >> word;
        int len;
        char x;
        if (word == "LEN") {
          (*istr) >> word;
          len = berndsutil::fromString<int>(word);     
           // eat characters until we saw an 'x'
          x = 'a';
          do {
            istr->get(x);
          } while (x != 'x');

	       /** len is the image length in bytes, have 2 hex digits per byte */
  	      char * blobbytes = new char[len+1];
  	      hex_to_chars(blobbytes, mImgWebBuf+istr->tellg(), len*2);
  	      badimage = false;
  	      emit BlobArrival (blobbytes, static_cast<quint64>(len));
          delete istr;
          return;
        }
      }
    }
   }
   if (badimage) {
     char * badbytes = new char[1024];
     memset (badbytes, 0, 1024);
     emit BlobArrival (badbytes, 1024);
   }
}


#endif

} // namespace
