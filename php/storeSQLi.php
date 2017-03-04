<?php      
$sqli = new mysqli('localhost','bernd','geronimo','satview');

$filename = $argv[1];
$bb = "<bytes>";
$picname = basename($filename);

$insert = "INSERT INTO `satpics` ( ident, picname, remark, image ) VALUES (?, ?, ?, ?) ";

$ident = sprintf("%d",time(0));
printf("ident is %s\n",$ident);
$remark = "SQLi";
$stmt = $sqli->prepare($insert);
$stmt->bind_param('sssb',$ident,$picname,$remark,$bb);

$fp = fopen($filename,"r");
while (!feof($fp)) {
  $stmt->send_long_data (3,fread($fp, 4096));
}

$rslt = $stmt->execute();

printf ("%d rows inserted.\n",$rslt);

?>
