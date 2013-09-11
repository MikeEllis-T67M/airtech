<?php
// Turn on the error reporting
error_reporting(E_ALL);             

// Library includes
require 'HTML/Template/ITX.php';
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1) ;

// Project includes
require 'include/db_params.inc';
require 'include/techlog.inc';

// Initialise the templates library
$page = new HTML_Template_ITX("templates/");
$page->loadTemplatefile("main.tpl");

print_techlog($page, 'G-BNSR', NULL, NULL);

$page->show();
?>
