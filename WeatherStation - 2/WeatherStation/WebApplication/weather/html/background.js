window.onload=function() {
  startRequest();
}

var timerId;

var settings = {
  "pollInterval" : 1800000
}

function ISODateString(d){  
  var timezone = d.getTimezoneOffset()/60;
  
  function pad(n){return n<10 ? '0'+n : n}  
  
  return d.getFullYear()+'-'  
      + pad(d.getMonth()+1)+'-'  
      + pad(d.getDate())+'T'  
      + pad(d.getHours()+timezone)+':'  
      + pad(d.getMinutes())+':'  
      + pad(d.getSeconds())+'Z';  
}
      
function storeExteriorData() {
  var xmlhttp=new XMLHttpRequest();
  
  xmlhttp.onreadystatechange = function() {
    if( xmlhttp.readyState == 4 && xmlhttp.status == 200 ) {
      //console.log(xmlhttp.responseText);
      
      parser=new DOMParser();
      xmlDoc=parser.parseFromString(xmlhttp.responseText,"text/xml");
      
      var measured_humidity_ext      = parseInt(xmlDoc.getElementsByTagName("RH")[0].childNodes[0].nodeValue);
      var measured_pressure_ext      = parseInt(xmlDoc.getElementsByTagName("P")[0].childNodes[0].nodeValue);
      var measured_temperature_ext   = parseFloat(xmlDoc.getElementsByTagName("T")[0].childNodes[0].nodeValue);
      var measured_dewpoint_ext      = parseFloat(xmlDoc.getElementsByTagName("D")[0].childNodes[0].nodeValue);
      var measured_humidex_ext       = parseFloat(xmlDoc.getElementsByTagName("H")[0].childNodes[0].nodeValue);
      var measured_windspeed_ext     = parseInt(xmlDoc.getElementsByTagName("WS")[0].childNodes[0].nodeValue);
      var measured_winddirection_ext = parseInt(xmlDoc.getElementsByTagName("WD")[0].childNodes[0].nodeValue);
      var measured_rainfall_ext      = parseFloat(xmlDoc.getElementsByTagName("R")[0].childNodes[0].nodeValue);
      var measured_light_ext         = parseInt(xmlDoc.getElementsByTagName("L")[0].childNodes[0].nodeValue);
      
      // calculate heat index[Celsius] using temperature[Celsius] and relative humidity[%]      
        var F = (measured_temperature_ext*(9/5))+32;
        var heatindexF = -42.379 + 2.04901523*F + 10.14333127*measured_humidity_ext - 0.22475541*F*measured_humidity_ext - 6.83783*Math.pow(10,-3)*Math.pow(F, 2) - 5.481717*Math.pow(10,-2)*Math.pow(measured_humidity_ext, 2) + 1.22874*Math.pow(10,-3)*Math.pow(F, 2)*measured_humidity_ext + 8.5282*Math.pow(10,-4)*F*Math.pow(measured_humidity_ext, 2) - 1.99*Math.pow(10,-6)*Math.pow(F, 2)*Math.pow(measured_humidity_ext, 2);        
        var calc_heatindex = (heatindexF-32)*(5/9);

        calc_heatindex=(F < 70) ? measured_temperature_ext : calc_heatindex;
        calc_heatindex = Math.round(100 * calc_heatindex) / 100;
      ///
      
      // calculate windchill[Celsius] using temperature[Celsius] and windspeed[km/h]
        var calc_windchill = 0
        calc_windchill=(13.12+0.6215*measured_temperature_ext-11.37*Math.pow(measured_windspeed_ext,0.16)+0.3965*measured_temperature_ext*Math.pow(measured_windspeed_ext,0.16));
        calc_windchill=(measured_windspeed_ext   <= 4.8) ? measured_temperature_ext : calc_windchill;
        calc_windchill=(measured_temperature_ext >  10 ) ? measured_temperature_ext : calc_windchill;
        
        calc_windchill =  Math.round(100 * calc_windchill) / 100;
      ///
      
      localStorage.measured_humidity_ext      = measured_humidity_ext;     
      localStorage.measured_pressure_ext      = measured_pressure_ext;    
      localStorage.measured_temperature_ext   = measured_temperature_ext;  
      localStorage.measured_dewpoint_ext      = measured_dewpoint_ext;     
      localStorage.measured_humidex_ext       = measured_humidex_ext;      
      localStorage.measured_windspeed_ext     = measured_windspeed_ext;    
      localStorage.measured_winddirection_ext = measured_winddirection_ext;
      localStorage.measured_rainfall_ext      = measured_rainfall_ext ;    
      localStorage.measured_light_ext         = measured_light_ext;
      localStorage.calc_heatindex_ext         = calc_heatindex;
      localStorage.calc_windchill_ext         = calc_windchill;            
      
    }
  }

  try {
    xmlhttp.open("GET", "http://leonardpitzu.dyndns.org:81", true);
    xmlhttp.send('');
  }
  catch(err) {
    console.log("ERROR: " + err.description);
  }
  
}        

