<!DOCTYPE html>
<html>
<body>

<?php
$servername = "localhost";
$username = "test";
$password = "0000";
$dbname = "practice";

$tableNum = $_GET['table'];
$string = $_GET['data'];

$conn =  mysqli_connect($servername, $username, $password, $dbname);
if($conn->connect_error){
	die("Connection failed: " . $conn->connect_error);
}

$sql = "INSERT INTO sectordata2 (tableNum, orderList) VALUES ('$tableNum', '$string')";

$result = mysqli_query($conn, $sql);
if(!result){
	die('Invalid query : ' . mysql_error());
}
mysqli_close($conn);
?>

</body>
</html>
