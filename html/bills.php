<?php
// Turn on the error reporting
//error_reporting(E_ALL);

// Library includes
require 'HTML/Template/ITX.php';

// Project includes
require 'include/bills.inc';

// Initialise the templates library
$page = new HTML_Template_ITX("templates/");
$page->loadTemplatefile("main.tpl");

print_bill($page, 'G-BNSR', $_GET['date'], $_GET['person']);

$page->show();
?>
