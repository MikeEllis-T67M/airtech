<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/convert_time.inc';

function print_logbook($page, $db, $aircraft, $month, $chargee)
{
    // Get some data from the database
    $sql = "SELECT   DateF, Flights, FlightTime, Airframe, Engine, Prop, HoursToRun, Oil 
	        FROM     LogbookFormattedWithHoursToRun;";

    // Execute the query and put results in $result
    $entry_list = $db->query($sql)
    or die ('Unable to execute query - '.$sql.' Error is' . mysql_error());

    $oddeven   = "even";

    // Write each flight out one row at a time
    while ($entry = $entry_list->fetch_assoc())
    {
        if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
        
        // Display a row of flight information
        $page->setCurrentBlock('LogbookRow');
        $page->setVariable($entry);
        $page->setVariable("oddeven", $oddeven);
        $page->parseCurrentBlock();
	}

    // Free up the memory
    $entry_list->free_result();
}
?>
