#include <Firebase_ESP_Client.h>
#include <Servo.h>


// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "home@129"
#define WIFI_PASSWORD "home@1234+"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCYnPqjbkx0vgjmRlsnNPid2lYXXOgRdAE"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "easypark-cc6dd"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "admin@admin.com"
#define USER_PASSWORD "123456"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson content;

Servo myservo;
Servo myservo2;


int gate_time = 5000;

void setup() {
  Serial.begin(115200);
  myservo.attach(5);
  myservo2.attach(4);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
}

void loop() {
  String documentPath = "qr-scan/qr";

  //Serial.print("Get a document... ");

  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str())) {
    FirebaseJson payload;
    //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());

    payload.setJsonData(fbdo.payload().c_str());
    FirebaseJsonData JsonData;
    FirebaseJsonData JsonData2;

    payload.get(JsonData, "fields/isScannedIn/stringValue", true);
    payload.get(JsonData2, "fields/isScannedOut/stringValue", true);




    String value = JsonData.stringValue;
    String value2 = JsonData2.stringValue;

    Serial.println(value2);


    if (value == "true") {
      myservo.write(0);
      delay(gate_time);
      content.set("fields/isScannedIn/stringValue", "false");
      if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), "isScannedIn" /* updateMask */))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      else
        Serial.println(fbdo.errorReason());
    } else if (value == "false") {
      myservo.write(180);
    }

    if (value2 == "true") {
      myservo2.write(0);
      delay(gate_time);
      content.set("fields/isScannedOut/stringValue", "false");
      if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), "isScannedOut" /* updateMask */))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      else
        Serial.println(fbdo.errorReason());
    } else if (value2 == "false") {
      myservo2.write(180);
    }




  } else {
    Serial.println(fbdo.errorReason());
  }
}
