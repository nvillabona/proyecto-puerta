
<?php
        $url=$_SERVER['REQUEST_URI'];
        header("Refresh: 0.9; URL=$url"); // Refresh the webpage every second
        ?>
    <html>

    <head>
        <title>Arduino Ethernet Database</title>
        <!-- id izq                       ----------------------------------------------------- -->
        <div class="izq">
            <link rel="stylesheet" href="stylex.css">
            

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



            
            <h2>Sistema de seguridad </h2>
           
            <h4><i> </i></h4>
            <?php
          include('connection.php');
          date_default_timezone_set('America/Bogota');
          $date_fromDB = mysqli_query($con,'SELECT * FROM data ORDER by event DESC  '); //LIMIT 1
          
          
          $fila = mysqli_fetch_array($date_fromDB);
          $date= new DateTime($fila['event']);
          $current_time = new DateTime('now');
        
          
          $date->add(new DateInterval('PT6S')); // adds 6 secs
        if($date > $current_time){
            echo "<img src='https://image.flaticon.com/icons/png/512/61/61355.png' style='width: 100px ; height: 100px'>";
            echo "<h1> La puerta está abierta </h1>";
            $intervalo = $date->diff($current_time);
            echo $intervalo->format('La puerta se cerrará en %H horas %i minutos %s segundos');
        }else{
            echo "<img src='https://image.flaticon.com/icons/png/512/61/61457.png' style='width: 100px ; height: 100px'>";
            $intervalo = $date->diff($current_time);
            echo "<h1> La puerta está Cerrada </h1>";
            echo $intervalo->format('La puerta se abrió hace %H horas %i minutos %s segundos');
        }

     
        ?>

        </div>
    </body>

    </html>