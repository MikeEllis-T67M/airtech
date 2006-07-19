<?php
// All scripts should start with this to ensure that the database is
// connected and the user is authenticated
require 'include/db_params.inc';
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title>Aircraft Technical Log System</title>
  <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"/>
  <link rel="shortcut icon" href="images/favicon.ico" type="image/x-icon" />
  <link  rel="stylesheet" type="text/css" media="all" href="css/style.css">
  <link  rel="stylesheet" type="text/css" media="all" href="css/layout.css">

  <script type="text/javascript" src="ruzee/cssquery2-p.js"></script>
  <script type="text/javascript" src="ruzee/ruzeeborders.js"></script>

  <script type="text/javascript"><!--
    // add the border definitions
    RUZEE.Borders.add({
      ".bordered" : { borderType:"shadow", cornerRadius:10, shadowWidth:5 }
    });

    // perform the rendering when the document has been loaded completely
    window.onload=function(){
      RUZEE.Borders.render();
    };
    //-->
  </script>
</head>
<body class="center10">
    <div id="layout_top">
      <div class="bordered">
        <div id="top" class="content">
          <IMG style="float: left " src="images/slingsby_right.png" height=80>
          <IMG style="float: right" src="images/slingsby_left.png"  height=80>
          <H1>The&nbsp;Slingsby&nbsp;SR Redhill&nbsp;Group</H1>
          <P>&copy;&nbsp;2006&nbsp;Mike&nbsp;Ellis and the Slingsby&nbsp;SR Redhill&nbsp;Group
        </div>
      </div>
    </div>

    <div id="layout_left">
      <div class="bordered">
        <div id="navbar" class="content">
          <H1>Technical Log</H1>
          <FORM NAME="date">
            <SELECT NAME="SelectDate" onChange="document.location.href=document.nav.SelectURL.options[document.date.SelectDate.selectedIndex].value">
              <OPTION VALUE="http://www.cs.tut.fi/~jkorpela/forms/jsnav.html" All>All techlog entries</OPTION>
              <OPTION VALUE="">May 2006</OPTION>
              <OPTION VALUE="">June 2006</OPTION>
              <OPTION VALUE="">July 2006</OPTION>
            </SELECT>
          </FORM>
          <H1>Documentation</H1>
          <P><A HREF="http://www.gbnsr.org.uk/checklist.pdf">Aircraft checklist</A>
        </div>
      </div>

      <div class="bordered">
        <div id="links" class="content">
          <H1>External links</H1>
          <P><A HREF="http://www.cubair.co.uk">Cubair Flight Training</A>
          <P><A HREF="http://www.redhillaerodrome.com">Redhill Aerodrome</A>
          <P><A HREF="http://www.met-office.gov.uk/aviation">Weather</A>
          <P><A HREF="http://www.ais.org.uk">NOTAMs</A>
        </div>
      </div>
    </div>

    <div id="layout_right">
      <div class="bordered">
        <div id="main" class="content">
        
<?php
// Get some data from the database
$sql = "SELECT   DATE_FORMAT(Takeoff, '%a, %D %b %Y')      AS TODate,
                 DATE_FORMAT(Landing, '%a, %D %b %Y')      AS LdgDate,
                 DATE_FORMAT(Takeoff, '%H:%i')             AS TOTime,
                 DATE_FORMAT(Landing, '%H:%i')             AS LdgTime,
                 TIME_FORMAT(TIMEDIFF(Landing, Takeoff), '%k:%i') AS FlightTime,
                 StartTach,
                 EndTach,
                 Departure,
                 Arrival,
                 Aircraft,
                 PIC
        FROM     Flights
        ORDER BY Takeoff, Aircraft";

// Execute the query and put results in $result
$result = mysql_query($sql)
or die ('Unable to execute query - '.$sql.' Error is' . mysql_error());

echo "<TABLE CLASS=\"techlog\">";
echo "<TR><TH>Aircraft</TH><TH>Date</TH><TH>Flight times</TH><TH>Duration</TH><TH>Flight</TH><TH>Start Tach</TH><TH>End Tach</TH><TH>Tach time</TH><TH>Pilot</TH></TR>";

$oddeven   = "odd";
$last_date = "";

// Write each flight out one row at a time
while ($flight = mysql_fetch_assoc($result))
{
   // Alternate the row class as the date changes to given alternate-line greying
   if ($flight['TODate'] != $last_date)
   {
      $last_date = $flight['TODate'];
      if ($oddeven == "odd") $oddeven = "even"; else $oddeven = "odd";
   }
   echo "<TR class=\"" . $oddeven . "\">";

   echo "<TD>" . $flight['Aircraft']                    . " </TD>";
   
   // Show the date as a single field if it was a single date, or as a range if
   // the flight spanned midnight
   if ($flight['TODate'] == $flight['LdgDate'])
   {
     echo "<TD>" . $flight['TODate'] . " </TD>";
   }
   else
   {
     echo "<TD>" . $flight['TODate'] . "&ndash;" . $flight['LdgDate'] . " </TD>";
   }
   
   echo "<TD>" . $flight['TOTime'] . "&ndash;" . $flight['LdgTime'] . " </TD>";
   echo "<TD>" . $flight['FlightTime']                        . " </TD>";
   
   if ($flight['Departure'] == $flight['Arrival'])
   {
     echo "<TD>" . $flight['Departure'] . " (local)</TD>";
   }
   else
   {
     echo "<TD>" . $flight['Departure'] . "&ndash;" . $flight['Arrival'] . "</TD>";
   }
   
   echo "<TD>" . number_format($flight['StartTach'], 1) . " </TD>";
   echo "<TD>" . number_format($flight['EndTach'], 1)   . " </TD>";
   echo "<TD>" . number_format($flight['EndTach'] -
                               $flight['StartTach'], 1) . " </TD>";

   echo "<TD>" . $flight['PIC']                         . " </TD>";
   echo "</TR>";
}

echo "</TABLE>";
?>

        </div>
      </div>
    </div>
</body>
</html>
