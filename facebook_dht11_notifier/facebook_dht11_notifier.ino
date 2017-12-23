/*
  In this sketch, it assumed you already have a user access token. Using this token you can test if it works to post something on facebook.
  
  If you don't have access token, please reference below to get one:
    https://developers.facebook.com/tools/explorer/
  And you need these access right to publish on feed:
    https://developers.facebook.com/docs/graph-api/reference/v2.8/user/feed

  If you already have one, using link blow to check the access right:
    https://developers.facebook.com/tools/debug/accesstoken
  You can also extend the expire time of access token in this page.

  This sketch use PMS3003 to get PM2.5 value. And it using pin 0 and pin 1 as UART RX and UART TX
*/

#include <WiFi.h>
//#include <PMS3003.h>
#include "DHT.h"

#define DHTPIN 2
DHT dht(DHTPIN, DHT11);

char ssid[] = "EE332";     // your network SSID (name)
char pass[] = "makerealrealmake";  // your network password
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "graph.facebook.com";    // facebook graph api service

char access_token[] = "EAACEdEose0cBAHzfk4VDsRQSLJZAfAEkbic4JAgzzI0LKou7nw531TqVhBMdI6SHfUiuehXlOsvZBypOfL5SMAkAKSW3DuEo52BeQDMfZBG5W3WIEcOFRXgH1N5l6JKPJ5UiOGiAjNxUiYKgGxL4XG9JMyiIPDQA7ezxxQM7IWUgnXcEdMHWqPSmBTCZCBiVvIKLzy4nNAZDZD";

WiFiSSLClient client;

#define FEED_ID "me"
//#define FEED_ID "1210426395661463"

//PMS3003 pms(0, 1);

struct humid_message {
  char *msg_part1;
  char *msg_part2;
};

#define MSG_LANGUAGE_EN 1
//#define MSG_LANGUAGE_ZHTW 1

