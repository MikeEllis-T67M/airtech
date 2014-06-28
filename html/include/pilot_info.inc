<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated

function print_pilot_info() {
  // Title the page
  printf("<H1>Pilot information</H1>");

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
  // Check connection
  if (mysqli_connect_errno()) {
     printf("Connect failed: %s\n", mysqli_connect_error());
     exit();
  }

  // Begin the table - do the headers etc
  echo "<DIV CLASS=\"techlog\">\n";
  echo "<TABLE CLASS=\"techlog_table\" cellpadding=\"0\" cellspacing=\"0\">\n";
  echo "<THEAD CLASS=\"techlog_fixed\">\n";
  echo "<TR>";
  echo "<TH>Aircraft</TH>";
  echo "<TH>Pilot</TH>";
  echo "<TH>Date range</TH>";
  echo "<TH># flights</TH>";
  echo "<TH>Total Time</TH>";
  echo "<TH>Fuel burn</TH>";
  echo "<TH>Tach hour ratio</TH>";
  echo "</TR>\n";
  echo "</THEAD>\n";
  echo "<TBODY CLASS=\"techlog_scrolling\">\n";

  $oddeven   = "odd";

  // Get the data from the database
  $pilot_info_sql = "
    SELECT
        PIC                                                 as Pilot,
        Aircraft                                            as Aircraft,
        count(*)                                            as NumFlights,
        TIME_FORMAT(SEC_TO_TIME(sum(TIME_TO_SEC(TIMEDIFF(Landing, Takeoff)))), '%H:%i') as TotalTime,
        DATE_FORMAT(min(takeoff), '%D %b %Y')               as FirstFlight,
        DATE_FORMAT(max(landing), '%D %b %Y')               as LastFlight,
        avg((StartFuel - EndFuel) / (EndTach - StartTach))  as FuelBurn,
        avg((EndTach - StartTach) / TIME_TO_SEC(TIMEDIFF(Landing, Takeoff)) * 3600)   as TachRatio
    FROM
        Flights AS F
    GROUP BY Aircraft, Pilot
    ORDER BY max(landing) DESC";

  // Prepare the SQL statement
  if ($pilot_info = $mysqldb->prepare($pilot_info_sql)) {
    // Bind parameters to the markers - none in this case
    // $pilot_info->bind_param("sss", $aircraft, $month, $chargee);

    // Execute the query
    if (!$pilot_info->execute()) {
      printf("Execute failed: %s.\n", $mysqldn->error());
    }

    // Tell PHP where to put the results
    if (!$pilot_info->bind_result($pilot, $aircraft, $flights, $totaltime, $first, $last, $fuel, $tach)) {
      printf("Result binding failed: %s.\n", $mysqldb->error());
    }

    // Process the results one row at a time
    while ($pilot_info->fetch()) {
        // Alternate the row class as the date
        // changes to given alternate-line greying
        if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
        printf("<TR class=\"%s\">", $oddeven);

        // Aircraft
        printf("<TD>%s</TD>", $aircraft);

        // Pilot
        printf("<TD>%s</TD>", $pilot);

        // Date range
        printf("<TD>%s &ndash; %s</TD>", $first, $last);

        // # flights
        printf("<TD>%s</TD>", $flights);

        // Total time
        printf("<TD>%s</TD>", $totaltime);

        // Average fuel burn
        printf("<TD>%.1f</TD>", $fuel);

        // Tach hour ratio
        printf("<TD>%.2f</TD>", $tach);

        // End of row
        echo "</TR>\n";
    }
    
    // End of table
    echo "</TBODY>\n";
    echo "</TABLE>\n";
    echo "</DIV>\n";

    // Free up the memory
    $pilot_info->close();
  }
}
?>

