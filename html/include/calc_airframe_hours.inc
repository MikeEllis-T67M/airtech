<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/db_params.inc';
require 'include/convert_time.inc';

function calculate_airframe_hours($aircraft)
{
  // Get some data from the database
  $db = techlog_connect();
  $aircraft = "G-BNSR";

  $sql = "SELECT   Takeoff,
                   TIME_TO_SEC(TIME(Landing)) - TIME_TO_SEC(TIME(Takeoff)) AS FlightTime,
                   TIME_FORMAT(TIMEDIFF(Landing, Takeoff), '%k:%i') AS Duration,
                   EndAirframeHours
          FROM     Flights
          WHERE    Aircraft = '" . $aircraft . "'
          ORDER BY Takeoff";

  // Execute the query and put results in $result
  $all_flights = $db->query($sql);
  
  $afhours    = 0;
  $last_check = 0;
  
  // Process each flight in turn
  while ($flight = $all_flights->fetch_assoc())
  {
    $sql = "SELECT MAX(DateOfCheck) AS CRS_Date
            FROM   CRS
            WHERE  DateOfCheck < '" . $flight['Takeoff'] . "'
            AND    Aircraft = '" . $aircraft . "'";
             
    $result   = $db->query($sql);
    $crs_date = $result->fetch_field();
    
    // Find the right entry in the CRS table
    $sql = "SELECT HoursAtCheck, DateOfCheck
            FROM   CRS
            WHERE  DateOfCheck = '" . $crs_date . "'
            AND    Aircraft = '" . $aircraft . "'";
             
    $result = $db->query($sql);

    $crs_details =$result->fetch_assoc();

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

      $db->query($sql);
    }
    else
    {
      // Airframe hours in database are "incorrect"
      printf("<P>%s + %s calculated airframe hours %s but database says %s\n",
             $flight['Takeoff'],
             $flight['Duration'],
             format_HMM($afhours),
             format_HMM($flight['EndAirframeHours']));
             
      // Continue counting hours based on the value in the database
      $afhours = $flight['EndAirframeHours'];
      
    }
  }
}
?>
