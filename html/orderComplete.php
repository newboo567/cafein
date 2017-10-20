<?php 
    $idx=$_GET['idx'];
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
    $sql = "UPDATE sectordata2 set isOut=true where idx=".$idx.";";
    $conn->query($sql);
    #echo $sql;
?>
    <script langauge=\"javascript\">
     window.open('http://192.168.100.8/led/7','signal transferred','width=400,height=250,left=600,top=300');
    </script>

<?php
    echo "<meta http-equiv='refresh' content='0; url=order.php'>"; 
    
?>