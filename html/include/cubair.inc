<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/convert_time.inc';

function print_cubair($page, $db, $aircraft, $month, $chargee)
{ 
  // Get some data from the database
  $sql = "SELECT   Mon           AS Month,
		           RedhillFuel   AS RedhillFuel,
				   OtherFuel     AS OtherFuel,
				   Oil           AS Oil,
				   CubairFlights AS CubairFlights,
				   CubairUse     AS Cubair,
				   GroupFlights  AS GrpFlights,
				   GroupUse      AS Grp,
				   URL           AS URL
          FROM     CubairMonthlySummary";
		  
  // Execute the query and put results in $result
  $flight_list = $db->query($sql)
	or die ('Unable to execute query - '.$sql.' Error is' . mysql_error());

  $oddeven   = "even";

  // Write each flight out one row at a time
  while ($logrow = $flight_list->fetch_assoc())
  {
	// Toggle the colour
    if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";

	// Modify zero values into dashes
	if ($logrow{'RedhillFuel'} == '0') $logrow{'RedhillFuel'} = '-';
	if ($logrow{'OtherFuel'}   == '0') $logrow{'OtherFuel'}   = '-';
	if ($logrow{'Oil'}         == '0') $logrow{'Oil'}         = '-';

    // Display a row of row of log information
    $page->setCurrentBlock('CubairLogRow');
    $page->setVariable($logrow);
    $page->setVariable("oddeven", $oddeven);
    $page->parseCurrentBlock();
  }

  // Free up the memory
  $flight_list->free_result();
}
?>
