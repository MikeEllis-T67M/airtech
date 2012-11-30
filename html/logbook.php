<?php
// Turn on the error reporting
error_reporting(E_ALL);    
//ini_set('display_errors', '1');         

// Library includes
require 'HTML/Template/ITX.php';

// Project includes
require 'include/db_params.inc';
require 'include/logbook.inc';

// Initialise the templates library
$page = new HTML_Template_ITX("templates/");
$page->loadTemplatefile("main.tpl");

print_logbook($page, 'G-BNSR', NULL, NULL);

//$page->parse("__global__");

$page->show();
?>
