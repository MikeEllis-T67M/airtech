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
  <link rel="stylesheet" type="text/css" media="all" href="css/layout.css">
  <link rel="stylesheet" type="text/css" media="all" href="css/style.css">
  <link rel="stylesheet" type="text/css" media="screen" href="css/techlogtable.css">
  <link rel="stylesheet" type="text/css" media="print" href="css/techlogtable_print.css">
  <link rel="stylesheet" type="text/css" media="print" href="css/print.css">

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
          <H2>Technical Log</H2>
          <FORM NAME="date">
            <SELECT NAME="SelectDate" onChange="document.location.href=document.nav.SelectURL.options[document.date.SelectDate.selectedIndex].value">
              <OPTION VALUE="http://www.cs.tut.fi/~jkorpela/forms/jsnav.html" All>All techlog entries</OPTION>
              <OPTION VALUE="">May 2006</OPTION>
              <OPTION VALUE="">June 2006</OPTION>
              <OPTION VALUE="">July 2006</OPTION>
            </SELECT>
          </FORM>
          <H2>Documentation</H2>
          <P><A HREF="http://www.gbnsr.org.uk/checklist.pdf">Aircraft checklist</A>
        </div>
      </div>

      <div class="bordered">
        <div id="links" class="content">
          <H2>External links</H2>
          <P><A HREF="http://www.cubair.demon.co.uk/pages/members/cubair_online_scheduling.htm">Online booking system</A>
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
  require 'include/bills.inc';
  display_bill("G-BNSR", $_GET[date], $_GET[person]);
?>

        </div>
      </div>
    </div>
</body>
</html>
