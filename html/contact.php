<!DOCTYPE html>
<html lang="utf-8">
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

	<script
	src="https://maps.googleapis.com/maps/api/js?libraries=places&key=AIzaSyDMrI-K3E7MIkrxe8bmXSBXMUIVwM84qo4"></script>
	
	 <script type="text/javascript" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" src="http://maps.google.com/maps/api/js?key=AIzaSyDMrI-K3E7MIkrxe8bmXSBXMUIVwM84qo4&sensor=false&language=ko"></script>
            <script>
            function initialize(){
            	var myLatlng = new google.maps.LatLng(37.566454,126.948307);
            	var mapOptions = {
            			zoom:15,
            			center:myLatlng,
            			mapTypeId: google.maps.MapTypeId.ROADMAP
            	};
            	var map = new google.maps.Map(document.getElementById
            			("map_canvas"),mapOptions);
            	var marker= new google.maps.Marker({
            		position:myLatlng,
            		map:map,
            	})
            }
            
            
            </script>
  </head>

  <body onload="initialize()">

    <div class="tagline-upper text-center text-heading text-shadow text-white mt-5 d-none d-lg-block">SYG Cafe</div>
    <div class="tagline-lower text-center text-expanded text-shadow text-uppercase text-white mb-5 d-none d-lg-block"><br>Dae heung dong Asan Eng 328 | 02.412.6808</div>

    <!-- Navigation -->
    <nav class="navbar navbar-expand-lg navbar-light bg-faded py-lg-4">
      <div class="container">
        <a class="navbar-brand text-uppercase text-expanded font-weight-bold d-lg-none" href="index.html">SYG Cafe</a>
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
            <li class="nav-item active px-lg-4">
              <a class="nav-link text-uppercase text-expanded" href="contact.php">Contact</a>
            </li>
            <?php 
            if ( isset($_SESSION['id'])){
                echo "<li class=\"nav-item px-lg-4\">
              <a class=\"nav-link text-uppercase text-expanded\" href=\"statics.php\">Statistics</a>
            </li>" ;
            }
            ?>
          </ul>
        </div>
      </div>
    </nav>

    <div class="container">

      <div class="bg-faded p-4 my-4">
        <hr class="divider">
        <h2 class="text-center text-lg text-uppercase my-0">Contact
          <strong>Cafein Team</strong>
        </h2>
        <hr class="divider">
        <div class="row">
          <div class="col-lg-8">
            <div class="embed-responsive embed-responsive-16by9 map-container mb-4 mb-lg-0"
             id="map_canvas"></div>
         </div>
          <div class="col-lg-4">
            <h5 class="mb-0">Phone:</h5>
            <div class="mb-4">02.412.6808</div>
            <h5 class="mb-0">Email:</h5>
            <div class="mb-4">
              yoonyeong64@naver.com
            </div>
            <h5 class="mb-0">Address:</h5>
            <div class="mb-4">
              Dae heung dong Asan Eng 328
              <br>
              South Korea Seoul
            </div>
          </div>
        </div>
      </div>

      <div class="bg-faded p-4 my-4">
        <hr class="divider">
        <h2 class="text-center text-lg text-uppercase my-0">Contact
          <strong>Form</strong>
        </h2>
        <hr class="divider">
        <form>
          <div class="row">
            <div class="form-group col-lg-4">
              <label class="text-heading">Name</label>
              <input type="text" class="form-control">
            </div>
            <div class="form-group col-lg-4">
              <label class="text-heading">Email Address</label>
              <input type="email" class="form-control">
            </div>
            <div class="form-group col-lg-4">
              <label class="text-heading">Phone Number</label>
              <input type="tel" class="form-control">
            </div>
            <div class="clearfix"></div>
            <div class="form-group col-lg-12">
              <label class="text-heading">Message</label>
              <textarea class="form-control" rows="6"></textarea>
            </div>
            <div class="form-group col-lg-12">
              <button type="submit" class="btn btn-secondary">Submit</button>
            </div>
          </div>
        </form>
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

    <!-- Zoom when clicked function for Google Map -->
    <script>
      $('.map-container').click(function () {
        $(this).find('iframe').addClass('clicked')
      }).mouseleave(function () {
        $(this).find('iframe').removeClass('clicked')
      });
    </script>

  </body>

</html>
