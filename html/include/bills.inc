<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated

//require 'include/new_db_params.inc';
require 'include/convert_time.inc';

function print_bill($page, $aircraft, $month, $chargee)
{
  error_reporting(E_ALL);

  // Title the page
  //printf("<H1>%s bill for %s in %s</H1>",
  //       $aircraft, $chargee, $month);

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

  $oddeven    = "odd";
  $total_cost = 0;
  $total_tach = 0;

  // Get the data from the database
  $list_of_flights_sql = "
    SELECT
      DATE_FORMAT(F.Takeoff, '%a %D %b &rsquo;%y')          AS BillTODate,
      DATE_FORMAT(F.Takeoff, '%H:%i')                       AS BillTOTime,
      DATE_FORMAT(F.Landing, '%H:%i')                       AS BillLdgTime,
      TIME_FORMAT(TIMEDIFF(F.Landing, F.Takeoff), '%k:%i')  AS BillFlightTime,
      FORMAT(F.EndTach - F.StartTach, 1)                    AS BillTachTime,
      F.Departure                                           AS BillDeparture,
      F.Arrival                                             AS BillArrival,
      F.PIC                                                 AS BillPIC,
      F.Pax                                                 AS BillPax,
      F.Notes                                               AS BillNotes,
      (R.HourlyFee     * (F.EndTach - F.StartTach) +
       (R.FlyingHourFee * TIME_TO_SEC(TIMEDIFF(F.Landing, F.Takeoff)) / 3600))
                                       * FC.Proportion      AS BillFlightCost,
      (
        SELECT
          SUM(AC.Quantity * AR.Rate * FC.Proportion)
        FROM      AirfieldCharges          AS AC
        LEFT JOIN AirfieldRates            AS AR ON (
          AC.Aircraft       = AR.Aircraft            AND
          AC.Airfield       = AR.Airfield            AND
          AC.Activity       = AR.Activity            AND
          AC.Takeoff  BETWEEN AR.StartDate AND AR.EndDate)
        WHERE
          F.Aircraft   = AC.Aircraft                 AND
          F.Takeoff    = AC.Takeoff
      )                                                     AS BillAirfieldFees
    FROM Flights                       AS F
    JOIN FlightCharges                 AS FC ON (
      F.Takeoff        = FC.Takeoff                  AND
      F.Aircraft       = FC.Aircraft)
    JOIN AircraftRates                 AS R ON (
      FC.ChargeTo      = R.Payer                     AND
      FC.Aircraft      = R.Aircraft                  AND
      FC.Takeoff BETWEEN R.StartDate AND R.EndDate)
    WHERE
      F.Aircraft                      = ?            AND
      ? IN (DATE_FORMAT(F.Takeoff, '%b %y'), 'All')  AND
      ? IN (FC.ChargeTo, 'All')
    ORDER BY F.Takeoff";

  // Prepare the SQL statement
  if ($list_of_flights = $mysqldb->prepare($list_of_flights_sql))
    {
      // Bind parameters to the markers
      $list_of_flights->bind_param("sss", $aircraft, $month, $chargee);

      // Execute the query
      if (!$list_of_flights->execute())
        printf("Execute failed: %s.\n", $mysqldn->error());

      // Process the results one row at a time
      while ($flight = mre_fetch_assoc($list_of_flights))
        {
          // Alternate the row class as the date
          // changes to given alternate-line greying
          if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";

          // Local, or two different airfields?
          if ($flight['BillDeparture'] == $flight['BillArrival'])
            $flight['BillDeparture'] .= " (local)";
          else
            $flight['BillDeparture'] .= " &ndash; " . $flight['BillArrival'];

          // Was there a passenger?
          if ($flight['BillPax'] != "")
            $flight['BillPIC'] .= "/" . $flight['BillPax'];

          // Were there any notes?
          if ($flight['BillNotes'] != "")
            $flight['BillPIC'] .= "<BR/>". $flight['BillNotes'];
            
          // Keep a running total of the cost and tach time
          $total_cost += $flight['BillFlightCost'] + $flight['BillAirfieldFees'];
          $total_tach += $flight['BillTachTime'];

          // Format all the numbers in a nice way
          $flight['BillFlightCost']   = number_format($flight['BillFlightCost'],   2);
          $flight['BillAirfieldFees'] = number_format($flight['BillAirfieldFees'], 2);

          // Display the row
          $page->setCurrentBlock('BillRow');
          $page->setVariable($flight);
          $page->setVariable("bill_oddeven", $oddeven);
          $page->parseCurrentBlock();
        }

      // Print the total cost
      $total_cost = number_format($total_cost, 2);
      $total_tach = number_format($total_tach, 1);

      $page->setCurrentBlock('BillSummary');
      $page->setVariable("BillTotalCost", $total_cost);
      $page->setVariable("BillTotalTach", $total_tach);
      $page->parseCurrentBlock();
      
      $page->parse('Bill');

      // Free up the memory
      $list_of_flights->close();
    }
}

function mre_fetch_assoc(&$stmt)
{
  // Fetch the meta-data about this query
  $meta = $stmt->result_metadata();

  // Not sure what retval is for - seems to be an array which maps to the
  // original statement we passed in - which wasn't an array...
  $retval[] = &$stmt;

  // tmp is used as a temporary store for the results, while names is the list
  // of names for the columns
  $tmp;
  $names;

  // Build up a list of all the field names
  while ($field = $meta->fetch_field())
  {
    $names[] = $field->name;
  }

  // Copy the list of field names... not sure why this is needed
  $tmp = $names;

  // Why is this not a foreach?
  for ($c = 0; $c < count($names); $c++)
  {
    $retval[] = &$tmp[$c]; // Why is this a reference not a copy?
  }

  // Bind each name in turn into the retval array ??? Why can't we bind
  // directly to the associative array just doing it one element at a time?
  call_user_func_array("mysqli_stmt_bind_result", $retval);

  // Fetch the next row of data from the database
  if ($stmt->fetch())
  {
    // Copy each element from the bound "retval" array into the desired associative array
    $assoc_array;
    for ($c = 0; $c < count($names); $c++)
    {
      $assoc_array[$names[$c]] = $retval[$c + 1];
    }

    return $assoc_array;
  }
  else
  {
    // No data returned - flag the end of data condition and bail out
    return FALSE;
  }
}
?>
