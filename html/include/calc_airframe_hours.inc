<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/db_params.inc';
require 'include/convert_time.inc';

function calculate_airframe_hours($aircraft)
{
  // Get some data from the database
  $aircraft = "G-BNSR";

  $sql = "SELECT   Takeoff,
                   TIME_TO_SEC(TIME(Landing)) - TIME_TO_SEC(TIME(Takeoff)) AS FlightTime,
                   TIME_FORMAT(TIMEDIFF(Landing, Takeoff), '%k:%i') AS Duration,
                   EndAirframeHours
          FROM     Flights
          WHERE    Aircraft = '" . $aircraft . "'
          ORDER BY Takeoff";

  // Execute the query and put results in $result
  $all_flights = mysql_query($sql)
      or die ('Unable to execute query '.$sql.'. Error was: ' . mysql_error());

  $afhours    = 0;
  $last_check = 0;
  
  // Process each flight in turn
  while ($flight = mysql_fetch_assoc($all_flights))
  {
    $sql = "SELECT MAX(DateOfCheck) AS CRS_Date
            FROM   CRS
            WHERE  DateOfCheck < '" . $flight['Takeoff'] . "'
            AND    Aircraft = '" . $aircraft . "'";
             
    $result   = mysql_query($sql)
      or die ('Unable to execute query '.$sql.'. Error was: ' . mysql_error());

    $crs_date = mysql_result($result, 0)
      or die ('Unable to get results from '.$sql.'. Error was: ' . mysql_error());

    mysql_free_result($result);
    
    // Find the right entry in the CRS table
    $sql = "SELECT HoursAtCheck, DateOfCheck
            FROM   CRS
            WHERE  DateOfCheck = '" . $crs_date . "'
            AND    Aircraft = '" . $aircraft . "'";
             
    $result         = mysql_query($sql)
      or die ('Unable to execute query '.$sql.'. Error was: ' . mysql_error());

    $crs_details = mysql_fetch_assoc($result)
      or die ('Unable to get results from '.$sql.'. Error was: ' . mysql_error());
      
    mysql_free_result($result);

    // Handle the initial condition
    if ($last_check == 0)
    {
      $last_check = $crs_details['HoursAtCheck'];
      $afhours    = $crs_details['HoursAtCheck'];
      
      printf("<P>Aircraft check at %s gives airframe hours as %s\n",
             $crs_details['DateOfCheck'],
             format_HMM($crs_details['HoursAtCheck']));
    }
    
    // Check to see if a CRS has been issued since the last flight
    if ($last_check != $crs_details['HoursAtCheck'])
    {
      $last_check = $crs_details['HoursAtCheck'];
      printf("<P>Aircraft check at %s gives airframe hours of %s\n",
             $crs_details['DateOfCheck'],
             format_HMM($crs_details['HoursAtCheck']));
      
      // Yes a check has been done - now do the calculated hours match...
      if (format_HMM($afhours) != format_HMM($crs_details['HoursAtCheck']))
      {
        // No - we've spotted an error here
        printf("<P>Aircraft hours calculated as %s - CRS error detected.\n",
               format_HMM($afhours));
               
        $afhours    = $last_check;
      }
    }

    // Add the flight on to the cummulative total
    $afhours    += $flight['FlightTime'] / 3600;
    
    if (format_HMM($afhours) == format_HMM($flight['EndAirframeHours']))
    {
      // Airframe hours already in database match the calculate figure
      printf("<P>%s + %s correctly shows airframe hours %s\n",
             $flight['Takeoff'],
             $flight['Duration'],
             format_HMM($afhours));
    }
    else if ($flight['EndAirframeHours'] == 0)
    {
      // Airframe hours not in database - add them
      printf("<P>%s + %s has airframe hours %s\n",
             $flight['Takeoff'],
             $flight['Duration'],
             format_HMM($afhours));
           
      $sql    = "UPDATE Flights SET EndAirframeHours = ".$afhours." ".
                "WHERE Takeoff = '".$flight['Takeoff']."' ".
                "AND Aircraft = '".$aircraft."'";
      $update = mysql_query($sql)
        or die ('Unable to execute query '.$sql.'. Error was: ' . mysql_error());
    }
    else
    {
      // Airframe hours in database are "incorrect"
      printf("<P>%s + %s calculated airframe hours %s but database says %s\n",
             $flight['Takeoff'],
             $flight['Duration'],
             format_HMM($afhours),
             format_HMM($flight['EndAirframeHours']));
    }
  }
  
  mysql_free_result($all_flights);
}
?>
