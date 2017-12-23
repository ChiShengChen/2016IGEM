// draw exterior data gauges
function drawExterior() {            
  document.getElementById('humidex').innerHTML   = parseFloat(localStorage.measured_humidex_ext);
  document.getElementById('dewpoint').innerHTML  = parseFloat(localStorage.measured_dewpoint_ext) + " &degC";
  document.getElementById('windchill').innerHTML = parseFloat(localStorage.calc_windchill_ext) + " &degC";
  
  var temperature_ext = new RGraph.Thermometer('temperature_exterior', -30, 100, parseFloat(localStorage.measured_temperature_ext));
  temperature_ext.Set('chart.colors', ['red']);
  temperature_ext.Set('chart.title.side', 'Temperature [°C]');
  temperature_ext.Set('chart.shadow', true);
  temperature_ext.Set('chart.shadow.offsetx', 0);
  temperature_ext.Set('chart.shadow.offsety', 0);
  temperature_ext.Set('chart.text.color', 'black');
  temperature_ext.Set('chart.ylabels.count', '8');
  temperature_ext.Set('chart.scale.visible', true);
  temperature_ext.Set('chart.shadow.blur',   15);
  temperature_ext.Set('chart.gutter.left',   15);
  temperature_ext.Set('chart.gutter.right',  15);
  temperature_ext.Set('chart.gutter.top',    15);
  temperature_ext.Set('chart.gutter.bottom', 15);
  RGraph.Effects.jQuery.Reveal(temperature_ext);
  
  var heatindex_ext = new RGraph.Thermometer('heatindex_exterior', -30, 100, parseFloat(localStorage.calc_heatindex_ext));
  heatindex_ext.Set('chart.colors', ['blue']);
  heatindex_ext.Set('chart.title.side', 'Heat Index [°C]');
  heatindex_ext.Set('chart.shadow', true);
  heatindex_ext.Set('chart.shadow.offsetx', 0);
  heatindex_ext.Set('chart.shadow.offsety', 0);
  heatindex_ext.Set('chart.text.color', 'black');
  heatindex_ext.Set('chart.ylabels.count', '8');
  heatindex_ext.Set('chart.scale.visible', true);
  heatindex_ext.Set('chart.shadow.blur',   15);
  heatindex_ext.Set('chart.gutter.left',   15);
  heatindex_ext.Set('chart.gutter.right',  15);
  heatindex_ext.Set('chart.gutter.top',    15);
  heatindex_ext.Set('chart.gutter.bottom', 15);
  RGraph.Effects.jQuery.Reveal(heatindex_ext);
  
  var pressure = new RGraph.Gauge('pressure', 700, 800, parseInt(localStorage.measured_pressure_ext));
  pressure.Set('chart.title', 'Pressure');
  pressure.Set('chart.title.bottom', '[mmHg]');
  pressure.Set('chart.title.bottom.size', 12);
  pressure.Set('chart.title.bottom.bold', true);
  pressure.Set('chart.title.bottom.color', 'blue');
  pressure.Set('chart.red.start', 770);
  pressure.Set('chart.red.color', 'red');
  pressure.Set('chart.yellow.color', 'orange');
  pressure.Set('chart.green.end', 750);
  pressure.Set('chart.green.color', 'blue');
  pressure.Set('chart.shadow.blur',   15);
  pressure.Set('chart.gutter.bottom', 15);           
  pressure.Set('chart.gutter.top',    15);           
  pressure.Set('chart.gutter.left',   15);           
  pressure.Set('chart.gutter.right',  15);
  RGraph.Effects.Gauge.Grow(pressure);
  
  var humidity_ext = new RGraph.Gauge('humidity_exterior', 0, 100, parseInt(localStorage.measured_humidity_ext));
  humidity_ext.Set('chart.title', 'Humidity');
  humidity_ext.Set('chart.title.bottom', '[%]');
  humidity_ext.Set('chart.title.bottom.size', 12);
  humidity_ext.Set('chart.title.bottom.bold', true);
  humidity_ext.Set('chart.title.bottom.color', 'blue');
  humidity_ext.Set('chart.red.start', 70);
  humidity_ext.Set('chart.red.color', '#7D0552');
  humidity_ext.Set('chart.yellow.color', 'green');
  humidity_ext.Set('chart.green.end', 40);
  humidity_ext.Set('chart.green.color', 'red');
  humidity_ext.Set('chart.shadow.blur',   15);
  humidity_ext.Set('chart.gutter.bottom', 15);           
  humidity_ext.Set('chart.gutter.top',    15);           
  humidity_ext.Set('chart.gutter.left',   15);           
  humidity_ext.Set('chart.gutter.right',  15);
  RGraph.Effects.Gauge.Grow(humidity_ext);
}
  
