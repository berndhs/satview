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
      width:30px;
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
</head>
<body>
<hr>
<img src="wcir.jpg" width="200px" style="border:3px;border-style:dashed;border-color:#ff0000;" >

  <?php
   include_once("satserv-functions.php");
   $p = fopen("space/cnt.txt","r");
   $data = fread($p,2048);
   //fseek($p,0);
   fclose($p);
   $p = fopen("space/cnt.txt","a");
   fwrite($p,"12345");
var_dump($p);
   fclose($p);
   echo "<p>number is " . $data . "||\n<br></p>";
   echo "<dev>\n";
   $con = login();
   $now = time();
   $hoursago = time() - 10*60 * 60;
   echo "<hr>\n";
   echo give_index($con,$hoursago,$now);
   echo "\n<hr>\n";
   $tmp = "satpic" ;//tempnam(".","tmp");
   $file = take_content($con,$tmp);
   echo "pic in file " . $file ."<br>";
  echo '<img width="300px" style="border:3px; border-style:dotted;border-color:#ff00ff;" ';

   echo "src=\"" . $file . "\"";
   echo ">\n\r";
   echo "<br>file size " . filesize($file);
  ?>
   
  <br>
  <br>
  <p> a paragraph </p>
  <hr>
  <p align="left">
  this is after the image
  </p>
 </body>
</html>
