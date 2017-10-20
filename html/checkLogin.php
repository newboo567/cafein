<?php 
 session_start();
?>
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
    </head>
    <body>
    <?php 

            $name=$_POST["username"];
            $pwd=$_POST["password"];
            
            if(strcmp($name,'yoonyeong')==0)
            {
                if(strcmp($pwd,'sygcafe')==0)
                {
                    session_start();
                    $_SESSION['id']=$name;
                   echo "<h1 style=\"text-align:center;\"><br>Login success <br> Refresh homepage!</h1>";
                }
                else 
                {   
                    echo "<h1 style=\"text-align:center;\"><br><br>Wrong Password</h1>";
                     
                 }
            }    
            else 
            {
                echo "<h1 style=\"text-align:center;\"><br><br>Check your ID</h1>";
            }
            
            ?>
        <script>
            setTimeout("window.close()",1200);
         </script>
    </body>
</html>