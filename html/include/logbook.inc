<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/db_params.inc';
require 'include/convert_time.inc';

function print_logbook($page, $aircraft, $month, $chargee)
{
    // Get some data from the database
    $sql = "SELECT   DateF, Flights, FlightTime, Airframe, Engine, Prop, HoursToRun 
	        FROM     LogbookFormatted
			WHERE    DATE_SUB(CURDATE(),INTERVAL 90 DAY) <= Date;";

    // Execute the query and put results in $result
    $entry_list = mysql_query($sql)
    or die ('Unable to execute query - '.$sql.' Error is' . mysql_error());

    $oddeven   = "even";

    // Write each flight out one row at a time
    while ($entry = mysql_fetch_assoc($entry_list))
    {
        if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
        
        // Display a row of flight information
        $page->setCurrentBlock('LogbookRow');
        $page->setVariable($entry);
        $page->setVariable("oddeven", $oddeven);
        $page->parseCurrentBlock();
	}

    // Free up the memory
    mysql_free_result($entry_list);
}
?>
