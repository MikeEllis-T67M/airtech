<?php

// Connection information for the database
$db_params['host']     = 'localhost';
$db_params['user']     = 'airtech';
$db_params['password'] = '';
$db_params['database'] = 'AircraftTech';

// Connect to MySQL
$mysqldb = new mysqli($db_params['host'],
                      $db_params['user'],
                      $db_params['password'],
                      $db_params['database']);

// Check the connection succeeded
//if (mysqli_connect_errno()) {
//   printf("Connect failed: %s\n", mysqli_connect_error());
//   exit();
//}

?>
