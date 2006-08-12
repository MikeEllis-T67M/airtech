<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/db_params.inc';
require 'include/convert_time.inc';

function print_techlog($aircraft, $month, $chargee)
{
  // Get some data from the database
  $aircraft = "G-BNSR";

  $sql = "SELECT   DATE_FORMAT(Takeoff, '%a, %D %b %Y')             AS TODate,
                   DATE_FORMAT(Landing, '%a, %D %b %Y')             AS LdgDate,
                   DATE_FORMAT(Takeoff, '%H:%i')                    AS TOTime,
                   DATE_FORMAT(Landing, '%H:%i')                    AS LdgTime,
                   TIME_FORMAT(TIMEDIFF(Landing, Takeoff), '%k:%i') AS FlightTime,
                   StartTach,
                   EndTach,
                   EndAirframeHours,
                   Departure,
                   Arrival,
                   Landings,
                   TouchAndGoes,
                   PIC,
                   Pax,
                   Charge1,
                   Charge2,
                   Notes,
                   DATE_FORMAT(Takeoff, '%Y%c%d%H%i%s')            AS Pkey
          FROM     Flights
          WHERE    Aircraft = '" . $aircraft . "' ";

  // Title the page, and add the date restriction to the query if appropriate
  echo "<H1>Aircraft Flight Log for ";
  if ($chargee != null)
  {
    echo $chargee . " in ";
    $sql .= "AND ( Charge1 = '" . $chargee . "' OR Charge2 = '" . $chargee . "') ";
  }

  echo $aircraft;
  
  if ($month != null)
  {
    echo " for " . $month;
    $sql .= "AND DATE_FORMAT(Takeoff, '%b %Y') = '". $month . "' ";
  }
  echo "</H1>\n";

  // Make sure the query returns the data in the correct order
  $sql .= "ORDER BY Takeoff";

  // Execute the query and put results in $result
  $flight_list = mysql_query($sql)
  or die ('Unable to execute query - '.$sql.' Error is' . mysql_error());

  echo "<DIV CLASS=\"techlog\">\n";
  echo "<TABLE CLASS=\"techlog_table\" cellpadding=\"0\" cellspacing=\"0\">\n";
  echo "<THEAD CLASS=\"techlog_fixed\">\n";
  echo "<TR>";
  echo "<TH>Flight details</TH><TH>Flight times</TH><TH>Airframe</TH>";
  echo "<TH>Tach</TH><TH>Cycles</TH><TH>Pilot/Pax</TH><TH>Payment</TH><TH>Notes</TH>";
  echo "</TR>\n";
  echo "</THEAD>\n";
  echo "<TBODY CLASS=\"techlog_scrolling\">\n";
  
  $oddeven   = "odd";
  $last_date = "";

  // Write each flight out one row at a time
  while ($flight = mysql_fetch_assoc($flight_list))
  {
    // Alternate the row class as the date changes to given alternate-line greying
    if ($flight['TODate'] != $last_date)
    {
      $last_date = $flight['TODate'];
      if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
      echo "<TR class=\"" . $oddeven . "\"><TH colspan=8>". $flight['TODate'] . "</TH></TR>\n";
    }
    echo "<TR class=\"" . $oddeven . "\">";

    // Make each row clickable - doesn't work :-(
    // echo "<A HREF=\"flight_details?date=" . $flight['Pkey'] . "&aircraft=" . $aircraft . "\">";

    // Flight details
    if ($flight['Departure'] == $flight['Arrival'])
    {
      echo "<TD>" . $flight['Departure'] . " (local)</TD>";
    }
    else
    {
      echo "<TD>" . $flight['Departure'] . " &ndash; " . $flight['Arrival'] . "</TD>";
    }
    
    // Takeoff and landing times
    echo "<TD>" . $flight['TOTime'] . " &ndash; "
         . $flight['LdgTime'] . "</TD>";

    // Airframe hours
    printf("<TD>%s (%s)</TD>",
           format_HMM($flight['EndAirframeHours']),
           $flight['FlightTime']);
    
    // Engine details
    printf("<TD>%s (%s)</TD>",
           number_format($flight['EndTach'], 1),
           number_format($flight['EndTach'] - $flight['StartTach'], 1));

    // Cycles
    echo "<TD>";
    if ($flight['TouchAndGoes'] > 0) echo $flight['TouchAndGoes'] . "+";
    echo $flight['Landings'] . "</TD>";
    
    // Pilot in command and passenger
    echo "<TD>" . $flight['PIC'];
    if ($flight['Pax'] != "") echo "/" . $flight['Pax'];
    echo "</TD>";
    
    // Charging info
    if ($flight['Charge2'] == null)
    {
      echo "<TD>" . $flight['Charge1'] . " </TD>";
    }
    else
    {
      echo "<TD>" . $flight['Charge1'] . "/" . $flight['Charge2'] . " </TD>";
    }
    
    // Notes
    echo "<TD>" . $flight['Notes'] . " </TD>";

    // End of row
    echo "</TR>\n";
  }
  
  // End of table
  echo "</TBODY>\n";
  echo "</TABLE>\n";
  echo "</DIV>\n";
  
  // Free up the memory
  mysql_free_result($flight_list);
}
?>
