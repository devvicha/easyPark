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

String id = "universityparking_6975403446446829_7991555124521255";

String documentPath = "parking-spaces/"+id;

const int trigPin1 = 5; // Trigger pin for sensor 1
const int echoPin1 = 14; // Echo pin for sensor 1
const int trigPin2 = 4; // Trigger pin for sensor 2
const int echoPin2 = 12; // Echo pin for sensor 2
const int trigPin3 = 0; // Trigger pin for sensor 3
const int echoPin3 = 13; // Echo pin for sensor 3
const int trigPin4 = 2; // Trigger pin for sensor 4
const int echoPin4 = 15; // Echo pin for sensor 4

int s1 = LOW;
int s2 = LOW;
int s3 = LOW;
int s4 = LOW;
int x;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

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
  // Read from sensor 1
  long duration1, distance1;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Read from sensor 2
  long duration2, distance2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Read from sensor 3
  long duration3, distance3;
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;

  // Read from sensor 4
  long duration4, distance4;
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin4, LOW);
  duration4 = pulseIn(echoPin4, HIGH);
  distance4 = duration4 * 0.034 / 2;

  // Reset counter
  x = 0;

  // Count the number of sensors detecting something
  if(distance1 < 20) {
    x++;
  }
  if(distance2 < 20) {
    x++;
  }
  if(distance3 < 20) {
    x++;
  }
  if(distance4 < 20) {
    x++;
  }

  // Output readings to serial monitor
/*  Serial.print("Sensor 1: ");
  Serial.print(distance1);
  Serial.print(" cm | Sensor 2: ");
  Serial.print(distance2);
  Serial.print(" cm | Sensor 3: ");
  Serial.print(distance3);
  Serial.print(" cm | Sensor 4: ");
  Serial.print(distance4);*/
  Serial.print(" cm | Number of active sensors: ");
  Serial.println(x);
  content.set("fields/freeSlots/integerValue", x);
      if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), "freeSlots" /* updateMask */))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      else
        Serial.println(fbdo.errorReason());

  delay(500); // Adjust delay as needed for your application
}