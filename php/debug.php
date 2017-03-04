<?php

include_once("once.php");

class DelibDebug extends OnlyOne {

private static $dbg = null;


public static function open($filename) 
{
  $dbg = fopen ($filename,"w");
}

public static function checkOpen () 
{
  if ($dbg === null) {
    self::open("~/debug.log");
  }
  $stamp = date("c",time());
  $msg = "auto-opened\n";
  fwrite($dbg,$stamp . $msg);
  fflush($dbg);
}

public static function debug($msg)
{
  self::checkOpen();
  $stamp = date("c",time());
  fwrite($dbg,$stamp . $msg);
  fflush($dbg);
}

public static function debugNotice($bigData)
{
  self::checkOpen();
  self::debug("length ".strlen($bigData));
  fflush($dbg);
}

public static function closeDebug() 
{
  fclose($dbg);
}

}

?>
