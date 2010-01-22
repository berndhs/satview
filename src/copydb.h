#ifndef COPYDB_H
#define COPYDB_H

//
//  Copyright (C) 2010 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

#include "copydb-config.h"
#include "dbconnect.h"
#include "ui_copydb.h"
#include <QObject>
#include <QDialog>
#include <QApplication>
#include <QTextDocument>
#include <QTextStream>
#include <stdio.h>

namespace satview {

class CopyEngine : public QObject , public Ui_CopyReport {

Q_OBJECT

public:

  
  CopyEngine (QApplication * pA, const bool dis = true);
  ~CopyEngine ();
  
  
  enum DBType { T_none, T_mysql, T_sqlite, T_web };
  
  void SetApp (QApplication * pA);
  
  void SetSource (const QString server, 
                  const QString path,
                  const DBType  dbtype,
                  const QString db,
                  const QString user,
                  const QString pass);  
                  
  void SetDest   (const QString server, 
                  const QString path,
                  const DBType  dbtype,
                  const QString db,
                  const QString user,
                  const QString pass);
                  
  void StartEngine (const QString picname,
            const unsigned long int too_old,
            const unsigned long int too_recent);
            
  void SetDisplay (bool dis) { dodisplay = dis; }
            
public slots:

  void StartCopy ();
  void ReceiveData (char * data, quint64 len);
  void Finish ();
  void Quit ();
  
signals:

  void HaveData (char * , quint64);
            
private:


  void StartNextCycle ();
  void Init ();
  void Status (const QString stat, const bool fini = false);
  
  void Report (const IndexRecord &rec, bool ok);
                  
  void SetConn   (DBConnection & con,
                  const QString server, 
                  const QString path,
                  const DBType  dbtype,
                  const QString db,
                  const QString user,
                  const QString pass);
                  

  QString Kind (const DBType t);

  QApplication *pApp;
  
  DBConnection DBin;
  DBConnection DBout;
  
  IndexRecord  currentRec;
  char        dataBuf[8*1024*1024];
  string      imageData;
  quint64     dataLen;
  
  int         numcopied;
  int         writefailed;
  QTextDocument   *progressMsg;
  QTextStream     *progressStream;
  
  QDialog         *gui;
  
  bool   dodisplay;
  bool   stopped;
  
  
};

} // namespace

#endif

