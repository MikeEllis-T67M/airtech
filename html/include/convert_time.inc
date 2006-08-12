<?php

function format_HMM($decimal_time)
{
    // Airframe hours
    $hours = floor($decimal_time);
    $mins  = round(($decimal_time - $hours) * 60, 0);
    
    // Correct the error caused by 49.9999999 -type barstorial case
    if ($mins == 60)
    {
      $mins = 0;
      ++$hours;
    }
    
    $pretty_time = sprintf("%s:%02d", number_format($hours), $mins);

    return $pretty_time;
}

?>
