<?php


function countme ($name2count = "") {
		$host="localhost";
		$user="root";
		$pass="zuiderzee";

		$dbname = "bernd_berndstats";
		$table="counters";
		
		if ($name2count == "") {
		   $name2count = $_SERVER['PHP_SELF'];
		}
		
		$db_serv = mysql_connect ($host, $user, $pass);
		
		$ok = mysql_select_db ($dbname, $db_serv);
		if (! $ok) {
		    return FALSE;
		}
		$qname = "'" . mysql_real_escape_string($name2count) . "'";
		$ask_this = "update ". $table . "  set loadcount=loadcount+1 where keyname=" . $qname;
		$answer = mysql_db_query ($dbname, $ask_this);
		$details = mysql_affected_rows ($db_serv);
		if ($details < 1) {
		   $addpage = "insert into " . $table . " values (" . $qname . ", 1)" ;
		   $answer = mysql_db_query ($dbname, $addpage);
		   $details = mysql_affected_rows ($db_serv);
		}
		mysql_close($db_serv);
		return TRUE;
}


function count_serve ($dest) {
		$host="localhost";
		$user="root";
		$pass="andromeda";
		$dbname = "bernd_berndstats";
		$table="forwarding";
      $customer = $_SERVER['REMOTE_ADDR'];
		$db_serv = mysql_connect ($host, $user, $pass);
		$err1 = mysql_error();
		$ok = mysql_select_db ($dbname, $db_serv);
                $err2 = mysql_error ();
		if (! $ok) {
		    return FALSE;
		}
		$qdest = "'" . mysql_real_escape_string($dest) . "'";
		$qcust = "'" . mysql_real_escape_string($customer) . "'";
		$ask_this = "UPDATE " . $table . " SET count=count+1 WHERE "
		            . "destination=" . $qdest 
		            . " AND user=" . $qcust ;
		$answer = mysql_db_query ($dbname, $ask_this);
		$n_rows = mysql_affected_rows ($db_serv);
		if ($n_rows < 1) {
		   $addrow = "INSERT INTO " . $table . " VALUES ("
		             . $qdest . " , " . $qcust . " , 1 )" ;
		   $answer = mysql_db_query ($dbname, $addrow);
		   $n_rows = mysql_affected_rows ($db_serv);
		   // if this fails, nothing much we can do about it
		}
                $ask_pattern = "REPLACE INTO latest SET user = '%s'";
                $ask_this = sprintf ($ask_pattern, $customer);
                $answer = mysql_db_query ($dbname, $ask_this);
		$ask_pattern = "REPLACE INTO latestpage VALUES ('%s', '%s', NULL)";
                $ask_this = sprintf ($ask_pattern, $dest, $customer);
                $answer = mysql_db_query ($dbname, $ask_this);
		mysql_close ($db_serv);
		return TRUE; 
}


$berndscounterisdefined = "yesitis";

?>
