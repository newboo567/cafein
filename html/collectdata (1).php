<!DOCTYPE html>
<html>
<body>

<?php
$servername = "localhost";
$username = "test";
$password = "0000";
$dbname = "practice";

$sector = $_GET['sector'];
$conn =  mysqli_connect($servername, $username, $password, $dbname);
if($conn->connect_error){
	die("Connection failed: " . $conn->connect_error);
}
$sql = "INSERT INTO sectorData2 (tableNum, orderList) VALUES (7,'$sector')";
$result = mysqli_query($conn, $sql);
if(!result){
	die('Invalid query : ' . mysql_error());
}
mysqli_close($conn);
?>

</body>
</html>
