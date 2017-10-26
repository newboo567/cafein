
<!DOCTYPE html>
<html>
<body>

<?php
$data = $_GET['data'];
echo $data;

 if($_GET['data']){
  echo "received.";
  //$data = $_GET['data'];
  //$value = explode(',', $data);
  //$num = count($value);

  //for($i = 0; $i < $num; $i++){
  //	echo $value[$i];
  //}
}
else
 echo "not received.";

?>

</body>
</html>
