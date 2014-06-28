<?php
// Turn on the error reporting
//error_reporting(E_ALL);

// Library includes
require 'HTML/Template/Sigma.php';

// Project includes
require 'include/db_params.inc';
require 'include/bills.inc';

// Initialise the templates library
$page = new HTML_Template_Sigma("templates/");
$page->loadTemplatefile("main.tpl");

$db = techlog_connect();

print_bill($page, $db, 'G-BNSR', $_GET['date'], $_GET['person']);

$page->show();
?>
