var unit_array = new Array("°C", "mmHg", "%", "°C", "", "km/h", "°", "mm/min", "lx", "");
var measures_array = new Array("Temperature", "Pressure", "Relative Humidity", "Dew Point", "Humidity Index", "Wind Speed", "Wind Direction", "Precipitation", "Light", "");
var hist_data = new Array(localStorage.temperature_hist_chart, localStorage.pressure_hist_chart, localStorage.rh_hist_chart, localStorage.dewpoint_hist_chart, localStorage.humidex_hist_chart, localStorage.windspeed_hist_chart, localStorage.winddir_hist_chart, localStorage.rain_hist_chart, localStorage.light_hist_chart);
var maxs = new Array(localStorage.temperature_max, localStorage.pressure_max, localStorage.rh_max, localStorage.dewpoint_max, localStorage.humidex_max, localStorage.windspeed_max, localStorage.rain_max, localStorage.winddir_max, localStorage.light_max, "n.a.");
var mins = new Array(localStorage.temperature_min, localStorage.pressure_min, localStorage.rh_min, localStorage.dewpoint_min, localStorage.humidex_min, localStorage.windspeed_min, localStorage.winddir_min, localStorage.rain_min, localStorage.light_min, "n.a.");
  
var first_click = false;

function updateChart() {
  var selectedChart  = document.getElementById("which_chart");        
  var selected_chart = parseInt(selectedChart[selectedChart.selectedIndex].value);
  var csv_data;
  
  var first_chart           = document.getElementById("compare_chart_one");
  var first_selected_chart  = parseInt(first_chart[first_chart.selectedIndex].value);
  var second_chart          = document.getElementById("compare_chart_two")
  var second_selected_chart = parseInt(second_chart[second_chart.selectedIndex].value);
  
  var first_selected_comparison_chart;
  var second_selected_comparison_chart;
  
  var op_A = document.getElementById("compare_chart_one").getElementsByTagName("option");
  var op_B = document.getElementById("compare_chart_two").getElementsByTagName("option");
                 
  first_selected_comparison_chart  = hist_data[first_selected_chart];
  second_selected_comparison_chart = hist_data[second_selected_chart];
  for(var i=0; i<=8; i++) {
    op_A[i].disabled = false;
    op_B[i].disabled = false;
  }
  op_B[first_selected_chart].disabled  = true;
  op_A[second_selected_chart].disabled = true;
  
  if(selected_chart == 9) csv_data = concatenateCSV(first_selected_comparison_chart, second_selected_comparison_chart);
  else                    csv_data = hist_data[selected_chart];
  
  legend = measures_array[selected_chart];
  
  if     (selected_chart == 9) unit = "";
  else if(selected_chart == 4) unit = measures_array[selected_chart];
  else                         unit = measures_array[selected_chart] + " [" + unit_array[selected_chart] + "]";
  
  
  if(selected_chart == 9) {
    document.getElementById("compare_chart_one").style.display = "";
    document.getElementById("compare_chart_two").style.display = "";
    document.getElementById("which_interval_chart").style.display = "none";
    document.getElementById("download_button").style.display = "none";
  } else {
    document.getElementById("compare_chart_one").style.display = "none";
    document.getElementById("compare_chart_two").style.display = "none";
    document.getElementById("which_interval_chart").style.display = "";
    document.getElementById("download_button").style.display = "";
  }
  
  document.getElementById('max').innerHTML = maxs[selected_chart] + " " + unit_array[selected_chart]; 
  document.getElementById('min').innerHTML = mins[selected_chart] + " " + unit_array[selected_chart];
  
  var date = 0
  if(selected_chart == 9) date = "";
  else                    date = "\"Date\"";
  
  if(selected_chart == 9) { 
    g = new Dygraph(
        document.getElementById("history_chart"),
        String(csv_data), {
          rollPeriod: 7,
          showRoller: true,
          animatedZooms: true,
          width: 600,
          height: 300,
          labels: ["Date", "A", "B"],
          xValueParser: function(x) { return Date.parse(x); },
          delimiter: ",",
          ylabel: unit,
          legend: 'always',
          animatedZooms: true,
          labelsDivStyles: { 'textAlign': 'right' }
        }
      );
  } else {        
    c = new Dygraph(
        document.getElementById("history_chart"),
        String(csv_data), {
          rollPeriod: 7,
          showRoller: true,
          animatedZooms: true,
          width: 600,
          height: 300,
          labels: ["Date", legend],
          xValueParser: function(x) { return Date.parse(x); },
          delimiter: ",",
          ylabel: unit,
          legend: 'always',
          animatedZooms: true,
          labelsDivStyles: { 'textAlign': 'right' }
        }
      );
  }
}

