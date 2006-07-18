<?php

//---------------------------------------------------------------  SQL-itize --
function SQLitize($orig)
{
    return ereg_replace("[ -;']", "", $orig);
}

//------------------------------------------------------------- ClearSession --
function ClearSession()
{
    unset($_SESSION['airtech_username']);
    unset($_SESSION['airtech_logout']);
}

//---------------------------------------------- AuthenticateAgainstDatabase --
function AuthenticateAgainstDatabase($user_id, $password)
{
    // We were passed a user ID and password -
    // look them up in the database to see if they're valid

    // Don't yet know the user's name - so set to null
    $userdata = false;
    
    // Some characters aren't legal in SQL queries - so remove them
    $user_id = SQLitize($user_id);

        
    // Formulate the query
    $sql = "SELECT user_name, user_pw_change FROM users WHERE
            user_id       = '$user_id' ";

    if ($password == "")
    {
        // NULL password submitted - must be null in the database too
        $sql .= "and user_password = ''";
    }            
    else
    {
        // The password is encrypted, so encrypt it here. To ensure the same salt
        // is used each time, use the un-encrypted password as the salt -
        // this may not be secure with all encryption types, but should be
        // good enough.
        $password = crypt($password, $password);
        $sql .= "AND user_password = '$password'";
    }
    
    // Execute the query and put results in $result
    $result = mysql_query($sql)
        or die ('Unable to execute query - ' . mysql_error());
        
    // Check the number of rows in $result.
    if (mysql_numrows($result) == 1)
    {
        // A single matching row was found - the user is authenticated
        $userdata = mysql_fetch_assoc($result);
    }
    
    return $userdata;
}

//------------------------------------------------------------- Authenticate --
//
// Sets $user_id up to include the current valid username
// This may be achieved using a 401-HTTP-Not-Authorized, or using
// session tracking

session_start();
header("Cache-control: private"); // Bugfix for MSIE6

// Login sessions expire after 900 seconds (for testing)
$login_timeout = 900;

if (isset($_SESSION['airtech_logout']))
{
    // Logout requested?  Unset the session username then
    ClearSession();
}
else if (isset($_SESSION['airtech_username']) &&
         isset($_SERVER['PHP_AUTH_USER']))
{
    // User already authenticated once - we have a valid username in the 
    // session - just need to check for timeout
    if (time() - $_SESSION['last_active'] > $login_timeout)
    {
        // Session has timed out
        ClearSession();
    }
    else
   {
        // Session still valid
        $userdata["user_name"]      = $_SESSION['chbs_username'];
        $userdata["user_pw_change"] = $_SESSION['chbs_force_pwd_chg'];
    }

}
else if (isset($_SERVER['PHP_AUTH_USER']) &&
         isset($_SERVER['PHP_AUTH_PW'])      )
{
    // No session username, but got an HTTP-401 username/password 
    // so check it against the database
    $userdata = AuthenticateAgainstDatabase($_SERVER['PHP_AUTH_USER'],
                                            $_SERVER['PHP_AUTH_PW']);
}

// Ensure the username is valid, and request one if it isn't
if ($userdata == false)
{
    // Send an HTTP-401 to request a username/password
    header('WWW-Authenticate: Basic realm="Cubair Hangar Booking System"');
    header('HTTP/1.0 401 Unauthorized');
    die("<P>Sorry - you are not authorized to access this website.");
}
else
{
    // Add the username to the session
    $_SESSION['airtech_username']      = $userdata["user_name"];
    $_SESSION['last_active']           = time();
    $_SESSION['airtech_force_pwd_chg'] = $userdata["user_pw_change"];
    
    // Have authenticated, so now update user_last_login
    $sql="UPDATE users SET user_last_login = CURRENT_TIMESTAMP()
            WHERE user_id = '".$_SERVER['PHP_AUTH_USER']."'";
    
    // Update 
    $result = mysql_query($sql)
        or die ('Unable to update users table - ' . mysql_error());
}

?>
