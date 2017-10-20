<!DOCTYPE html>
<html>
<body>

<?php
$servername = "localhost";
$username = "test";
$password = "0000";
$dbname = "practice";

//$sector = $_GET['sector'];
$tableNum = $_GET['table'];
$string = $_GET['data'];

$conn =  mysqli_connect($servername, $username, $password, $dbname);
if($conn->connect_error){
	die("Connection failed: " . $conn->connect_error);
}
//$dateString = date("Y-m-d H:i:s", $phptime);
$dataString = date("Y-n-d H:i:s");
//$sql = "INSERT INTO sectorData1 (logdate, sector) VALUES ('$dateString', '$sector')";
$sql = "INSERT INTO sectordata2 (orderDate, tableNum, orderList) VALUES ('$dateString', '$tableNum', '$string')";

$result = mysqli_query($conn, $sql);
if(!result){
	die('Invalid query : ' . mysql_error());
}
mysqli_close($conn);
?>

</body>
</html>
