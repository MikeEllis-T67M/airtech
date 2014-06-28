<?php
// Turn on the error reporting
error_reporting(E_ALL);             

// Library includes
require 'HTML/Template/Sigma.php';
//ini_set('display_errors', 1);

// Project includes
require 'include/db_params.inc';
require 'include/techlog.inc';

// Initialise the templates library
$page = new HTML_Template_Sigma("templates/");
$page->loadTemplatefile("main.tpl");

// Connect to the database
$db = techlog_connect();

// Print the techlog page itself
print_techlog($page, $db, 'G-BNSR', NULL, NULL);

// Render the page
$page->show();

$db->close();
?>