function storeChartData(selected_chart, date) {
  var xmlhttp=new XMLHttpRequest();
  var csv_data = "http://api.pachube.com/v2/feeds/46257/datastreams/" + selected_chart  + ".csv?end=" + ISODateString(date) + "&per_page=1000&duration=1week";
   
  xmlhttp.onreadystatechange = function() {
    if( xmlhttp.readyState == 4 && xmlhttp.status == 200 ) {
      //console.log(xmlhttp.responseText);
      
      switch(selected_chart) {
        case 0: localStorage.temperature_hist_chart = xmlhttp.responseText; break;
        case 1: localStorage.pressure_hist_chart    = xmlhttp.responseText; break;
        case 2: localStorage.rh_hist_chart          = xmlhttp.responseText; break;
        case 3: localStorage.dewpoint_hist_chart    = xmlhttp.responseText; break;
        case 4: localStorage.humidex_hist_chart     = xmlhttp.responseText; break;
        case 5: localStorage.windspeed_hist_chart   = xmlhttp.responseText; break;
        case 6: localStorage.winddir_hist_chart     = xmlhttp.responseText; break;
        case 7: localStorage.rain_hist_chart        = xmlhttp.responseText; break;
        case 8: localStorage.light_hist_chart       = xmlhttp.responseText; break;
      }              
    }
  }

  try {
    xmlhttp.open("GET", csv_data, true);
    xmlhttp.setRequestHeader("X-PachubeApiKey", "6eApn6wF33PqzSHgmtqvyWuxHOQRHDarfISspWE5yT4");
    xmlhttp.send('');
  }
  catch(err) {
    console.log("ERROR: " + err.description);
  }
}

function storeMinMax(cnt) {
  var xmlhttp=new XMLHttpRequest();
  
  xmlhttp.onreadystatechange = function() {
    if( xmlhttp.readyState == 4 && xmlhttp.status == 200 ) {
      parser=new DOMParser();
      xmlDoc=parser.parseFromString(xmlhttp.responseText,"text/xml");
      
      switch(cnt) {
        case 0: localStorage.temperature_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.temperature_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;
        case 1: localStorage.pressure_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.pressure_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;        
        case 2: localStorage.rh_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.rh_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;        
        case 3: localStorage.dewpoint_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.dewpoint_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;        
        case 4: localStorage.humidex_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.humidex_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;        
        case 5: localStorage.windspeed_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.windspeed_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;
        case 6: localStorage.winddir_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.winddir_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;
        case 7: localStorage.rain_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.rain_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;
        case 8: localStorage.light_max = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("max_value")[0].childNodes[0].nodeValue))/100;
                localStorage.light_min = Math.round(100 * parseFloat(xmlDoc.getElementsByTagName("min_value")[0].childNodes[0].nodeValue))/100;
                break;
      }
    }
  }
  
  try {
    xmlhttp.open("GET", "http://api.pachube.com/v2/feeds/46257/datastreams/" + cnt + ".xml", true);
    xmlhttp.setRequestHeader("X-PachubeApiKey", "6eApn6wF33PqzSHgmtqvyWuxHOQRHDarfISspWE5yT4");
    xmlhttp.send('');
  }
  catch(err) {
    alert("ERROR: " + err.description);
  }            

}

function startRequest() {
  var date = new Date();
  
  storeExteriorData();
  for(var cnt=0; cnt < 9; cnt++) {
    storeChartData(cnt, date);
    storeMinMax(cnt);
  }
  
  localStorage.lastPollingTime = date;
  
  timerId = window.setTimeout(startRequest, settings["pollInterval"]);
}

function stopRequest() {
  window.clearTimeout(timerId);
}