#define humid_LEVEL_SIZE 4
struct humid_message humid_message_template[humid_LEVEL_SIZE] = {
#if (MSG_LANGUAGE_EN)
  {"PM2.5%20concentration:%20","%20(%CE%BCg%2Fm3)%20(Low)%0A%0AAccompanying%20health%20messages%20for%20the%20general%20population%3A%0A%E3%80%80%E3%80%80Enjoy%20your%20usual%20outdoor%20activities%0AAccompanying%20health%20messages%20for%20at-risk%20individuals%3A%0A%E3%80%80%E3%80%80Enjoy%20your%20usual%20outdoor%20activities"},
  {"PM2.5%20concentration:%20","%20(%CE%BCg%2Fm3)%20(Moderate)%0A%0AAccompanying%20health%20messages%20for%20the%20general%20population%3A%0A%E3%80%80%E3%80%80Enjoy%20your%20usual%20outdoor%20activities%0AAccompanying%20health%20messages%20for%20at-risk%20individuals%3A%0A%E3%80%80%E3%80%80Adults%20and%20children%20with%20lung%20problems%2C%20and%20adults%20with%20heart%20problems%2C%20who%20experience%20symptoms%2C%20should%20consider%20reducing%20strenuous%20physical%20activity%2C%20particularly%20outdoors."},
  {"PM2.5%20concentration:%20","%20(%CE%BCg%2Fm3)%20(High)%0A%0AAccompanying%20health%20messages%20for%20the%20general%20population%3A%0A%E3%80%80%E3%80%80Anyone%20experiencing%20discomfort%20such%20as%20sore%20eyes%2C%20cough%20or%20sore%20throat%20should%20consider%20reducing%20activity%2C%20particularly%20outdoors.%0AAccompanying%20health%20messages%20for%20at-risk%20individuals%3A%0A%E3%80%80%E3%80%80Adults%20and%20children%20with%20lung%20problems%2C%20and%20adults%20with%20heart%20problems%2C%20should%20reduce%20strenuous%20physical%20exertion%2C%20particularly%20outdoors%2C%20and%20particularly%20if%20they%20experience%20symptoms.%20People%20with%20asthma%20may%20find%20they%20need%20to%20use%20their%20reliever%20inhaler%20more%20often.%20Older%20people%20should%20also%20reduce%20physical%20exertion."},
  {"PM2.5%20concentration:%20","%20(%CE%BCg%2Fm3)%20(Very%20High)%0A%0AAccompanying%20health%20messages%20for%20the%20general%20population%3A%0A%E3%80%80%E3%80%80Reduce%20physical%20exertion%2C%20particularly%20outdoors%2C%20especially%20if%20you%20experience%20symptoms%20such%20as%20cough%20or%20sore%20throat.%0AAccompanying%20health%20messages%20for%20at-risk%20individuals%3A%0A%E3%80%80%E3%80%80Adults%20and%20children%20with%20lung%20problems%2C%20adults%20with%20heart%20problems%2C%20and%20older%20people%2C%20should%20avoid%20strenuous%20physical%20activity.%20People%20with%20asthma%20may%20find%20they%20need%20to%20use%20their%20reliever%20inhaler%20more%20often."},
#elif (MSG_LANGUAGE_ZHTW)
  {"PM2.5%E6%BF%83%E5%BA%A6:%20","%20(%CE%BCg%2Fm3)%20(%E6%BF%83%E5%BA%A6%E4%BD%8E)%0A%0A%E4%B8%80%E8%88%AC%E6%B0%91%E7%9C%BE%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%E6%AD%A3%E5%B8%B8%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82%0A%E6%95%8F%E6%84%9F%E6%80%A7%E6%97%8F%E7%BE%A4%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%E6%AD%A3%E5%B8%B8%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82"},
  {"PM2.5%E6%BF%83%E5%BA%A6:%20","%20(%CE%BCg%2Fm3)%20(%E6%BF%83%E5%BA%A6%E4%B8%AD)%0A%0A%E4%B8%80%E8%88%AC%E6%B0%91%E7%9C%BE%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%E6%AD%A3%E5%B8%B8%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82%0A%E6%95%8F%E6%84%9F%E6%80%A7%E6%97%8F%E7%BE%A4%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%E6%9C%89%E5%BF%83%E8%87%9F%E3%80%81%E5%91%BC%E5%90%B8%E9%81%93%E5%8F%8A%E5%BF%83%E8%A1%80%E7%AE%A1%E7%96%BE%E7%97%85%E7%9A%84%E6%88%90%E4%BA%BA%E8%88%87%E5%AD%A9%E7%AB%A5%E6%84%9F%E5%8F%97%E5%88%B0%E7%99%A5%E7%8B%80%E6%99%82%EF%BC%8C%E6%87%89%E8%80%83%E6%85%AE%E6%B8%9B%E5%B0%91%E9%AB%94%E5%8A%9B%E6%B6%88%E8%80%97%EF%BC%8C%E7%89%B9%E5%88%A5%E6%98%AF%E6%B8%9B%E5%B0%91%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82"},
  {"PM2.5%E6%BF%83%E5%BA%A6:%20","%20(%CE%BCg%2Fm3)%20(%E6%BF%83%E5%BA%A6%E9%AB%98)%0A%0A%E4%B8%80%E8%88%AC%E6%B0%91%E7%9C%BE%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%E4%BB%BB%E4%BD%95%E4%BA%BA%E5%A6%82%E6%9E%9C%E6%9C%89%E4%B8%8D%E9%81%A9%EF%BC%8C%E5%A6%82%E7%9C%BC%E7%97%9B%EF%BC%8C%E5%92%B3%E5%97%BD%E6%88%96%E5%96%89%E5%9A%A8%E7%97%9B%E7%AD%89%EF%BC%8C%E6%87%89%E8%A9%B2%E8%80%83%E6%85%AE%E6%B8%9B%E5%B0%91%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82%0A%E6%95%8F%E6%84%9F%E6%80%A7%E6%97%8F%E7%BE%A4%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%0A%E3%80%80%E3%80%801.%20%E6%9C%89%E5%BF%83%E8%87%9F%E3%80%81%E5%91%BC%E5%90%B8%E9%81%93%E5%8F%8A%E5%BF%83%E8%A1%80%E7%AE%A1%E7%96%BE%E7%97%85%E7%9A%84%E6%88%90%E4%BA%BA%E8%88%87%E5%AD%A9%E7%AB%A5%EF%BC%8C%E6%87%89%E6%B8%9B%E5%B0%91%E9%AB%94%E5%8A%9B%E6%B6%88%E8%80%97%EF%BC%8C%E7%89%B9%E5%88%A5%E6%98%AF%E6%B8%9B%E5%B0%91%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82%0A%E3%80%80%E3%80%802.%20%E8%80%81%E5%B9%B4%E4%BA%BA%E6%87%89%E6%B8%9B%E5%B0%91%E9%AB%94%E5%8A%9B%E6%B6%88%E8%80%97%E3%80%82%20%0A%E3%80%80%E3%80%803.%20%E5%85%B7%E6%9C%89%E6%B0%A3%E5%96%98%E7%9A%84%E4%BA%BA%E5%8F%AF%E8%83%BD%E9%9C%80%E5%A2%9E%E5%8A%A0%E4%BD%BF%E7%94%A8%E5%90%B8%E5%85%A5%E5%8A%91%E7%9A%84%E9%A0%BB%E7%8E%87%E3%80%82"},
  {"PM2.5%E6%BF%83%E5%BA%A6:%20","%20(%CE%BCg%2Fm3)%20(%E6%BF%83%E5%BA%A6%E9%9D%9E%E5%B8%B8%E9%AB%98)%0A%0A%E4%B8%80%E8%88%AC%E6%B0%91%E7%9C%BE%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%E4%BB%BB%E4%BD%95%E4%BA%BA%E5%A6%82%E6%9E%9C%E6%9C%89%E4%B8%8D%E9%81%A9%EF%BC%8C%E5%A6%82%E7%9C%BC%E7%97%9B%EF%BC%8C%E5%92%B3%E5%97%BD%E6%88%96%E5%96%89%E5%9A%A8%E7%97%9B%E7%AD%89%EF%BC%8C%E6%87%89%E6%B8%9B%E5%B0%91%E9%AB%94%E5%8A%9B%E6%B6%88%E8%80%97%EF%BC%8C%E7%89%B9%E5%88%A5%E6%98%AF%E6%B8%9B%E5%B0%91%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82%0A%E6%95%8F%E6%84%9F%E6%80%A7%E6%97%8F%E7%BE%A4%E6%B4%BB%E5%8B%95%E5%BB%BA%E8%AD%B0%3A%0A%E3%80%80%E3%80%801.%20%E6%9C%89%E5%BF%83%E8%87%9F%E3%80%81%E5%91%BC%E5%90%B8%E9%81%93%E5%8F%8A%E5%BF%83%E8%A1%80%E7%AE%A1%E7%96%BE%E7%97%85%E7%9A%84%E6%88%90%E4%BA%BA%E8%88%87%E5%AD%A9%E7%AB%A5%EF%BC%8C%E4%BB%A5%E5%8F%8A%E8%80%81%E5%B9%B4%E4%BA%BA%E6%87%89%E9%81%BF%E5%85%8D%E9%AB%94%E5%8A%9B%E6%B6%88%E8%80%97%EF%BC%8C%E7%89%B9%E5%88%A5%E6%98%AF%E9%81%BF%E5%85%8D%E6%88%B6%E5%A4%96%E6%B4%BB%E5%8B%95%E3%80%82%0A%E3%80%80%E3%80%802.%20%E5%85%B7%E6%9C%89%E6%B0%A3%E5%96%98%E7%9A%84%E4%BA%BA%E5%8F%AF%E8%83%BD%E9%9C%80%E5%A2%9E%E5%8A%A0%E4%BD%BF%E7%94%A8%E5%90%B8%E5%85%A5%E5%8A%91%E7%9A%84%E9%A0%BB%E7%8E%87%E3%80%82"},
#endif
};