function initialDraw() { // default chart & min/max
  if(first_click == false) {          
    first_click = true;
    
    document.getElementById('max').innerHTML = localStorage.temperature_max + " " + unit_array[0]; 
    document.getElementById('min').innerHTML = localStorage.temperature_min + " " + unit_array[0];
    
    g = new Dygraph(
          document.getElementById("history_chart"),
          String(localStorage.temperature_hist_chart), {
            rollPeriod: 7,
            showRoller: true,
            animatedZooms: true,
            width: 600,
            height: 300,
            labels: [ "Date", "Temperature" ],
            xValueParser: function(x) { return Date.parse(x); },
            delimiter: ",",
            ylabel: 'Temperature [°C]',
            legend: 'always',
            animatedZooms: true,
            labelsDivStyles: { 'textAlign': 'right' }
          }
        );
  }
}

function concatenateCSV(CSV_A, CSV_B) {
  var output = 0;
  var line_counter = 0;
  
  var number_lines_A = CSV_A.split('\n').length;
  var number_lines_B = CSV_B.split('\n').length;
  
  if(number_lines_A < number_lines_B) line_counter = number_lines_A;
  else                                line_counter = number_lines_B;
  
  var line_A = CSV_A.split('\n');
  var line_B = CSV_B.split('\n');
  
  var min_A = parseInt(line_A[0].split(',')[1]);
  var max_A = parseInt(line_A[0].split(',')[1]);
  var max_B = parseInt(line_B[0].split(',')[1]);
  var min_B = parseInt(line_B[0].split(',')[1]);
  
  var i=0;
  var date = 0;
  var new_date = 0;        
  for(i=0; i<=line_counter-1; i++) {          
    if (max_A < parseInt(line_A[i].split(',')[1])) max_A = parseInt(line_A[i].split(',')[1]);
    if (min_A > parseInt(line_A[i].split(',')[1])) min_A = parseInt(line_A[i].split(',')[1]);
    if (max_B < parseInt(line_B[i].split(',')[1])) max_B = parseInt(line_B[i].split(',')[1]);
    if (min_B > parseInt(line_B[i].split(',')[1])) min_B = parseInt(line_B[i].split(',')[1]);
  }
  
  for(i=0; i<=line_counter-1; i++) {
    date = line_A[i].split(',')[0].split('T')[0];
    new_date = date.split('-')[0] + "/" + date.split('-')[1] + "/" + date.split('-')[2] + " " + line_A[i].split('T')[1].split('.')[0];
    
    output += new_date + "," + Math.round(100 * ((2*((parseInt(line_A[i].split(',')[1]) - min_A)/(max_A-min_A)))-1)) / 100 + "," + Math.round(100 * ((2*((parseInt(line_B[i].split(',')[1]) - min_B)/(max_B-min_B)))-1)) / 100 + "\n";
  }
  return output;
}

function download_chart() {
  var selected_chart    = parseInt(document.getElementById("which_chart")[document.getElementById("which_chart").selectedIndex].value);
  var selected_interval = parseInt(document.getElementById("which_interval_chart")[document.getElementById("which_interval_chart").selectedIndex].value);
  
  var duration;
  switch(selected_interval) {
    case 0  : duration = "1hour";   interval_value="0";     break;
    case 1  : duration = "2hours";  interval_value="0";     break;
    case 2  : duration = "3hours";  interval_value="0";     break;
    case 3  : duration = "12hours"; interval_value="900";   break;
    case 4  : duration = "1day";    interval_value="900";   break;
    case 5  : duration = "1week";   interval_value="900";   break;
    case 6  : duration = "1month";  interval_value="3600";  break;
    case 7  : duration = "3month";  interval_value="21600"; break;
    case 8  : duration = "6month";  interval_value="43200"; break;
    case 9  : duration = "1year";   interval_value="43200"; break;
    default : duration = "1hour";   interval_value="0";     break;
  }
  var csv_data = "http://api.pachube.com/v2/feeds/46257/datastreams/" + selected_chart + ".csv?end=" + ISODateString(new Date()) + "&interval=" + interval_value + "&duration=" + duration + "&per_page=1000" + "?key=6eApn6wF33PqzSHgmtqvyWuxHOQRHDarfISspWE5yT4";

  return csv_data;
}