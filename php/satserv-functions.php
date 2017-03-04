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


function login ()
{
    
    $the_host = "localhost";
    $the_user = "root";
    $the_pass = "geronimo";
    $the_db = "satview";
    $con = new mysqli ($the_host, $the_user, $the_pass,$the_db);
    if ($con->connect_errno) {
        // The connection failed. What do you want to do? 
        // You could contact yourself (email?), log the error, show a nice page, etc.
        // You do not want to reveal sensitive information

        // Let's try this:
        echo "Bad database connection<br>";

        // Something you should not do on a public site, but this example will show you
        // anyways, is print out MySQL error related information -- you might log this
        echo "Error: Failed to make a MySQL connection, here is why: <br>\n";
        echo "Errno: " . $mysqli->connect_errno . "<br>\n";
        echo "Error: " . $mysqli->connect_error . "<br>\n";
    
        // You might want to show them something nice, but we will simply exit
        return "";
    }
    return $con;
}

function button_code ($txt, $link)
{
   $but = "<button type=\"button\"  onclick=\"route('$link')\" >". $txt . "</button>"; 
   return $but;
}

function item_button ($ndx, $filename,$stamp)
{
  $lnk = "singlepic.php";
  $lnk .= "?t=" . $ndx;
  $lnk .= "&pic=" . $filename;
  $localt = strtotime($stamp);
  
  return button_code (date("c",$ndx) . " <b>" . $filename . "</b> stored " . date(DATE_RFC2822,$localt), $lnk);
}

function give_index($db_con, $imin, $imax)
{
    header ("HTTP/1.1 200 OK\r\n");
    echo "<br>";
    echo "<script> function route (lnk) {alert(lnk); document.location=lnk; } </script>";
   
    $cntlpage = "satservControl.php";
    echo button_code("Foo",$cntlpage) . " to " . $cntlpage;
    echo "<br>";
    $query = "select ident, picname, stamp from `satpics` where";
    
    if ($imin == "") {
      $imin = time() - 3 * 24 * 60 * 60;
    }
    $query .= " ident >= " . $imin;
    if ($imax == "") {
       $imax = time() + 3 * 60* 60;
    }
    $query .= " and ident <= " . $imax . " order by ident DESC limit 10;";
    $result = $db_con->query ($query);
    echo " query is " . $query . "<br>";   
    $nrows = $result->num_rows;
    echo "there are rows, $nrows <br>";
    if ($nrows > 0) {
      $r = 0;
      $fields = array("ident","picname","stamp");
      while ($r<$nrows) {
        $row_array = $result->fetch_assoc();
        // echo "record " . $r . "\r\n";
        
        $ident = intval($row_array["ident"]);
        $picname = $row_array["picname"];
        $stamp = $row_array["stamp"];
        echo $ident . " " . item_button($ident,$picname,$stamp) . "<br>\r\n";
        foreach ($fields as $f) {
          $rawval = trim($row_array[$f]);
          
          if (strlen($rawval) == 0) {
            $rawval = "0";
          }
        }
        $r++;
      }
      echo "query was " . $query . "  ";
    } else {
      header ("HTTP/1.0 204 No Data\r\n",false,204);
    }
}

function take_content ($con,$tmpfile)
{
  
  $now = time() - 6*60; // 3 secs from now
  $data = give_image ($con,$now);
  $count = strlen($data);
  echo "==== count is " . $count . "<br>";
  $dest = "/var/www/html/satpic.jpg";
  system("rm " . $dest);
  $fd = fopen($dest,"w");
  fwrite($fd,$data);
  fflush($fd);
  system("sync;sync");
  fclose($fd);
  echo "\n\nwrote ".$dest . " bytes " . strlen($data);
  echo " " . filesize($dest);
  echo "\n\r<br>";
  return $dest;
}

function give_image($db_con, $ident)
{
  echo "db_con is ";
  var_dump($db_con);
  $query = "select image,picname, remark from satpics where "
       . "ident < '"
    . $ident
    . "' order by ident desc limit 1;";
    $res = $db_con->query ($query);
  $nrows = $res->num_rows;
  echo "query " . $query . "<br>\n\r";
  echo "result is " ;
  var_dump($res);
  if ($nrows > 0) {
    $row_array = $res->fetch_assoc();
    $len = count($row_array[0]);
    
//    echo "SATVIEW-ITEM\r\n" ;
//    echo "LEN ". $len . "\r\n";
    //var_dump($row_array);
//    echo bin2hex($row_array[0]);
     return ( $row_array["image"]);
  } else {
      echo "zero rows<br>";
    
  }
}

function report_bad_request ()
{
if (isset ($berndscounterisdefined)) {
   if ($berndscounterisdefined == "yesitis") {
      count_serve('query bad: ' . $_SERVER['PHP_SELF'] . ' ' . $funct);
   }
}  
header ("HTTP/1.1 400 Bad Request Format\r\n");
}


function give_content($r) 
{
  $req = $r;
 // var_dump ($req);
  $supported = array ("index","item");
  $funct= $req["fn"];

  $was_ok = FALSE;
  if ($funct == "") {
    $funct = "index";
  }
  $yeah = in_array($funct,$supported);
  if ($yeah) {
    $con = login();
    if ($funct == "index") {
      if (in_array("min",$req)) {
        $ndxmin = $req["min"];
      } else {
        $ndxmin = 1487789702; // time() -  24 * 60 * 69;   // 1 days
      }
      if (in_array("max",$req)) {
        $ndxmax = $req["max"];
      } else {
        $ndxmax = time() + 24 * 60 * 60;  // tomorrow
      }
    
      give_index ($con,$ndxmin,$ndxmax);
      $was_ok=TRUE;
    } else if ($funct == "item") {
      $key1=  $req["k1"];
      $key2=  $req["k2"];
      $real_k1 = pack('H*',$key1);
      $real_k2 = pack('H*',$key2);
    if ($con) {
      mysqli_close($con);
    }
      return (give_image ($con,$key1,$key2));
      $was_ok=TRUE;
    }
    if ($con) {
      mysqli_close($con);
    }
  } else {
    $funct = "unknown funct";
    echo $func . "<br>";
  }
  if (isset ($berndscounterisdefined)) {
     if ($berndscounterisdefined == "yesitis") {
        countme();
        count_serve('query ' . $_SERVER['PHP_SELF'] . ' ' . $funct);
     }
  }
  if (!$was_ok) {
    report_bad_request();
  }
}
//header ("HTTP/1.1 200 OK\r\n");
//give_content($_REQUEST);
?>
