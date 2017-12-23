var initial_qr = false;
var timer_id = 0;

window.onload=function() {
  restore_options();
  drawExterior();
  drawWindRain();
  lastPollingTime();
  
  
  // forecast tab
  // ==================================================================================
  document.getElementById('forecast').onclick = function() {
    forecast();
  }
  // ==================================================================================
  
  
  // history tab
  // ==================================================================================
  document.getElementById('history').onclick = function() {
    initialDraw();
  }
  
  document.getElementById('which_chart').onchange = function() {
    updateChart();
  }
  document.getElementById('compare_chart_one').onchange = function() {
    updateChart();
  } 
  document.getElementById('compare_chart_two').onchange = function() {
    updateChart();
  }
  
  document.getElementById('download_button').onclick = function() {
    chrome.tabs.create({url:download_chart()});
  }
  // ==================================================================================
  
  
  // options tab
  // ==================================================================================
  document.getElementById('save_settings').onclick = function() {
    save_options();
  }
  // ==================================================================================
  
  
  // about tab
  // ==================================================================================
  document.getElementById('about').onclick = function() {
    if(initial_qr == false) jQuery('#qr_url').qrcode({width: 64, height: 64, text: "https://pachube.com/feeds/46257"});
    
    initial_qr = true;
  }
  
  document.getElementById('code_download_link').onclick = function() {
    chrome.tabs.create({url:this.href});
  }
  
  document.getElementById('mail_link').onclick = function() {
    chrome.tabs.create({url:this.href});
  }
  // ==================================================================================
}
