
# Dexcom ESP

The object of this project is to display Dexcom readings on an LED matrix. It uses an £SP32 and checks the Dexcom share enpoint for readings. It will auto update every "x" mins and display it on an always on LED matrix display.

I've only started workingo n this project so there is lots more to do.


## Bits to Change

### WiFi Details

Change the SSID and Password for your WiFi. Change this on **Line 22** and **Line 23**.

```C+
  // Wifi Config
    const char* ssid = "YOUR WIFI SSID";
    const char* password = "YOUR WIFI PASSWORD";
```

### Dexcom Share URL

There are 2 URLS that are used for the endpoints. 

US  - https://share1.dexcom.com

UK / Europe - https://shareous1.dexcom.com

There are 2 places within the code that will need to be changed depending on where the Dexcom account is. **Line 26** and **Line 64**.

### Dexcom Credentials

In order for this  work, you will need to use your Clarity Credentials. Change this on **Line 46**.

```C+
  // Credentials 
  String json = "{\"accountName\":\"PUTYOURUSERNAMEHERE\",\"applicationId\":\"d89443d2-327c-4a6f-89e5-496bbb0317db\",\"password\":\"PUTYOURPASSWORDHERE\"}";
  ```
## Bits to check

### Board PIN

This is coded to use `PIN 5` however if you would like to use another pin change **Line 11**.

```C+
  #define PIN 5 // Data pin for LED Matrix
```

### Matrix Size

This is configured to use a board size of 8x32. Currently this hasnt been tested on anything else, however this can be changed on **Line 13**.

```C+
  Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
```



## Acknowledgements

 - [Great information about the Dexcom EndPoints](https://gist.github.com/StephenBlackWasAlreadyTaken/adb0525344bedade1e25)