function drawWindRain() {  
  var wind_speed = parseInt(localStorage.measured_windspeed_ext);
  
  if     (      wind_speed < 1  ) document.getElementById('beaufort').innerHTML = "CALM - "            + wind_speed + " km/h";
  else if(1   < wind_speed < 5  ) document.getElementById('beaufort').innerHTML = "LIGHT AIR - "       + wind_speed + " km/h";
  else if(6   < wind_speed < 11 ) document.getElementById('beaufort').innerHTML = "LIGHT BREEZE - "    + wind_speed + " km/h";
  else if(12  < wind_speed < 19 ) document.getElementById('beaufort').innerHTML = "GENTLE BREEZE - "   + wind_speed + " km/h";
  else if(20  < wind_speed < 28 ) document.getElementById('beaufort').innerHTML = "MODERATE BREEZE - " + wind_speed + " km/h";
  else if(28  < wind_speed < 38 ) document.getElementById('beaufort').innerHTML = "FRESH BREEZE - "    + wind_speed + " km/h";
  else if(39  < wind_speed < 49 ) document.getElementById('beaufort').innerHTML = "STRONG BREEZE - "   + wind_speed + " km/h";
  else if(50  < wind_speed < 61 ) document.getElementById('beaufort').innerHTML = "NEAR GALE - "       + wind_speed + " km/h";
  else if(62  < wind_speed < 74 ) document.getElementById('beaufort').innerHTML = "GALE - "            + wind_speed + " km/h";
  else if(75  < wind_speed < 88 ) document.getElementById('beaufort').innerHTML = "STRONG GALE - "     + wind_speed + " km/h";
  else if(89  < wind_speed < 102) document.getElementById('beaufort').innerHTML = "STORM - "           + wind_speed + " km/h";
  else if(103 < wind_speed < 117) document.getElementById('beaufort').innerHTML = "VIOLENT STORM - "   + wind_speed + " km/h";
  else if(      wind_speed > 118) document.getElementById('beaufort').innerHTML = "HURRICANE - "       + wind_speed + " km/h";        
  
  var wind_dir = parseInt(localStorage.measured_winddirection_ext);
  var displayed_wind_direction = 0;
  if( (wind_dir >=192) & (wind_dir <= 202) ) displayed_wind_direction = 0  ;
  if( (wind_dir >=110) & (wind_dir <= 120) ) displayed_wind_direction = 45 ;
  if( (wind_dir >=18 ) & (wind_dir <= 28 ) ) displayed_wind_direction = 90 ;
  if( (wind_dir >=41 ) & (wind_dir <= 51 ) ) displayed_wind_direction = 135;
  if( (wind_dir >=67 ) & (wind_dir <= 77 ) ) displayed_wind_direction = 180;
  if( (wind_dir >=153) & (wind_dir <= 163) ) displayed_wind_direction = 225;
  if( (wind_dir >=231) & (wind_dir <= 241) ) displayed_wind_direction = 270;
  if( (wind_dir >=216) & (wind_dir <= 226) ) displayed_wind_direction = 315;
  
  var windvane = new steelseries.Compass('windvane', {size: 250});
  windvane.setFrameDesign(steelseries.FrameDesign.STEEL);
  windvane.setBackgroundColor(steelseries.BackgroundColor.WHITE);
  windvane.setForegroundType(steelseries.ForegroundType.TYPE3);
  windvane.setValue(displayed_wind_direction); 

  var rainmeter = new RGraph.Bar('rainmeter', [(parseFloat(localStorage.measured_rainfall_ext)*60)]);
  rainmeter.Set('chart.gutter.left', 75);
  rainmeter.Set('chart.title.yaxis.pos', 0.22);
  rainmeter.Set('chart.title.yaxis', 'Rainfall [mm/h]');
  rainmeter.Set('chart.text.size', 12);
  rainmeter.Set('chart.xlabels.offset', 10);
  rainmeter.Set('chart.background.grid', false);
  rainmeter.Draw();

  var coords = rainmeter.coords[0];  
  var vp = new RGraph.VProgress('rainmeter', (parseFloat(localStorage.measured_rainfall_ext)*60), 100);
  vp.Set('chart.title.side', 'Rainfall [mm/h]');
  vp.Set('chart.colors', ['blue']);
  vp.Set('chart.gutter.left', coords[0]);
  vp.Set('chart.gutter.top', coords[1]);
  vp.Set('chart.gutter.right', rainmeter.canvas.width - (coords[0] + coords[2]) );
  vp.Set('chart.gutter.bottom', 25);
  vp.Set('chart.labels.specific', []);
  vp.Set('chart.tickmarks', false);
  vp.Draw();  
}      

// display time & data when data was last fetched
function lastPollingTime() {
  var month = new Array("jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec");
  var date = new Date(localStorage.lastPollingTime);
  
  document.getElementById('lasttime').innerHTML = date.getDate() + '/' + month[date.getMonth()] + '/' + date.getFullYear() +' ' + date.getHours() + ':' + date.getMinutes() + ':' + date.getSeconds(); 
}

// Saves options to localStorage.
function save_options() {      
  var selectDataPoll   = document.getElementById("poll_data");
  
  localStorage.data_poll_interval   = selectDataPoll[selectDataPoll.selectedIndex].value;
  
  bgPage = chrome.extension.getViews()[0];
  bgPage.settings = {
    "pollInterval" : parseInt(localStorage.data_poll_interval, 10) * 1000 // needs milliseconds
  }
  
  // Update status to let user know options were saved.
  var status = document.getElementById("status");
  status.innerHTML = "Options Saved.";
  setTimeout(function() {
    status.innerHTML = "";
  }, 2000);
}

// Restores select box state to saved value from localStorage.
function restore_options() {
  var pollingTime_Data   = localStorage.data_poll_interval;
  
  var select_data        = document.getElementById("poll_data");
  
  for(var i = 0; i < select_data.children.length; i++) {
    var child = select_data.children[i];
    if (child.value == pollingTime_Data) {
      child.selected = "true";
      break;
    }
  }
  
}
