<?php

//
//  Copyright (C) 2009 - Bernd H Stramm
//
include_once ("../count.php");


function login ()
{
    
    $the_host = "localhost";
    $the_user = "bernd_weather";
    $the_pass = "quetzalcoatl";
    $the_db = "bernd_weather";
    $con = mysql_connect($the_host, $the_user, $the_pass);
    $ok = mysql_select_db($the_db,$con);

    return $con;

}

function give_index($db_con)
{
    $query = "select ident, picname, storetime, remark from `satpics` where 1";
    $result = mysql_query ($query,$db_con);
   
    $nrows = mysql_num_rows($result);

    $r = 0;
    $fields = array("ident","picname","storetime","remark");
    while ($r<$nrows) {
      $row_array = mysql_fetch_assoc($result);
      echo "record " . $r . "\r\n";
      foreach ($fields as $f) {
        $rawval = trim($row_array[$f]);
        if (strlen($rawval) == 0) {
          $rawval = "0";
	}
	$val = bin2hex($rawval);
	echo $f . " - " . $val . "\r\n";
      }
      $r++;
    }
}

function give_image($db_con, $ident, $picname)
{
  $query = "select `image` from `satpics` where "
       . "`ident`='"
    . mysql_real_escape_string($ident)
    . "' and `picname`='"
    . mysql_real_escape_string($picname)
    . "'";
  $res = mysql_query ($query, $db_con);
  $nrows = mysql_num_rows($res);
  if ($nrows > 0) {
    $row_array = mysql_fetch_row($res); 
    $len_array = mysql_fetch_lengths($res);
    echo "LEN ". $len_array[0] . "\r\nx";
    echo bin2hex($row_array[0]);
  }
}

function report_bad_request ()
{
if (isset ($berndscounterisdefined)) {
   if ($berndscounterisdefined == "yesitis") {
      count_serve('query bad: ' . $_SERVER['PHP_SELF'] . ' ' . $funct);
   }
}
}


// start page code

$supported = array ("index","item");
$funct= $_REQUEST["fn"];


$was_ok = FALSE;
if (in_array($funct,$supported)) {
  $con = login();
  if ($funct == "index") {
    echo "SATVIEW-INDEX\r\n";
    give_index ($con);
    $was_ok=TRUE;
  } else if ($funct == "item") {
    echo "SATVIEW-ITEM\r\n" ;
    $key1=  $_REQUEST["k1"];
    $key2=  $_REQUEST["k2"];
    $real_k1 = pack('H*',$key1);
    $real_k2 = pack('H*',$key2);
    give_image ($con,$real_k1,$real_k2);
    $was_ok=TRUE;
  }
  if ($con) {
    mysql_close($con);
  }
} else {
  $funct = "unknown funct";
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
