<?php
$con = mysql_connect("localhost","bernd_weather","quetzalcoatl");

$db = "bernd_weather";

$filename = $argv[1];

$ok = mysql_select_db($db,$con);

$t_str = sprintf("%d",time(0));

$query_1 = "INSERT INTO `satpics` ( ident, picname, remark, image ) VALUES ( ";
$query_2 = $t_str;
$query_3 = $filename;
$query_4 = "from php";
$query_5 = mysql_real_escape_string(file_get_contents($filename));

mysql_query( $query_1 
       . $query_2 
       . " , '" 
       . mysql_real_escape_string($filename)
       . "' , '" 
       . $query_4 
       . "' , '"
       . $query_5 
       . "' )"
	     ) ;
$affected = mysql_affected_rows($con);
echo $affected . " rows affected \r\n";

mysql_close($con);
?>