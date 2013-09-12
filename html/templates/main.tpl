<?xml version="1.0" encoding="utf-8"?>
<!-- ... and keep IE7 in quirks mode -->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<HTML xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<HEAD>
  <TITLE>Aircraft Technical Log System</TITLE>
  <META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"/>
  <LINK rel="shortcut icon" href="images/favicon.ico" type="image/x-icon" />
  <LINK rel="stylesheet" type="text/css" media="all" href="css/layout.css" />
  <LINK rel="stylesheet" type="text/css" media="all" href="css/style.css" />
  <LINK rel="stylesheet" type="text/css" media="screen" href="css/techlogtable.css" />
  <LINK rel="stylesheet" type="text/css" media="screen" href="css/flightentryformtable.css" />
  <LINK rel="stylesheet" type="text/css" media="print" href="css/techlogtable_print.css" />
  <LINK rel="stylesheet" type="text/css" media="print" href="css/print.css" />

  <SCRIPT type="text/javascript" src="scripts/dateSel.js"></SCRIPT>
</HEAD>

<BODY>
  <DIV id="layout_top">
    <DIV class="bordered">
      <DIV id="top" class="content">
        <IMG style="float: left " src="images/slingsby_right.png" height=60 alt="Slingsby Firefly" />
        <IMG style="float: right" src="images/slingsby_left.png"  height=60 alt="Slingsby Firefly" />
        <H1>The&nbsp;Slingsby&nbsp;SR Redhill&nbsp;Group</H1>
        <P>&copy;&nbsp;2006-2012&nbsp;Mike&nbsp;Ellis and the Slingsby&nbsp;SR Redhill&nbsp;Group
      </DIV>
    </DIV>
  </DIV>

