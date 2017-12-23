function ISODateString(d) {  
  var timezone = d.getTimezoneOffset()/60;
  
  function pad(n){return n<10 ? '0'+n : n}  
  
  return d.getFullYear()+'-'  
      + pad(d.getMonth()+1)+'-'  
      + pad(d.getDate())+'T'  
      + pad(d.getHours()+timezone)+':'  
      + pad(d.getMinutes())+':'  
      + pad(d.getSeconds())+'Z';
}

function forecast() {
  var trend;
  
  var pressure_average_one_day    = computeAverage(localStorage.pressure_hist_chart, 24);
  var pressure_average_half_day   = computeAverage(localStorage.pressure_hist_chart, 12);
  var pressure_average_one_hour   = computeAverage(localStorage.pressure_hist_chart, 1);
  var pressure_average_two_hour   = computeAverage(localStorage.pressure_hist_chart, 2);
  var pressure_average_three_hour = computeAverage(localStorage.pressure_hist_chart, 3);
  
  var humidity_average_one_day  = computeAverage(localStorage.rh_hist_chart, 24);
  var humidity_average_one_hour = computeAverage(localStorage.rh_hist_chart, 1);
  
  var temperature_average_one_day  = computeAverage(localStorage.temperature_hist_chart, 24);
  var temperature_average_one_hour = computeAverage(localStorage.temperature_hist_chart, 1);
  
  var curent_date = new Date();
  var myDate_low = new Date;
  var myDate_high = new Date;
  myDate_low.setHours(6);
  myDate_high.setHours(18);
  
  
  // night forecast
  if( ( (curent_date.getHours()) > myDate_low.getHours() ) && ( (curent_date.getHours()) < myDate_high.getHours() ) ){ //after 6 in the morning && before 18 o'clock
    if( (pressure_average_one_day > 720) && (humidity_average_one_day < 50) ) { // good weather
      document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-night-icon.png";
    }
    
    if((pressure_average_one_hour < pressure_average_two_hour) && (humidity_average_one_hour > humidity_average_one_day)) {
      trend = "humidity is rising; a storm is likely in 5 to 6 hours";
      document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-storm-night-icon.png";
    }
    
    if(pressure_average_half_day > pressure_average_one_day) {        // slow increase in pressure
      trend = "general dry, and colder weather";
      document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-few-clouds-night-icon.png";
      
      if(pressure_average_one_hour > pressure_average_two_hour > pressure_average_one_day) {
        trend += " but a low pressure cell is approaching";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-night-icon.png";
      }
    } else if(pressure_average_half_day < pressure_average_one_day) { // slow decrease in pressure
      trend = "in general a stormy, rainy and windy weather";
      if(temperature_average_one_day < 3) document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-snow-scattered-night-icon.png";
      else                                document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-showers-night-icon.png";
      
      if(pressure_average_one_hour > pressure_average_two_hour) {
        trend += " with a short period of fair weather is likely";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-night-icon.png";
      }  
        
      if(pressure_average_one_hour > pressure_average_two_hour > pressure_average_three_hour > pressure_average_half_day) {
        trend += " but a long period of good weather is on its way";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-night-icon.png";
      }  
        
    } else {                                                          // constant pressure
      if(humidity_average_one_hour > humidity_average_one_day) {
        trend = "rising humidity; precipitations are likely";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-showers-scattered-night-icon.png";
      } else if(humidity_average_one_hour < humidity_average_one_day) {
        trend = "falling humidity; precipitations are unlikely";
        if(humidity_average_one_day < 50) {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-night-icon.png";
        } else {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-night-icon.png";
        }
      } else {
        trend = "constant humidity; weather is likely to stay as it is";
        if(humidity_average_one_day < 50) {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-night-icon.png";
        } else {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-night-icon.png";
        }
      }
    }
    
  } else { //day forecast          
    if( (pressure_average_one_day > 720) && (humidity_average_one_day < 50) ) { // good weather
      document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-icon.png";
    }
    
    if((pressure_average_one_hour < pressure_average_two_hour) && (humidity_average_one_hour > humidity_average_one_day)) {
      trend = "humidity is rising; a storm is likely in 5 to 6 hours";
      document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-showers-day-icon.png";
    }
    
    if(pressure_average_half_day > pressure_average_one_day) {        // slow increase in pressure
      trend = "general dry, and colder weather";
      document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-few-clouds-icon.png";
      
      if(pressure_average_one_hour > pressure_average_two_hour > pressure_average_one_day) {
        trend += " but a low pressure cell is approaching";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-icon.png";
      }
    } else if(pressure_average_half_day < pressure_average_one_day) { // slow decrease in pressure
      trend = "in general a stormy, rainy and windy weather";
      if(temperature_average_one_day < 3) document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-snow-scattered-day-icon.png";
      else                                document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-showers-day-icon.png";
      
      if(pressure_average_one_hour > pressure_average_two_hour) {
        trend += " with a short period of fair weather is likely";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-icon.png";
      }
        
      if(pressure_average_one_hour > pressure_average_two_hour > pressure_average_three_hour > pressure_average_half_day) {
        trend += " but a long period of good weather is on its way";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-icon.png";
      }  
        
    } else {                                                          // constant pressure
      if(humidity_average_one_hour > humidity_average_one_day) {
        trend = "rising humidity; precipitations are likely";
        document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-showers-scattered-day-icon.png";
      } else if(humidity_average_one_hour < humidity_average_one_day) {
        trend = "falling humidity; precipitations are unlikely";
        if(humidity_average_one_day < 50) {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-icon.png";
        } else {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-icon.png";
        }
      } else {
        trend = "constant humidity; weather is likely to stay as it is";
        if(humidity_average_one_day < 50) {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clear-icon.png";
        } else {
          document.getElementById("forecast_img").src = "../images/status_weather_icons/Status-weather-clouds-icon.png";
        }
      }
    }
      
  }        
  
  document.getElementById('forecast_data').innerHTML = trend;
}

function computeAverage(data, window) {
  var temp = new Array();
  var average = 0;
  var i = 0;
  var window_length = 0;
  
  temp = data.split('\n');
  
  switch(window) {
    case 1:  window_length = 4;             break; // 4 measures/hour (every 15 minutes)
    case 2:  window_length = 8;             break;
    case 3:  window_length = 12;            break;
    case 6:  window_length = 24;            break;
    case 12: window_length = 48;            break;
    case 24: window_length = 96;            break;
    default: window_length = temp.length-1; break;
  }
  for(i=temp.length-1; i>(temp.length-1-window_length); i--) {
    average = average + parseInt(temp[i].split(',')[1]);
  }
  
  return Math.round(100 *(average/window_length)/100);
}
