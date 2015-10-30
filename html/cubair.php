<?php
// Turn on the error reporting
error_reporting(E_ALL);    
ini_set('display_errors', '1');         

// Library includes
require 'HTML/Template/Sigma.php';

// Project includes
require 'include/db_params.inc';
require 'include/cubair.inc';

// Initialise the templates library
$page = new HTML_Template_Sigma("templates/");
$page->loadTemplatefile("main.tpl");

$db = techlog_connect();

print_cubair($page, $db, 'G-BNSR', NULL, NULL);

//$page->parse("__global__");

$page->show();
?>
