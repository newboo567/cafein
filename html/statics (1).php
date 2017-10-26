<!DOCTYPE html>
<!-- <html lang="utf-8"> -->
 <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">

	<style>
	yt{
	   font-size:20pt;
	}
	</style>
    <title>Welcome to SYG Cafe</title>

    <!-- Bootstrap core CSS -->
    <link href="vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom fonts for this template -->
    <link href="https://fonts.googleapis.com/css?family=Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800" rel="stylesheet" type="text/css">
    <link href="https://fonts.googleapis.com/css?family=Josefin+Slab:100,300,400,600,700,100italic,300italic,400italic,600italic,700italic" rel="stylesheet" type="text/css">

    <!-- Custom styles for this template -->
    <link href="css/business-casual.css" rel="stylesheet">

  </head>

  <body>

    <div class="tagline-upper text-center text-heading text-shadow text-white mt-5 d-none d-lg-block">SYG Cafe</div>
    <div class="tagline-lower text-center text-expanded text-shadow text-uppercase text-white mb-5 d-none d-lg-block"><br>Dae heung dong Asan Eng 328 | 02.412.6808</div>

    <!-- Navigation -->
    <nav class="navbar navbar-expand-lg navbar-light bg-faded py-lg-4">
      <div class="container">
        <a class="navbar-brand text-uppercase text-expanded font-weight-bold d-lg-none" href="#">SYG Cafe</a>
        <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarResponsive">
          <ul class="navbar-nav mx-auto">
            <li class="nav-item px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="index.php">Home
                <span class="sr-only">(current)</span>
              </a>
            </li>
            <li class="nav-item px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="seat.php">Seats</a>
            </li>
            <li class="nav-item px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="order.php">Orders</a>
            </li>
            <li class="nav-item px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="contact.php">Contact</a>
            </li>
            <li class="nav-item active px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="statics.php">Statistics</a>
            </li>
          </ul>
        </div>
      </div>
    </nav>
	<div class="container">
	
   <!--  <div class="container">

      <div class="bg-faded p-4 my-4">
        <div class="card card-inverse">
          <img class="card-img img-fluid w-100" src="img/slide-1.jpg" alt="">
          <div class="card-img-overlay bg-overlay"> -->
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
                
                $menu[80];
                // output data of each row
                while($row = $result->fetch_assoc()) { 
                    $strTok = explode(',',$row["orderList"]);
                    $cnt = count($strTok);
                    
                   // echo "<h1>hihi ".$menu[6],"</h1>";
                    
                    for($i=0; $i <$cnt; $i++){
                        $menu[$strTok[$i]]++;
                    }
                }
                echo "<div class=\"bg-faded p-4 my-4\"><h2 style=\"text-align:center;\"> *** 주문 메뉴 통계 *** </h2>";
                for($i=1; $i <=70; $i++)
                {
                    //echo " h:".$menu[$i] ;
                    if($menu[$i]>0)
                    {
                        $h = $menu[$i]*40;
                                  
                        echo "\t<img style=\"position:relative;\" src=\"img/bar1.png\" width=\"".$h."\" height=\"30\">\t....<yt>".foo($i)."</yt> \t(".$menu[$i].")<br>";    
                       // echo foo($i); 
                    }  
        	    }
                echo "</div>";
        	} else {
        	    echo "0 results";
        	}     	
        	
        	$sql2 = "SELECT * FROM seatInfo where totTime>0";
        	$result = $conn->query($sql2);
        	
        	if ($result->num_rows > 0) {
        	    
        	    $menu[80];
        	    // output data of each row
        	    echo "<div class=\"bg-faded p-4 my-4\"><h2 style=\"text-align:center;\"> *** 좌석 현황 통계 *** </h2>";
        	    
        	    while($row = $result->fetch_assoc()) {
       
        	        $t2=strtotime($row["totTime"])-strtotime('000000');
        	       // echo "t2:".$t2;        	 
        	        $t2=floor($t2/60);
        	        
        	        echo "\t<img style=\"position:relative;\" src=\"img/bar1.png\" width=\"".$t2."\" height=\"30\">\t....<yt>".$row['seatNum']."번 자리</yt> \t(".$t2."분)<br>";
  
        	        // echo "<h1>hihi ".$menu[6],"</h1>";
        	  
        	    }
        	    
        	    echo "</div>";
        	} else {
        	    echo "0 results";
        	}
        	$conn->close();
        	?> 
        	    		
     
    <!-- /.container -->

    <footer class="bg-faded text-center py-5">
      <div class="container">
        <p class="m-0">Copyright &copy; 이수련,김윤영,탁금지</p><br>
        	  
		
      </div>
    </footer>

    <!-- Bootstrap core JavaScript -->
    <script src="vendor/jquery/jquery.min.js"></script>
    <script src="vendor/popper/popper.min.js"></script>
    <script src="vendor/bootstrap/js/bootstrap.min.js"></script>

  </body>

