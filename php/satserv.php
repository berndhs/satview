<!DOCTYPE html>
<?php

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2017, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/
?>

<html>
<head>
  <?php include_once("debug.php"); DelibDebug::open("/tmp/delibdebug.debug.log"); DelibDebug::debug("start"); ?>
  <title>Satserv</title>
  <style>
   .control {
      float:left;
      width:9em;
      height:100%;
    }
    .content {
      float:right;
      width:90%;
      height:200mm;
    }
  </style>
<style>
.onImage {
  z: 100;
  float: left;
}
</style>
<?php include_once ("satservContent.php"); ?>
</head>
<body>


<button style="onImage" onclick="alert('do Fwd');">Next</button>   

<br>
<button style="onImage" onclick="alert('do Back');">Back</button>
  <img src='data:image/jpeg;base64,'  <?php $picdata = take_content(); DelibDebug::debugNotice($picdata); imagejpeg($picdata);?> >
 </body>
</html>
