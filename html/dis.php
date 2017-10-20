<!DOCTYPE html>
<html>
<body>

<?php
include("connect.php");

$link = Connection();
$sql = "SELECT * FROM sectorData1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<br> logdate: ". $row["logdate"]. " - sector: ". $row["sector"]. "<br>";
    }
} else {
    echo "0 results";
}


//close connection

?>

</body>
</html>
