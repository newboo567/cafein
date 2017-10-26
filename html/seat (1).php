<!DOCTYPE html>
  <head>
<?php 
@session_start();
?>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">

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
            <li class="nav-item active px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="seat.php">Seats</a>
            </li>
            <li class="nav-item px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="order.php">Orders</a>
            </li>
            <li class="nav-item px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="contact.php">Contact</a>
            </li>
            <?php 
            if ( isset($_SESSION['id'])){//세션을 체크하는 부분으로
                echo "<li class=\"nav-item px-lg-4\">
              <a class=\"nav-link text-uppercase text-expanded\" href=\"statics.php\">Statistics</a>
            </li>" ;//세션 아이디 없으면 index.php로 가게함.
            }
            ?>
          </ul>
        </div>
      </div>
    </nav>

    <div class="container">

      <div class="bg-faded p-4 my-4" style="position:relative;">
        <hr class="divider" >
        <h2 class="text-center text-lg text-uppercase my-0">About
          <strong>SYG Cafe</strong>
        </h2>
        <hr class="divider">
        <div class="row" style="padding-bottom: 350px;">
          <div class="col-lg-6">
            <td>
          <?php 
          
            $servername = "127.0.0.1"; //localhost ip address
            $username = "test";
            $password = "0000";
            $dbname = "practice";
            date_default_timezone_set('Asia/Seoul');
            // Create connection
            $conn = new mysqli($servername, $username, $password, $dbname);
            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            }
            $sql = "SELECT * FROM seatInfo where seatNum IN (3,4,6,28)";
            $result = $conn->query($sql);
      
            if ($result->num_rows > 0) { 
                // output data of each row
                while($row = $result->fetch_assoc()) { 
                    $ch=$row["scheck"];
                    $num=$row["seatNum"];
                    $stime=$row["startTime"];
                    $ttime=$row["totTime"];
                     if($ch == 0){
                        //echo "hi";
                        if($stime == NULL) //아무도 안앉고 있는 자리
                           continue;
                        else //누가 앉았다가 일어난 자리  startTime 초기화, totTime 에 누적
                        {
                            $thisTime = date("H:i:s");
                            
                            $t = strtotime($thisTime)-strtotime($stime);
                            $t2=strtotime($ttime)-strtotime('000000');
                            $f=$t+$t2;
                            
                            $f=date("H:i:s",$f-32400);
                            $sql2 = "UPDATE seatInfo set totTime ='".$f."', startTime=NULL 
                            where seatNum IN (".$num."); ";
                            $r=$conn->query($sql2);
                            
                            }
                        }
                    
                    //누가 앉아있다
                    else {
                       //앉은 첫순간                    
                       if($stime==NULL)
                       {
                           $thisTime = date("H:i:s");
                          
                           $sql3 = "UPDATE seatInfo set startTime='".$thisTime."' 
                            where seatNum IN (".$num."); ";
                           $r2 = $conn->query($sql3);
                         
                           
                       }
                       echo "<div style=\"position:relative;z-index:3\">         
                        <img style=\"position:absolute;\" class=\"img-fluid mb-4 mb-lg-0\" src=\"img/seatNum".$num.".png\" alt=\"\"></img>
                        </div>";
                        
                    }
                }
            }
            
            
       ?>  
               
            
            <div style="position:relative;z-index:2">         
            <img style="position:absolute;" class="img-fluid mb-4 mb-lg-0" src="img/seatNumOther.png" alt=""></img>
            </div>
         	<div style="position:relative;z-index:1">
           	<img style="position:absolute;" class="img-fluid mb-4 mb-lg-0" src="img/seats.jpg" alt=""></img>
            </div>
            </td>
		</div>
		
          
          <div class="col-lg-6 d-none d-lg-block" id="example" style="position:relative;" >
            <p><br>안녕하세요. SYG 카페에 오신 것을 환영합니다. SYG 카페에서는 작은 것부터 정성을 다하는 것을 원칙으로 삼고 있습니다.</p>
        	<p>SYG 카페는 창립자 이수련, 김윤영, 탁금지 세명의 이니셜을 따서 만들어진 이름입니다. 이름을 걸고 고객님들께 차별화된 서비스를 제공할 것을 약속합니다. 시그 카페에서는  주문과정, 음료와 음식의 맛, 합리적인 가격, 따뜻한 분위기, 즐거운 시간을 제공합니다.
        	시그 카페에서 다양한 이벤트와 새로운 카페 문화를 경험해보시기를 바랍니다. </p><p>시그 카페는 카페의 새로운 문화를 만들어 냅니다. 주문과정의 경우 고객은 앉은 자리에서 테이블의 주문 모드를 누르고  원하는 메뉴를 누름으로 간단하게  주문할 수 있습니다. 오락 기능의 경우 가위바위보 게임, ox 퀴즈 게임, 이심전심 게임, 숫자 야구 게임 등이 있습니다. 
        	음악재생 모드에선 미리 저장되어 있던 노래 중 랜덤으로 노래가 흘러나옵니다. 음량 조절은 물론 원하는 분위기나 종류의 노래를 골라서 들을 수 있습니다. 
        	</p><p>시그 카페는 고객님들의 편의를 위하여 의자에 고객이 착석할 경우 카페 운영자의 PC와 통신하여 실시간 좌석 정보를 제공합니다. 또한 주문한 음료의 상태를 홈페이지에서 확인하실 수 있습니다. 그럼 시그 카페에서 풍성한 시간 보내시길 바랍니다. </p></div>
        </div>
      </div>

      <div class="bg-faded p-4 my-4" >
        <hr class="divider" >
        <h2 class="text-center text-lg text-uppercase my-0">Our
          <strong>Seats</strong>
        </h2>
        <hr class="divider">
        <div class="row">
          <div class="col-md-4 mb-4 mb-md-0">
            <div class="card h-100">
              <img class="card-img-top" src="img/seat1.jpg" alt="">
              <div class="card-body text-center">
                <h4 class="card-title m-0">구석 자리
                  <small class="text-muted">19-20</small>
                </h4>
              </div>
            </div>
          </div>
          <div class="col-md-4 mb-4 mb-md-0">
            <div class="card h-100">
              <img class="card-img-top" src="img/seat2.jpg" alt="">
              <div class="card-body text-center">
                <h4 class="card-title m-0">중앙 자리
                  <small class="text-muted">1-7</small>
                </h4>
              </div>
            </div>
          </div>
          <div class="col-md-4">
            <div class="card h-100">
              <img class="card-img-top" src="img/seat3.jpg" alt="">
              <div class="card-body text-center">
                <h4 class="card-title m-0">창가 자리
                  <small class="text-muted">21-24</small>
                </h4>
              </div>
            </div>
          </div>
        </div>
      </div>

    </div>
    <!-- /.container -->

    <footer class="bg-faded text-center py-5">
      <div class="container">
        <p class="m-0">Copyright &copy; 이수련,김윤영,탁금지</p>
      </div>
    </footer>

    <!-- Bootstrap core JavaScript -->
    <script src="vendor/jquery/jquery.min.js"></script>
    <script src="vendor/popper/popper.min.js"></script>
    <script src="vendor/bootstrap/js/bootstrap.min.js"></script>

  </body>
<script language='javascript'> 

  window.setTimeout('window.location.reload()',10000); //5초마다 리플리쉬 시킨다 1000이 1초가 된다. 

</script>

</html>
