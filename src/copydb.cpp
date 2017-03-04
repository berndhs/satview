#include "copydb.h"
#include <iostream>
#include "delib-debug.h"

//
//  Copyright (C) 2017 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

//

using namespace std;

namespace satview {

CopyEngine::CopyEngine (QApplication * pA, const bool dis)
:pApp (pA),
 progressStream (0),
 dodisplay (dis),
 stopped (false)
{ 
  Init ();
  if (dis) {
    gui->show ();
  } 
}

CopyEngine::~CopyEngine ()
{}

void
CopyEngine::Init ()
{
  if (dodisplay) {
    gui = new QDialog;
    setupUi (gui);;
    progressMsg = new QTextDocument (gui);
    progressMsg->clear();
    messageBox->setDocument (progressMsg);
    connect (doneButton, SIGNAL (clicked()), this, SLOT (Finish()));
  }
  progressStream  = new QTextStream (stdout);
}

void
CopyEngine::SetApp (QApplication * pA)
{
  pApp = pA;
}

QString
CopyEngine::Kind (const DBType t)
{
  QString tp ("none");
  switch (t) {
  case T_mysql:
    tp = "QMYSQL";
    break;
  case T_sqlite:
    tp = "QSQLITE";
    break;
  default:
    break;
  }
  return tp;
}

void
CopyEngine::SetSource ( const QString server, 
                  const QString path,
                  const DBType  dbtype,
                  const QString db,
                  const QString user,
                  const QString pass)
{
  SetConn (DBin, server, path, dbtype, db, user, pass);
  if (dodisplay) {
    QString displayName;
    if (dbtype == T_sqlite) {
      displayName = path + "/" + db;
    } else {
      displayName = server; 
    }
    sourceLine->setText (displayName);
  }
}


void
CopyEngine::SetDest ( const QString server, 
                  const QString path,
                  const DBType  dbtype,
                  const QString db,
                  const QString user,
                  const QString pass)
{
  SetConn (DBout, server, path, dbtype, db, user, pass);
  if (dodisplay) {
    QString displayName;
    if (dbtype == T_sqlite) {
      displayName = path + "/" + db;
    } else {
      displayName = server; 
    }
    destLine->setText (displayName);
  }
}


void
CopyEngine::SetConn (DBConnection & con,
                  const QString server, 
                  const QString path,
                  const DBType  dbtype,
                  const QString db,
                  const QString user,
                  const QString pass)
{
  if (dbtype == T_web) {
    con.SetMethod (DBConnection::Con_Web);
  } else {
    con.SetMethod (DBConnection::Con_MySql);
    con.SetDBType (Kind(dbtype).toStdString());
  }
  if (path != "") {
    con.SetWebPath (path.toStdString());
  }
  con.ConnectDB (server.toStdString(), db.toStdString(), 
                 user.toStdString(), pass.toStdString());
}

void
CopyEngine::StartEngine (const QString picname,
                  const unsigned long int too_old,
                  const unsigned long int too_recent)
{
  connect (&DBin, SIGNAL (IndexArrival()), this, SLOT (StartCopy ()));
  connect (&DBin, SIGNAL (BlobArrival(char *, quint64)),
           this, SLOT (ReceiveData (char *, quint64)));
  connect (this, SIGNAL (HaveData (char *, quint64)),
           this, SLOT (ReceiveData (char *, quint64)));
  bool ok = DBin.LoadIndex (picname.toStdString(), false, too_old, too_recent);
  Status ("Copying...");
  if (dodisplay) {
    doneButton->setText (tr("Stop"));
  }
}

void
CopyEngine::StartCopy ()
{
  StartNextCycle ();
}

void
CopyEngine::Status (const QString stat, const bool fini)
{
  if (dodisplay) {
    statusLine->setText (stat);
  }
  if (fini) {
    if (dodisplay) {
      doneButton->setText (tr("Done"));
      disconnect (doneButton, SIGNAL (clicked()), this, SLOT (Finish()));
      connect (doneButton, SIGNAL (clicked()), this, SLOT (Quit()));
    }
    if (!dodisplay) {
      Quit ();
    }
  }
}

void
CopyEngine::StartNextCycle ()
{
  if (stopped) {
    Status (tr("Stopped Copy"), true);
    return;
  }
  bool haverec = DBin.ReadIndexRec (currentRec);

  if (!haverec) {
    Status (tr("Finished Copy"), true);
    return;
  }
  size_t imgSize = DBin.ReadImageData (currentRec, imageData);
  if (imgSize > 0) {
    memcpy (dataBuf, imageData.c_str(), imgSize);
    emit HaveData (dataBuf, imgSize);
  }
}

void
CopyEngine::ReceiveData (char * data, quint64 len)
{
  bool ok(false);
  if (len > 0) {
    ok = DBout.InsertRec (currentRec, data, len);
  }
  Report (currentRec, ok);
  StartNextCycle ();
}

void
CopyEngine::Report (const IndexRecord & rec, bool ok)
{
  QString newLine("\n");
  if (ok) {
   newLine.append (tr("copied: "));
   numcopied ++;
 } else {
   newLine.append (tr("failed: "));
   writefailed ++;
 }
 newLine.append (rec.String().c_str());
 if (dodisplay) {
   messageBox->append (newLine);
 } else {
   (*progressStream) << newLine << endl;
 }
}

void
CopyEngine::Finish ()
{
  stopped = true;
  DBin.Disconnect ();
  DBout.Disconnect ();
  QString newLine("\n");
  newLine.append (tr("copied ok: "));
  newLine.append (QString::number(numcopied));
  newLine.append ("\n");
  newLine.append (tr("failed to copy: "));
  newLine.append (QString::number(writefailed));
  newLine.append ("\n");
  if (dodisplay) {
    messageBox->append (newLine);
  }
}

void
CopyEngine::Quit ()
{
  if (pApp) {
    pApp->quit();
  }
}


};


