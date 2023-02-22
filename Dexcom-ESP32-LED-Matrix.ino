#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// LED Config
#define PIN 5 // Data pin for LED Matrix
// Change the pixel count for your board, this is using 8x32
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};


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
  authkey = authkey.substring(1, authkey.length()-1);
  https.end();
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextColor(colors[0]);
}
int x    = matrix.width();
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
	//Serial.println(httpCode);
	//Serial.println(payload1);

	payload1 = payload1.substring(1, payload1.length()-1);
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload1);
    String DexcomDate = doc["ST"];
	// Variable - Glucose value in mg/dL
    float DexcomValue = doc["Value"];
	// Connvert dexcom value to mmol/L
    float FormattedDex = DexcomValue / 18;
	// Variable - Show which direction the glucose is going in
    String DexcomTrend = doc["Trend"];
    float Testing = (FormattedDex,1);
    Serial.println(DexcomDate);
  //  Serial.println(DexcomValue);
    Serial.println(FormattedDex,1); 
    Serial.println(DexcomTrend);
    Serial.println(Testing);
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(2, 1);
	// Print Dexcom result to LED Matrix
    matrix.print(FormattedDex,1);
    matrix.setTextColor(colors[5]);

	matrix.show();
	//https.end();

	delay(100000);
}
