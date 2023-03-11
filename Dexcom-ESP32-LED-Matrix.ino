#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// LED Config
#define PIN 5  // Data pin for LED Matrix
// Change the pixel count for your board, this is using 8x32
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                                               NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)
};


// Wifi Config
const char* ssid = "YOUR WIFI SSID";
const char* password = "YOUR WIFI PASSWORD";

// Decom URL - US use share1
const char* host = "https://shareous1.dexcom.com/ShareWebServices/Services/General/LoginPublisherAccountByName";

String authkey;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  HTTPClient https;
  String url = host;
  https.begin(url);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("User-Agent", "Dexcom Share/3.0.2.11 CFNetwork/711.2.23 Darwin/14.0.0");
  https.addHeader("Accept", "application/json");

  // Credentials
  String json = "{\"accountName\":\"PUTYOURUSERNAMEHERE\",\"applicationId\":\"d89443d2-327c-4a6f-89e5-496bbb0317db\",\"password\":\"PUTYOURPASSWORDHERE\"}";
  int httpCode = https.POST(json);

  String payload = https.getString();
  Serial.println(httpCode);
  Serial.println(payload);
  authkey = payload;
  authkey = authkey.substring(1, authkey.length() - 1);
  https.end();
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextColor(colors[0]);
}
int x = matrix.width();
int pass = 0;
void loop() {
  // Build Dexcom URL
  String dexcomreadingurl = "https://shareous1.dexcom.com/ShareWebServices/Services/Publisher/ReadPublisherLatestGlucoseValues?sessionId=" + authkey + "&minutes=1440&maxCount=1";
  HTTPClient https;
  // Add Headers
  https.begin(dexcomreadingurl);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("User-Agent", "Dexcom Share/3.0.2.11 CFNetwork/711.2.23 Darwin/14.0.0");
  https.addHeader("Accept", "application/json");
  // Make reuqest and get readings
  int httpCode = https.GET();
  String payload1 = https.getString();
  Serial.println(httpCode);
  Serial.println(payload1);

  payload1 = payload1.substring(1, payload1.length() - 1);
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload1);
  // You can now access the values in the JSON object like this:
  String DexcomDate = doc["ST"];
  float DexcomValue = doc["Value"];
  float FormattedDex = DexcomValue / 18;
  String DexcomTrend = doc["Trend"];
  float Testing = (FormattedDex, 1);
  Serial.println(DexcomDate);
  //  Serial.println(DexcomValue);
  Serial.println(FormattedDex, 1);
  Serial.println(DexcomTrend);
  Serial.println(Testing);
  matrix.fillScreen(0);  //Turn off all the LEDs
  matrix.setCursor(1, 1);
  matrix.print(FormattedDex, 1);
  // Displays an arrow to show the direction of Glucose
  // Flat = Green arrow horizonatal
  // fortyFiveUp = Yellow angled arrow pointing up
  // singleUp = Yellow arrow pointing straight up
  // doubleUp = Red arrow pointing straight up
  // fortyFiveDown = Yellow angled arrow pointing down
  // singleDown = Yellow arrow pointing straight down
  // doubleDown = Red arrow pointing straight down
  if (DexcomTrend == "Flat" || DexcomTrend == "flat") {
    matrix.drawPixel(29, 2, matrix.Color(0, 255, 0));
    matrix.drawPixel(30, 3, matrix.Color(0, 255, 0));
    matrix.drawPixel(31, 4, matrix.Color(0, 255, 0));
    matrix.drawPixel(30, 4, matrix.Color(0, 255, 0));
    matrix.drawPixel(29, 4, matrix.Color(0, 255, 0));
    matrix.drawPixel(28, 4, matrix.Color(0, 255, 0));
    matrix.drawPixel(27, 4, matrix.Color(0, 255, 0));
    matrix.drawPixel(26, 4, matrix.Color(0, 255, 0));
    matrix.drawPixel(30, 5, matrix.Color(0, 255, 0));
    matrix.drawPixel(29, 6, matrix.Color(0, 255, 0));
  }
  if (DexcomTrend == "fortyFiveUp" || DexcomTrend == "FortyFiveUp") {
    matrix.drawPixel(30, 1, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 2, matrix.Color(255, 255, 0));
    ;
    matrix.drawPixel(30, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 1, matrix.Color(255, 255, 0));
    matrix.drawPixel(28, 1, matrix.Color(255, 255, 0));
    matrix.drawPixel(27, 1, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 2, matrix.Color(255, 255, 0));
    matrix.drawPixel(28, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(27, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(26, 5, matrix.Color(255, 255, 0));
  }
  if (DexcomTrend == "SingleUp" || DexcomTrend == "singleUp") {
    Serial.println("if statement weorks");
    matrix.drawPixel(29, 1, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 2, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 5, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 6, matrix.Color(255, 255, 0));
    matrix.drawPixel(28, 2, matrix.Color(255, 255, 0));
    matrix.drawPixel(27, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 2, matrix.Color(255, 255, 0));
    matrix.drawPixel(31, 3, matrix.Color(255, 255, 0));
  }
  if (DexcomTrend == "doubleUp" || DexcomTrend == "DoubleUp") {
    matrix.drawPixel(29, 1, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 2, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 3, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 4, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 5, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 6, matrix.Color(255, 0, 0));
    matrix.drawPixel(28, 2, matrix.Color(255, 0, 0));
    matrix.drawPixel(27, 3, matrix.Color(255, 0, 0));
    matrix.drawPixel(30, 2, matrix.Color(255, 0, 0));
    matrix.drawPixel(31, 3, matrix.Color(255, 0, 0));
  }
  if (DexcomTrend == "fortyFiveDown" || DexcomTrend == "FortyFiveDown") {
    matrix.drawPixel(30, 6, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 5, matrix.Color(255, 255, 0));
    matrix.drawPixel(28, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(27, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(26, 2, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 5, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 6, matrix.Color(255, 255, 0));
    matrix.drawPixel(28, 6, matrix.Color(255, 255, 0));
    matrix.drawPixel(27, 6, matrix.Color(255, 255, 0));
  }
  if (DexcomTrend == "singleDown" || DexcomTrend == "SingleDown") {
    matrix.drawPixel(29, 1, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 2, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 3, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 5, matrix.Color(255, 255, 0));
    matrix.drawPixel(29, 6, matrix.Color(255, 255, 0));
    matrix.drawPixel(28, 5, matrix.Color(255, 255, 0));
    matrix.drawPixel(27, 4, matrix.Color(255, 255, 0));
    matrix.drawPixel(30, 5, matrix.Color(255, 255, 0));
    matrix.drawPixel(31, 4, matrix.Color(255, 255, 0));
  }
  if (DexcomTrend == "doubleDown" || DexcomTrend == "DoubleDown") {
    matrix.drawPixel(29, 1, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 2, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 3, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 4, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 5, matrix.Color(255, 0, 0));
    matrix.drawPixel(29, 6, matrix.Color(255, 0, 0));
    matrix.drawPixel(28, 5, matrix.Color(255, 0, 0));
    matrix.drawPixel(27, 4, matrix.Color(255, 0, 0));
    matrix.drawPixel(30, 5, matrix.Color(255, 0, 0));
    matrix.drawPixel(31, 4, matrix.Color(255, 0, 0));
  }

  matrix.show();
  //https.end();

  delay(100000);
}
