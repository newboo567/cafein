<!DOCTYPE html>
<html>
<body>

<?php
$servername = "127.0.0.1"; //localhost ip address
$username = "test";
$password = "0000";
$dbname = "practice";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT * FROM sectordata2";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<br> idx: ". $row["idx"]. " - orderDate: ". $row["orderDate"]. " - tableNum: ". $row["tableNum"]. " - orderList: ". $row["orderList"]. " - isOut: ". $row["isOut"]. "<br>";
    }
} else {
    echo "0 results";
}

$conn->close();
?> 

</body>
</html>
