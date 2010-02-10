#include "import.h"
#include <iostream>
#include "delib-debug.h"

//
//  Importright (C) 2009 - Bernd H Stramm 
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

ImportEngine::ImportEngine (QApplication * pA, const bool dis)
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

ImportEngine::~ImportEngine ()
{}

void
ImportEngine::Init ()
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
ImportEngine::SetApp (QApplication * pA)
{
  pApp = pA;
}

QString
ImportEngine::Kind (const DBType t)
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

int
ImportEngine::GetInputFiles ()
{
  QDir::Filters filter (QDir::Files | QDir::Readable);
  filesys.setFilter (filter);
  QStringList jpegNames;
  jpegNames << "*_*.jpg" << "*_*.JPG" << "*_*.jpeg" << "*_*.JPEG";
  localDir.setNameFilters (jpegNames);
  jpegFiles.append (localDir.entryList (jpegNames, filter));
  return jpegFiles.size();
}

void
ImportEngine::SetSourceDir (const QString path)
{
  inPath = path;
  localDir.setPath (inPath);
  if (dodisplay) {
    QString displayName;
    displayName = path + "/";
    sourceLine->setText (displayName);
  }
}


void
ImportEngine::SetDest ( const QString server, 
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
ImportEngine::SetConn (DBConnection & con,
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
ImportEngine::StartEngine (const QString picname,
                  const unsigned long int too_old,
                  const unsigned long int too_recent)
{
  onlyPicname = picname;
  tooOld = too_old;
  tooRecent = too_recent;
  connect (this, SIGNAL (HaveData (char *, quint64)),
           this, SLOT (ReceiveData (char *, quint64)));
  bool ok = GetInputFiles () >= 0;
  if (ok) {
    StartImport ();
  }
  Status ("Importing...");
  if (dodisplay) {
    doneButton->setText (tr("Stop"));
  }
}

void
ImportEngine::StartImport ()
{
  fileit = jpegFiles.begin();
  StartNextCycle ();
}

void
ImportEngine::Status (const QString stat, const bool fini)
{
  if (dodisplay) {
    statusLine->setText (stat);
  }
  if (fini) {
    if (dodisplay) {
      doneButton->setText (tr("Done"));
      disconnect (doneButton, SIGNAL (clicked()), this, SLOT (Finish()));
      connect (doneButton, SIGNAL (clicked()), this, SLOT (Quit()));
      doneButton->update ();
    }
    if (!dodisplay) {
      Quit ();
    }
  }
}

bool
ImportEngine::GetFileIndexRec (IndexRecord & rec, QString & fname)
{
  if (fileit == jpegFiles.end()) {
    return false;
  }
  bool fileGood(false);
  do {
    QString nextName = *fileit;
    fileit++;
    QString name = nextName.section ('_',0,0);
    int namelen = name.length ();
    QString rest = nextName.section ('_',1,1);
    QString stamp = rest.section ('.',0,0);
    unsigned long int ident = stamp.toULong ();
    if (namelen > 0 && ident != 0) {
      rec.ident = ident;
      rec.picname = name.toStdString();
      rec.remark = nextName.toStdString();
      rec.storetime = "";
      fname = nextName;
      fileGood = true;
    }
  } while (!fileGood && fileit != jpegFiles.end());
  return fileGood;
}

size_t
ImportEngine::ReadFileImageData (const QString filename, 
                                 QByteArray & data)
{
  QFile file (inPath + "/" + filename);
  bool ok = file.open (QIODevice::ReadOnly);
  if (ok) {
    data = file.readAll ();
    return data.size();
  }
  return 0;
}

void
ImportEngine::StartNextCycle ()
{

  if (stopped) {
    Status (tr("Stopped Import"), true);
    return;
  }
  QString filename;
  bool haverec = GetFileIndexRec (currentRec, filename);

  if (!haverec) {
    Status (tr("Finished Import"), true);
    return;
  }
  
  size_t imgSize = ReadFileImageData (filename, dataBytes);
  if (imgSize > 0) {
    memcpy (dataBuf, dataBytes.data(), imgSize);
    emit HaveData (dataBuf, imgSize);
  }
}

void
ImportEngine::ReceiveData (char * data, quint64 len)
{
  bool ok(false);
  if (len > 0) {
    ok = DBout.InsertRec (currentRec, data, len);
  }
  Report (currentRec, ok);
  StartNextCycle ();
}

void
ImportEngine::Report (const IndexRecord & rec, bool ok)
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
ImportEngine::Finish ()
{
  stopped = true;
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
ImportEngine::Quit ()
{
  if (pApp) {
    pApp->quit();
  }
}


};


