#!/bin/sh
# 
# Build Script for cloudtrack
#
# //
# //  Copyright (C) 2010 - Bernd H Stramm
# //
#
# // This program is distributed under the terms of 
# // the GNU General Public License version 3 
# //
# // This software is distributed in the hope that it will be useful, 
# // but WITHOUT ANY WARRANTY; without even the implied warranty 
# // of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
# //

PROJECT=cloudtrack
DOT_PRO_FILE=$PROJECT.pro

DIR_BIN=bin

DIR_VIEWER=viewer
DIR_COPIER=copier

PGM_VIEWER=cloudtrack
PGM_COPIER=copydb


QM=`which qmake`
QM4=`which qmake-qt4`

if [ -z $QM ] 
then
  RES3="0"
else
  RES3=`$QM -query QT_VERSION`
fi
if [ -z $QM4 ]
then
  RES4="0"
else
  RES4=`$QM4 -query QT_VERSION`
fi

MATCH3=`expr match $RES3 '4.'`
MATCH4=`expr match $RES4 '4.'`

QMAKE=qmake

if [ $MATCH3 -eq 2 ]
then
  QMAKE=$QM
else
  if [ $MATCH4 -eq 2 ]
  then
    QMAKE=$QM4
  else
    echo "cannot find qmake for Qt4"
    exit 1
  fi
fi

echo
echo Using $QMAKE
echo
echo Trying to build Makefile
echo

$QMAKE $DOT_PRO_FILE
OK=$?
if [ $OK -ne 0 ]
then
  echo Failed to build Makefile
  exit 1
fi

make clean

make release


if [ -e $DIR_VIEWER/$PGM_VIEWER ]
then
  install $DIR_VIEWER/$PGM_VIEWER $DIR_BIN
else
  echo "Failed to build $PGM_VIEWER"
fi

if [ -e $DIR_COPIER/$PGM_COPIER ]
then
  install $DIR_COPIER/$PGM_COPIER $DIR_BIN
else
  echo "Failed to build $PGM_COPIER"
fi

ls -l $DIR_BIN



