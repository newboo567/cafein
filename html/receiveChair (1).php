<!DOCTYPE html>
<html>
<body>
//this code if for chair

<?php
$servername = "localhost";
$username = "test";
$password = "0000";
$dbname = "practice";


$seatNum = $_GET['seat'];
$scheck = $_GET['check'];
$conn =  mysqli_connect($servername, $username, $password, $dbname);
if($conn->connect_error){
	die("Connection failed: " . $conn->connect_error);
}
//$dateString = date("Y-m-d H:i:s", $phptime);
//$sql = "INSERT INTO sectorData1 (logdate, sector) VALUES ('$dateString', '$sector')";
//$sql = "INSERT INTO sectordata2 (orderDate, tableNum, orderList) VALUES ('$dateString', 7, '$string')";
$sql = "UPDATE seatInfo SET scheck = $scheck where seatNum = $seatNum";
$result = mysqli_query($conn, $sql);
if(!result){
	die('Invalid query : ' . mysql_error());
}
mysqli_close($conn);
?>

</body>
</html>
