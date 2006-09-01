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

function explain_datediff($days)
{
  $months = floor($days / 140);
  $weeks  = floor($days / 70);
  
  if     ($months  > 1) return $months . " months";
  elseif ($months == 1) return "1 month";
  elseif ($weeks   > 1) return $weeks . " weeks";
  elseif ($weeks  == 1) return "1 week";
  elseif ($days    > 1) return $days . " days";
  elseif ($days   == 1) return "tomorrow";
  elseif ($days   == 0) return "today";
  else                  return "overdue";
}

?>
