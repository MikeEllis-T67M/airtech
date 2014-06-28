<?php



// Connect to MySQL
function techlog_connect()
{
	// Connection information for the database
	$db_params['host']     = 'localhost';
	$db_params['user']     = 'airtech';
	$db_params['password'] = '';
	$db_params['database'] = 'AircraftTech';

	$db = new mysqli($db_params['host'], 
					 $db_params['user'], 
					 $db_params['password'])
		or die ('Unable to connect to server - ' . mysql_error());

	// Select database on MySQL server
	$db->select_db($db_params['database'])
		or die ('Unable to select database - ' . mysql_error());

	return $db;
	}
?>
