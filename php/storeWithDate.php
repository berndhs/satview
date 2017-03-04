<?php      
$sqli = new mysqli('localhost','bernd','geronimo','satview');

$filename = $argv[2];
$stamp = $argv[1];
echo "\nargs file " . $filename . " and " . $stamp . "\n";
$bb = "<bytes>";
$picname = basename($filename);

$insert = "INSERT INTO `satpics` ( ident, picname, remark, stamp, image ) VALUES (?, ?, ?, ?, ?) ";

$ident = $stamp;
$datestamp = date("c",$stamp);
printf("ident is %s\n",$ident);
$remark = "SQLi";
$stmt = $sqli->prepare($insert);
$stmt->bind_param('ssssb',$ident,$picname,$remark, $datestamp, $bb);

$fp = fopen($filename,"r");
while (!feof($fp)) {
  $stmt->send_long_data (4,fread($fp, 4096));
}

$rslt = $stmt->execute();
echo "stamp " . $stamp . " is " . $datestamp . " for " . $filename . "\n";

printf ("%d rows inserted.\n",$rslt);

?>
