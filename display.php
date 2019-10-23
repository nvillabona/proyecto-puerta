<?php
        $url=$_SERVER['REQUEST_URI'];
        header("Refresh: 3; URL=$url"); // Refresh the webpage every second
        ?>
    <html>

    <head>
        <title>Arduino Ethernet Database</title>
        <!-- id izq                       ----------------------------------------------------- -->
        <div class="izq">
            <link rel="stylesheet" href="style.css">
            <script src="functions.js"></script>

            <body onload="ConteoRegresivo()">

    </head>

    <body>

        <h1>Arduino Data Logging to Database</h1>
        <table border="10" cellspacing="0" cellpadding="4">
            <tr>
                <td class="table_titles">ID</td>
                <td class="table_titles">Date and Time</td>
                <td class="table_titles">Temperature</td>
                <td class="table_titles">Humidity</td>
                <td class="table_titles">Heat_index</td>
            </tr>
            <?php
      include('connection.php');
      //obtain data from db
      $result = mysqli_query($con,'SELECT * FROM data ORDER BY id DESC');
      // Process every record
      $oddrow = true;
      while($row = mysqli_fetch_array($result))
      {
      if ($oddrow)
      {
      $css_class=' class="table_cells_odd"';
      }
      else
      {
      $css_class=' class="table_cells_even"';
      }
      $oddrow = !$oddrow; 
      echo "<tr>";
      echo "<td '.$css_class.'>" . $row['id'] . "</td>";
      echo "<td '.$css_class.'>" . $row['event'] . "</td>";
      echo "<td '.$css_class.'>" . $row['temperature'] . "</td>";
      echo "<td '.$css_class.'>" . $row['humidity'] . "</td>";
      echo "<td '.$css_class.'>" . $row['heat_index'] . "</td>";
      echo "</tr>"; 
      }
      
      // Close the connection
      mysqli_close($con);
    ?>
        </table>
        </div>
        <!-- class der ------------------------>
        <div class="der">



            <img src="https://image.freepik.com/vector-gratis/vector-plantilla-puerta-abierta_23-2147495012.jpg" style="width: 100px ; height: 100px">
            <h2>Sistema de seguridad </h2>
            <!-- <div id="contador"></div> -->
            <h4><i> </i></h4>
            <?php
          include('connection.php');
          date_default_timezone_set('America/Bogota');
          $date_fromDB = mysqli_query($con,'SELECT * FROM data ORDER by event DESC  '); //LIMIT 1
          
          $date = strtotime("October 22, 2019 4:00 AM");
          $remaining = $date - time();
          $days_remaining = floor($remaining / 86400);
          $hours_remaining = floor(($remaining % 86400) / 3600);
          $seconds_remaining = floor(($hours_remaining)/3600);
          echo "There are $days_remaining days and $hours_remaining hours left seconds $seconds_remaining";
          $timezone = date_default_timezone_get();
          $time = time();
          echo "The current server timezone is: " . $timezone . "and time is " . $time;
          
          //trae el valor de event en la consulta sql
          $fila = mysqli_fetch_array($date_fromDB);
          echo "<h2>" .$fila['event'] .'</h2>';
        
          
          //$date = date('m/d/Y h:i:s a', time());
          //echo $date;
        ?>

        </div>
    </body>

    </html>