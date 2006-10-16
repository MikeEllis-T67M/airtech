<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated

//require 'include/new_db_params.inc';
require 'include/convert_time.inc';

function display_bill($aircraft, $month, $chargee)
{
  // Title the page
  printf("<H1>%s flight record for %s in %s</H1>",
         $aircraft, $chargee, $month);

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
  echo "<TH>Date</TH>";
  echo "<TH>Takeoff</TH>";
  echo "<TH>Landing</TH>";
  echo "<TH>Flight time</TH>";
  echo "<TH>Tach time</TH>";
  echo "<TH>PIC / Pax</TH>";
  echo "<TH>Notes</TH>";
  echo "<TH>Proportion</TH>";
  echo "</TR>\n";
  echo "</THEAD>\n";
  echo "<TBODY CLASS=\"techlog_scrolling\">\n";

  $oddeven   = "odd";
  $last_date = "";

  // Get the data from the database
  $list_of_flights_sql = "
    SELECT
      DATE_FORMAT(F.Takeoff, '%a, %D %b %Y')               AS TODate,
      DATE_FORMAT(F.Takeoff, '%H:%i')                      AS TOTime,
      DATE_FORMAT(F.Landing, '%H:%i')                      AS LdgTime,
      TIME_FORMAT(TIMEDIFF(F.Landing, F.Takeoff), '%k:%i') AS FlightTime,
      F.EndTach - F.StartTach                              AS TachTime,
      F.Departure,
      F.Arrival,
      F.PIC,
      F.Pax,
      F.Notes,
      FC.Proportion
    FROM
      Flights AS F
    JOIN
      FlightCharges as FC ON (
        F.Takeoff = FC.Takeoff              AND
        F.Aircraft = FC.Aircraft)
    WHERE
      F.Aircraft                      = ?   AND
      DATE_FORMAT(F.Takeoff, '%b %y') = ?   AND
      FC.ChargeTo                     = ?
    ORDER BY F.Takeoff";
 
  // Prepare the SQL statement
  if ($list_of_flights = $mysqldb->prepare($list_of_flights_sql))
  {
    // Bind parameters to the markers
    $list_of_flights->bind_param("sss", $aircraft, $month, $chargee);

    // Execute the query
    if (!$list_of_flights->execute()) {
      printf("Execute failed: %s.\n", $mysqldn->error());
    }

    // Tell PHP	where to put the results
    if (!$list_of_flights->bind_result($TODate,
                                       $TOTime,
                                       $LdgTime,
                                       $FlightTime,
                                       $TachTime,
                                       $Departure,
                                       $Arrival,
                                       $PIC,
                                       $Pax,
                                       $Notes,
                                       $Proportion)) {
      printf("Result binding failed: %s.\n", $mysqldb->error());
    }

    // Process the results one row at a time
    while ($list_of_flights->fetch())
      {
        // Alternate the row class as the date
        // changes to given alternate-line greying
        if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
        printf("<TR class=\"%s\">", $oddeven);

        // Date
        printf("<TD>%s</TD>", $TODate);

        // Flight details
        if ($Departure == $Arrival) {
          printf("<TD>%s (local)</TD>", $Departure);
        } else {
          printf("<TD>%s &ndash; %s</TD>",
                 $Departure, $Arrival);
        }
    
        // Takeoff and landing times
        printf("<TD>%s &ndash; %s</TD>",
               $TOTime, $LdgTime);

        // Flight time
        printf("<TD>%s</TD>", $FlightTime);

        // Tach time
        printf("<TD>%s</TD>", $TachTime);

        // Pilot in command and passenger
        if ($Pax != "") {
          printf("<TD>%s / %s</TD>", $PIC, $Pax);
        } else {
          printf("<TD>%s", $PIC);
        }
        
        // Notes
        printf("<TD>%s</TD>", $Notes);

        // Charge proportion
        printf("<TD>%4.1f%%</TD>", $Proportion * 100.0);
        
        // End of row
        echo "</TR>\n";
      }

    if ($mysqldb->errno()) {
      printf("Fetch failed: %s\n", $mysqldb->error());
      exit();
    }

    // End of table
    echo "</TBODY>\n";
    echo "</TABLE>\n";
    echo "</DIV>\n";

    // Free up the memory
    $list_of_flights->close();
  }
}
?>
