#!/bin/sh
# 
# Local Install Script for cloudtrack
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

DIR_BIN=bin

DIR_VIEWER=viewer
DIR_COPIER=copier

PGM_VIEWER=cloudtrack
PGM_COPIER=copydb

if [ ! -d $DIR_BIN ]
then
  mkdir $DIR_BIN
fi


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

