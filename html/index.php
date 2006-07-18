<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title>Aircraft Technical Log System</title>
  <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"/>
  <link rel="shortcut icon" href="images/favicon.ico" type="image/x-icon" />
  <style type="text/css" media="all">
    @import url("css/style.css");
    @import url("css/layout.css");
  </style>

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
<body class="center90">
    <div id="layout_top">
      <div class="bordered">
        <div id="top" class="content">
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
          <P><A HREF="http://www.gbnsr.org.uk/docs/checklist.pdf">Aircraft checklist</A>
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
          <H1>Main page area</H1>
          <P>This will normally have quite a large amount of content in it, primarily derived by extracting information from the SQL database.
        </div>
      </div>
    </div>
</body>
</html>
