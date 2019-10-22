<?php
    $url=$_SERVER['REQUEST_URI'];
    header("Refresh: 5; URL=$url"); // Refresh the webpage every 5 seconds
    ?>
    <html>
    <head>
    <title>Arduino Ethernet Database</title>
<div class="izq">
<link rel="stylesheet" href="style.css">

    
    </head>
    <body>
    <h1>Arduino Data Logging to Database</h1>
    <table border="0" cellspacing="0" cellpadding="4">
    <tr>
    <td class="table_titles">ID</td>
    <td class="table_titles">Date and Time</td>
    <td class="table_titles">Temperature</td>
    <td class="table_titles">Humidity</td>
    <td class="table_titles">Heat_index</td>
    </tr>
    <?php
    include('connection.php');
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
    <div class="der">
    <script src="functions.js"></script>
    test
    <div id="contenedor">
      <!-- <img src="https://image.freepik.com/vector-gratis/vector-plantilla-puerta-abierta_23-2147495012.jpg"> -->
      <h2>Sistema de seguridad </h2>
      <div id="contador"></div>
      <h4><i> </i></h4>
      </div>
    </div>
    </body>
    </html>