<DIV id="layout_main">
  <DIV id="layout_right">
    <DIV class="bordered">
      <DIV id="main">
        <!-- BEGIN Errors -->
          <DIV CLASS="errors">
            <P>The following errors were detected:-
            <UL>
            <!-- BEGIN OneError -->
              <LI>{ErrorText}</LI>
            <!-- END OneError -->
            </UL>
          </DIV>
        <!-- END Errors -->
        <!-- BEGIN TechLogTable -->
          <DIV CLASS="techlog">
          <TABLE CLASS="techlog_table" cellpadding="0" cellspacing="0">
            <THEAD CLASS="techlog_fixed">
              <TR>
                <TH>Detail</TH>
                <TH>Time</TH>
                <TH>Airframe</TH>
                <TH>Tach</TH>
                <TH>Cycles</TH>
                <TH>Hours / Days<BR>to check</TH>
                <TH>Pilot / Pax<BR>Notes</TH>
              </TR>
            </THEAD>
            <TBODY CLASS="techlog_scrolling">
              <!-- BEGIN TechLogDate -->
              <TR class="{date_oddeven}">
                <TH colspan=7><H2>{TODate}</H2></TH>
              </TR>
              <!-- BEGIN TechLogRow -->
			  <TR class="{oddeven}">
                <TD>{Departure}</TD>
                <TD>{TOTime} &ndash; {LdgTime}</TD>
                <TD>{EndAirframeHours} ({FlightTime})</TD>
                <TD>{EndTach} ({EngineTime})</TD>
                <TD>{Landings}</TD>
                <TD>{HoursToCheck} / {DaysToCheck}</TD>
                <TD>{PIC}<BR>{Notes}</TD>
              </TR>
              <!-- END TechLogRow -->
			  <!-- END TechLogDate -->
            </TBODY>
          </TABLE>
          </DIV>
        <!-- END TechLogTable -->
        <!-- BEGIN LogbookTable -->
          <DIV CLASS="techlog">
          <TABLE CLASS="techlog_table" cellpadding="0" cellspacing="0">
            <THEAD CLASS="techlog_fixed">
              <TR>
                <TH>Date</TH>
                <TH>Flights</TH>
                <TH>Duration</TH>
                <TH>Airframe</TH>
                <TH>Engine</TH>
                <TH>Prop</TH>
                <TH>Hours to check</TH>
              </TR>
            </THEAD>
            <TBODY CLASS="techlog_scrolling">
              <!-- BEGIN LogbookRow -->
              <TR class="{oddeven}">
                <TD>{DateF}</TD>
                <TD>{Flights}</TD>
                <TD>{FlightTime}</TD>
                <TD>{Airframe}</TD>
                <TD>{Engine}</TD>
                <TD>{Prop}</TD>
                <TD>{HoursToRun}</TD>
              </TR>
              <!-- END LogbookRow -->
            </TBODY>
          </TABLE>
          </DIV>
        <!-- END LogbookTable -->
        <!-- BEGIN Bill -->
          <DIV CLASS="techlog">
          <TABLE CLASS="techlog_table" cellpadding="0" cellspacing="0">
            <THEAD CLASS="techlog_fixed">
              <TR>
                <TH>Date</TH>
                <TH>Detail</TH>
                <TH>Time</TH>
                <TH>Tach (flight)<BR/>time</TH>
                <TH>PIC / Pax<BR/>Notes</TH>
                <TH>Cost<BR>Plane + Airfield</TH>
              </TR>
            </THEAD>
            <TBODY CLASS="techlog_scrolling">
              <!-- BEGIN BillRow -->
              <TR class="{bill_oddeven}">
                <TD>{BillTODate}</TD>
                <TD>{BillDeparture}</TD>
                <TD>{BillTOTime} &ndash; {BillLdgTime}</TD>
                <TD>{BillTachTime} ({BillFlightTime})</TD>
                <TD>{BillPIC}</TD>
                <TD>&pound;{BillFlightCost} + &pound;{BillAirfieldFees}</TD>
              </TR>
              <!-- END BillRow -->
              <!-- BEGIN BillSummary -->
              <TR class="total">
                <TD>Totals</TD>
                <TD COLSPAN=2></TD>
                <TD>{BillTotalTach}</TD>
                <TD></TD>
                <TD>&pound;{BillTotalCost}</TD>
              </TR>
              <!-- END BillSummary -->
            </TBODY>
          </TABLE>
          </DIV>
        <!-- END Bill -->
        <!-- BEGIN FlightEntryForm -->
        <!-- END FlightEntryForm -->
        <!-- BEGIN BillGrid -->
          <DIV CLASS="techlog">
          <TABLE CLASS="techlog_table" cellpadding="0" cellspacing="0">
            <THEAD CLASS="techlog_fixed">
              <TR>
                <TH>Month</TH>
                <TH>Person</TH>
                <TH>Flights</TH>
                <TH>Total tach</TH>
                <TH>Cost</TH>
              </TR>
            </THEAD>
            <TBODY CLASS="techlog_scrolling">
              <!-- BEGIN BillGridRow -->
              <TR class="{billgrid_oddeven}">
                <TD>{BillGridmonth}</TD>
                <TD>{BillGridPerson}</TD>
                <TD>{BillGridFlights}</TD>
                <TD>{BillGridTach}</TD>
                <TD>{BillGridCost</TD>
              </TR>
              <!-- END BillGridRow -->
            </TBODY>
          </TABLE>
          </DIV>
        <!-- END BillGrid -->
      </DIV>
    </DIV>
  </DIV>
</DIV>

  <DIV id="layout_left">
    <DIV class="bordered">
      <DIV id="navbar" class="content">
        <H2>Aircraft info</H2>
          <P><A HREF="">Technical log</A>
        <H2>Documentation</H2>
          <P><A HREF="http://www.gbnsr.org.uk/docs/checklist.pdf">Aircraft checklist</A>
          <P><A HREF="http://www.gbnsr.org.uk/docs/FlightLogSheet.pdf">Flight record sheet</A>
        <H2>External links</H2>
          <P><A HREF="http://www.cubair.demon.co.uk/pages/members/cubair_online_scheduling.htm">Online booking system</A>
          <P><A HREF="http://www.cubair.co.uk">Cubair Flight Training</A>
          <P><A HREF="http://www.redhillaerodrome.com">Redhill Aerodrome</A>
          <P><A HREF="http://www.skydemonlight.com">SkyDemon Light</A>
          <P><A HREF="http://www.met-office.gov.uk/aviation">Weather</A>
          <P><A HREF="http://www.ais.org.uk">NOTAMs</A>
          <P><A HREF="http://www.bustard.force9.co.uk">G-BUUI group</A>
          <P><A HREF="http://www.caa.co.uk/ginfo">G-INFO database</A>
      </DIV>
    </DIV>
  </DIV>
</BODY>
</HTML>