char message_buf[2048];

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 test!");

  dht.begin();

  // attempt to connect to Wifi network:
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected to wifi");
}

void loop() {
   delay(2000);
  float humid = dht.readHumidity();
    if (isnan(humid)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.println(" %");
  ////
  Serial.print("Humidity: ");
  Serial.println(humid);
  if (need_publish_on_feed(humid)) {
    Serial.println("Try to update humid value");
    int humidlevel = get_humid_level(humid);
    char *humidmsg_part1 = humid_message_template[humidlevel].msg_part1;
    char *humidmsg_part2 = humid_message_template[humidlevel].msg_part2;
  
    while (1) {
      if (!client.connect(server, 443)) {
        Serial.println("Connect to server failed. Retry after 1s.");
        client.stop();
        delay(1000);
        continue;
      }
  
      Serial.println("connected to server");
      // Make a HTTP request:
      sprintf(message_buf, "POST /v2.8/%s/feed?access_token=%s&message=%s%d%s\r\nHost: %s\r\n\r\n",
        FEED_ID,
        access_token,
        humidmsg_part1,
        humid,
        humidmsg_part2,
        server
      );
      client.write(message_buf);
  
      while (!client.available()) delay(100);
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      client.stop();
      Serial.println("Update humidity value done");
      break;
    }
  }
  delay(2 * 1000);
}

/*float humidavg = -1;
int get_humid_aircondition() {
//  int humid = pms.get_pm2p5_air();
  if (humidavg > 0) {
    // moving average
    humidavg = humidavg * 0.9 + humid * 0.1;
  } else {
    humidavg = humid;
  }
  return humidavg;
}*/

int get_humid_level(int humid) {
  if (humid < 36)
    return 0;
  else if (humid < 54)
    return 1;
  else if (humid < 71)
    return 2;
  else
    return 3;
}

int last_humid_value = -100;
uint32_t last_update_time = 0;
bool need_publish_on_feed(int humid) {
  if ( (abs(humid - last_humid_value) >= 10) || (millis() - last_update_time > 60 * 60 * 1000) ) {
    last_humid_value = humid;
    last_update_time = millis();
    return true;
  } else {
    return false;
  }
}