</html>
<?php 
function foo($strTok)
{
    $i=1;
    
    switch($strTok){
        case '1':$menu[$i]='아메리카노(아이스)'; break; //6   81
        case '2':$menu[$i]='아메리카노(핫)';break;     //7
        case '3':$menu[$i]='카페라떼(아이스)';break;   //
        case '4':$menu[$i]='카페라떼(핫)';break;     //9
        case '5':$menu[$i]='카페모카(아이스)'; break;   //11  85 86
        case '6':$menu[$i]='카페모카(핫)'; break;
        case '7':$menu[$i]='바닐라 라떼(아이스)'; break;
        case '8':$menu[$i]='바닐라 라떼(핫)'; break;
        case '9':$menu[$i]='카라멜 마키야또(아이스)'; break;
        case '10':$menu[$i]='카라멜 마키야또(핫)'; break;  //17
        case '11':$menu[$i]='카푸치노(아이스)'; break;
        case '12':$menu[$i]='카푸치노(핫)'; break;
        case '13':$menu[$i]='에스프레소'; break;
        case '14':$menu[$i]='모카 프라푸치노';break;
        case '15':$menu[$i]='카라멜 프라푸치노';break;
        case '16':$menu[$i]='화이트 초콜릿 모카 프라푸치노';break;
        case '17':$menu[$i]='바닐라 크림 프라푸치노';break;
        case '18':$menu[$i]='딸기 크림 프라푸치노';break;
        case '19':$menu[$i]='초콜릿 크림 프라푸치노'; break;
        case '20':$menu[$i]='딸기 요거스 스무디'; break;
        case '21':$menu[$i]='망고 바나나 스무디'; break;
        case '22':$menu[$i]='청포도 스무디'; break;
        case '23':$menu[$i]='그린 티 레모네이드 피지오'; break;
        case '24':$menu[$i]='쿨 라임 피지오'; break;
        case '25':$menu[$i]='아이스 샷 그린 티 라떼'; break;
        case '26':$menu[$i]='민트 블렌드 티'; break;
        case '27':$menu[$i]='얼 그레이 티(아이스)'; break;
        case '28':$menu[$i]='얼 그레이 티(핫)'; break;
        case '29':$menu[$i]='캐모마일 티(아이스)'; break;
        case '30':$menu[$i]='캐모마일 티(핫)'; break;
        case '31':$menu[$i]='차이 티(아이스)'; break;
        case '32':$menu[$i]='차이 티(핫)'; break;
        case '33':$menu[$i]='아이스 티'; break;
        case '34':$menu[$i]='밀크 티(아이스)'; break;
        case '35':$menu[$i]='밀크 티(핫)'; break;
        case '36':$menu[$i]='그린 티 라떼(아이스)'; break;
        case '37':$menu[$i]='그린 티 라떼(핫)'; break;
        case '38':$menu[$i]='스팀 우유'; break;
        case '39':$menu[$i]='레모네이드(아이스)'; break;
        case '40':$menu[$i]='딸기 빙수'; break;
        case '41':$menu[$i]='초코 빙수'; break;
        case '42':$menu[$i]='메론 빙수'; break;
        case '43':$menu[$i]='다크 초콜릿 칩 머핀'; break;
        case '44':$menu[$i]='블루베리 머핀'; break;
        case '45':$menu[$i]='호두 크림치즈 베이글'; break;
        case '46':$menu[$i]='치즈 베이글'; break;
        case '47':$menu[$i]='블루베리 베이글'; break;
        case '48':$menu[$i]='허니 브레드'; break;
        case '49':$menu[$i]='구운 바나나 케이크'; break;
        case '50':$menu[$i]='레드벨벳 크림치즈 케이크'; break;
        case '51':$menu[$i]='레몬 타르트'; break;
        case '52':$menu[$i]='티라미수'; break;
        case '53':$menu[$i]='프렌치 어니언 샌드위치'; break;
        case '54':$menu[$i]='단호박 에그 샌드위치'; break;
        case '55':$menu[$i]='뉴 비프 브리또'; break;
        case '56':$menu[$i]='블루베리 마카롱'; break;
        case '57':$menu[$i]='바닐라 마카롱'; break;
        case '58':$menu[$i]='초콜릿 마카롱'; break;
        case '59':$menu[$i]='초콜릿 푸딩'; break;
        case '60':$menu[$i]='바닐라 푸딩'; break;
        case '61':$menu[$i]='아메리카노(아이스)L'; break; //메뉴 선택 후 사이즈 다시 물어보기 81
        case '62':$menu[$i]='아메리카노(핫)L';break;
        case '63':$menu[$i]='카페라떼(아이스)L';break;
        case '64':$menu[$i]='카페라떼(핫)L';break;
        case '65':$menu[$i]='카페모카(아이스)L'; break; //86 ,85
        case '66':$menu[$i]='카페모카(핫)L'; break; // 87
        case '67':$menu[$i]='바닐라 라떼(아이스)L'; break;
        case '68':$menu[$i]='바닐라 라떼(핫)L'; break;
        case '69':$menu[$i]='카라멜 마키야또(아이스)L'; break;
        case '70':$menu[$i]='카라멜 마키야또(핫)L'; break;
        
    }
    return $menu[1];
}

?>