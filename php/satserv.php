<?php

//
//  Copyright (C) 2017 - Bernd H Stramm
//
// include_once ("../count.php");


function login ()
{
    
    $the_host = "localhost";
    $the_user = "berndsat";
    $the_pass = "";
    $the_db = "satview";
    $con = new mysqli ($the_host, $the_user, $the_pass,$the_db);
//    $ok = mysql_select_db($the_db,$con);
if ($con->connect_errno) {
    // The connection failed. What do you want to do? 
    // You could contact yourself (email?), log the error, show a nice page, etc.
    // You do not want to reveal sensitive information

    // Let's try this:
      header ("HTTP/1.0 204 Bad Data\r\n",false,204);

    // Something you should not do on a public site, but this example will show you
    // anyways, is print out MySQL error related information -- you might log this
    echo "Error: Failed to make a MySQL connection, here is why: \n";
    echo "Errno: " . $mysqli->connect_errno . "\n";
    echo "Error: " . $mysqli->connect_error . "\n";
    
    // You might want to show them something nice, but we will simply exit
    exit;
}
    return $con;

}

function give_index($db_con, $imin, $imax)
{
   echo "we have give_index<br>";
    $query = "select ident, picname, remark from `satpics` where 1";
    
    if ($imin != "") {
        $query .= " and ident >= " . $imin;
    }
    if ($imax != "") {
        $query .= " and ident <= " . $imax;
    }
    $result = $db_con->query ($query);
   
    $nrows = $result->num_rows;
    echo "there are rows, $nrows <br>";
    if ($nrows > 0) {
      echo "SATVIEW-INDEX\r\n";
      $r = 0;
      $fields = array("ident","picname","remark");
      while ($r<$nrows) {
        $row_array = $result->fetch_assoc();
        echo "record " . $r . "\r\n";
        var_dump($row_array);
        foreach ($fields as $f) {
          $rawval = trim($row_array[$f]);
          var_dump($rawval);
          if (strlen($rawval) == 0) {
            $rawval = "0";
          }
  	  $val =  bin2hex($rawval);
  	  //echo $f . " - " . $val . "\r\n";
        }
        $r++;
      }
    } else {
      header ("HTTP/1.0 204 No Data\r\n",false,204);
    }
}

function give_image($db_con, $ident, $picname)
{
  $query = "select `image`,`remark` from `satpics` where "
       . "`ident`='"
    . $ident
    . "' and `picname`='"
    . $picname
    . "'";
    $res = $db_con->query ($query);
  $nrows = $res->num_rows;
  
  if ($nrows > 0) {
    $row_array = $res->fetch_array();
    $len = count($row_array[0]);
//    echo "SATVIEW-ITEM\r\n" ;
//    echo "LEN ". $len . "\r\n";
    //var_dump($row_array);
//    echo bin2hex($row_array[0]);
header ("Content-type: image/jpg");
     echo $row_array[0];
  } else {
  
      header ("HTTP/1.0 204 No Data\r\n",false,204);
    
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


// start page code

$supported = array ("index","item");
$funct= $_REQUEST["fn"];

$was_ok = FALSE;
$yeah = in_array($funct,$supported);
if ($yeah) {
  $con = login();
  if ($funct == "index") {
    echo "it is INDEX!!<br>";
    $ndxmin = $_REQUEST["min"];
    $ndxmax = $_REQUEST["max"];
    
    give_index ($con,$ndxmin,$ndxmax);
    $was_ok=TRUE;
  } else if ($funct == "item") {
    $key1=  $_REQUEST["k1"];
    $key2=  $_REQUEST["k2"];
    $real_k1 = pack('H*',$key1);
    $real_k2 = pack('H*',$key2);
    give_image ($con,$key1,$key2);
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

?>
