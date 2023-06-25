#include <WiFi.h>
#include <FirebaseESP32.h>


#define LED 2

FirebaseData firebaseData;

#define FIREBASE_HOST "https://model-house-iot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ocL8ltSlCFTLliWSGQyVKHY2eMflRKkbxu7Ho58V"

unsigned long duration = 0;
int r, g, b;

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting with ");
  WiFi.begin("Wokwi-GUEST", "", 6);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Wifi conected, IP: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  duration = millis();
  pinMode(LED, OUTPUT);
}

void loop() {
  controlLed();
  sendTime();
}

void controlLed(){
  
  if (Firebase.getString(firebaseData, "/devices/luis/id")){
    String uuid = firebaseData.stringData();
    if (uuid != "4e72237c-ad3a-45d2-b2f4-1824b47e3c42"){
      return;
    }
  }

  if (Firebase.getString(firebaseData, "/devices/luis/LED")){
    String ledState = firebaseData.stringData();
    if (ledState == "1"){
      Serial.println(" ==> Led: 1 Action");
      // digitalWrite(LED, HIGH);
      // delay(500);
      if (Firebase.getString(firebaseData, "/devices/luis/brightness")){
        int firebaseBrightness = firebaseData.stringData().toInt();
        // int brightness = (int) map(firebaseBrightness, 0, 255, 0, 1023);
        analogWrite(LED, firebaseBrightness);
        delay(10);    // Pequeña pausa para estabilizar las lecturas
      }
      else{
        Serial.println("error");
      }
    }
    else {
      Serial.println(" ==> Led: 0 Action");
      analogWrite(LED, 0);
      // digitalWrite(LED, LOW);
    }
  }

}

void sendTime(){
  if (millis() - duration >= 60000){
    duration = millis();
    Serial.print("Minutes is ");
    Firebase.getInt(firebaseData, "/devices/luis/minutes");
    int minutes = firebaseData.intData();
    minutes++;
    Serial.println(minutes);
    Firebase.setInt(firebaseData, "/devices/luis/minutes", minutes);
  }
}