<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/convert_time.inc';

function print_techlog($page, $db, $aircraft, $month, $chargee)
{ 
  // Get some data from the database
  $sql = "SELECT   DATE_FORMAT(F.Takeoff, '%a, %D %b %Y')               AS TODate,
                   DATE_FORMAT(F.Landing, '%a, %D %b %Y')               AS LdgDate,
                   DATE_FORMAT(F.Takeoff, '%H:%i')                      AS TOTime,
                   DATE_FORMAT(F.Landing, '%H:%i')                      AS LdgTime,
                   TIME_FORMAT(TIMEDIFF(F.Landing, F.Takeoff), '%k:%i') AS FlightTime,
                   FORMAT(F.EndTach - F.StartTach, 1)                   AS EngineTime,
                   FORMAT(F.EndTach, 1)                                 AS EndTach,
                   F.EndAirframeHours,
                   F.Departure,
                   F.Arrival,
                   F.Landings,
                   F.TouchAndGoes,
                   F.PIC,
                   F.Pax,
                   F.Notes,
                   TO_DAYS(C.DateOfNextCheck) - TO_DAYS(F.Takeoff)     AS DaysToCheck,
                   TIME_FORMAT(SEC_TO_TIME(round((C.HoursAtNextCheck - F.EndAirframeHours)*3600)), '%k:%i')
                                                                       AS HoursToCheck,
                   DATE_FORMAT(F.Takeoff, '%Y%c%d%H%i%s')            AS Pkey
          FROM     Flights AS F
          JOIN     CRS     AS C
          ON     (   F.Takeoff BETWEEN C.DateOfCheck AND C.CRSExpiry
                   AND F.Aircraft = C.Aircraft )
          WHERE    F.Aircraft = '" . $aircraft . "'
          AND      C.Item     = 'Airframe'
          ORDER BY Takeoff DESC";

  // Execute the query and put results in $result
  $flight_list = $db->query($sql)
  or die ('Unable to execute query - '.$sql.' Error is' . mysql_error());

  $oddeven   = "odd";
  $last_date = "";

  // Write each flight out one row at a time
  while ($flight = $flight_list->fetch_assoc())
  {
    // Alternate the row class as the date changes to given alternate-line greying
    if ($flight['TODate'] != $last_date)
    {
	
	  // If we've already had some flights then make sure we display them and move on to the next lot
      if ($last_date != "")
      {
		$page->setCurrentBlock('TechLogDate');
        $page->setVariable("date_oddeven", $oddeven);
		$page->setVariable("TODate", $last_date);
		$page->parse('TechLogDate');
      }

	  // Update the date we're currently working on
      $last_date = $flight['TODate'];
	  
	  // Toggle the colour
      if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
    }
    
    // Fudge the data into the correct formats
    
    // Local, or two different airfields?
    if ($flight['Departure'] == $flight['Arrival'])
      $flight['Departure'] .= " (local)";
    else
      $flight['Departure'] .= " &ndash; " . $flight['Arrival'];

    // Format the airframe hours nicely
    $flight['EndAirframeHours'] = format_HMM($flight['EndAirframeHours']);
    
    // Add in any touch-and-goes
    if ($flight['TouchAndGoes'] > 0)
      $flight['Landings'] = $flight['TouchAndGoes'] . " + " . $flight['Landings'];

    // Convert the days to check into a more friendly form
    $flight['DaysToCheck'] = explain_datediff($flight['DaysToCheck']);

    // Was there a passenger?
    if ($flight['Pax'] != "")
      $flight['PIC'] .= "/" . $flight['Pax'];
    
    // Display a row of flight information
    $page->setCurrentBlock('TechLogRow');
    $page->setVariable($flight);
    $page->setVariable("oddeven", $oddeven);
    $page->parseCurrentBlock();
  }

  // The last block of flights won't have been displayed yet, so display them
  $page->setCurrentBlock('TechLogDate');
  $page->setVariable("date_oddeven", $oddeven);
  $page->setVariable("TODate", $last_date);
  $page->parse('TechLogDate');

  // Free up the memory
  $flight_list->free_result();
}
?>
