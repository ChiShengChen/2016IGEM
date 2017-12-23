/// handles json command requests(HTTP command interface)
void jsonCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  URLPARAM_RESULT rc;
  char name[PARAMETER_LEN];
  char value[PARAMETER_LEN];
  
  if (type == WebServer::HEAD) return;
  
  //Loop through all Parameters in URL
  if (strlen(url_tail) > 0) {
    while (strlen(url_tail)) {
      
      rc = server.nextURLparam(&url_tail, name, PARAMETER_LEN, value, VALUE_LEN);
      if (rc != URLPARAM_EOS) {
        
        //Navigation Command - Each Page calls the appropriate routine to build the response
        if (strcmp(name, "io") == 0) {
          server.httpSuccess();
          
          if( strcmp( value, "rs" ) == 0 ) {
            reset(); //reset the board
            return;
          } else {
            server.print("ERROR");
          }
        }
      }
    }
  }
}

// jumps to executing the home page and displaying it in a broswer
void XMLdata(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  if (type == WebServer::POST) return;
  if (type == WebServer::HEAD) return;
  
  if (type == WebServer::GET) {
    P(headerP) = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    P(data_startP) = "<M>";
      P(RH_startP) = "<RH>"; P(RH_endP) = "</RH>";
      P(P_startP ) = "<P>";  P(P_endP ) = "</P>";
      P(T_startP ) = "<T>";  P(T_endP ) = "</T>";
      P(D_startP ) = "<D>";  P(D_endP ) = "</D>";
      P(H_startP ) = "<H>";  P(H_endP ) = "</H>";
      P(WS_startP) = "<WS>"; P(WS_endP) = "</WS>";
      P(WD_startP) = "<WD>"; P(WD_endP) = "</WD>";
      P(R_startP ) = "<R>";  P(R_endP ) = "</R>";
      P(L_startP ) = "<L>";  P(L_endP ) = "</L>";
    P(data_endP) = "</M>";
    
    
    server.httpSuccess("text/xml; charset=utf-8");
    
    server.printP(headerP);
    server.printP(data_startP);
      server.printP(RH_startP); server.print(results.RH              ); server.printP(RH_endP);
      server.printP(P_startP ); server.print(results.pressure,      0); server.printP(P_endP );
      server.printP(T_startP ); server.print(results.temperature,   1); server.printP(T_endP );
      server.printP(D_startP ); server.print(results.dewpoint,      1); server.printP(D_endP );
      server.printP(H_startP ); server.print(results.humidex,       1); server.printP(H_endP );
      server.printP(WS_startP); server.print(results.windspeed,     1); server.printP(WS_endP);
      server.printP(WD_startP); server.print(results.winddirection   ); server.printP(WD_endP);
      server.printP(R_startP ); server.print(results.precipitation, 1); server.printP(R_endP );
      server.printP(L_startP ); server.print(results.light           ); server.printP(L_endP );
    server.printP(data_endP);
  }
}

void initWebServer() {
  Ethernet.begin(mac,ip);
  
  webserver.addCommand("", &jsonCmd);
  webserver.setDefaultCommand(&XMLdata); //Create json commands
  
  webserver.begin();
}
