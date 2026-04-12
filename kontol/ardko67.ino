#include <Firebase.h>
#include <Servo.h>
#include <ArduinoJson.h>
#define WIFI_SSID     "ADINATA"
#define WIFI_PASSWORD "ADINATA12311"
#define REFERENCE_URL "https://arduinokokogan-default-rtdb.asia-southeast1.firebasedatabase.app/"

Firebase fb(REFERENCE_URL);

// Firebase fb(REFERENCE_URL, AUTH_TOKEN);

Servo myservo;
int pos = 0;

long durasi;
float jarak;

void setup() {
    Serial.begin(115200);
    pinMode(D1, OUTPUT);//red
    pinMode(D2, OUTPUT);//green
    pinMode(D3, OUTPUT);//blue
    myservo.attach(D4);//servo
    // Set pin Trig sebagai Output dan Echo sebagai Input
    pinMode(D6, OUTPUT);//trig
    pinMode(D7, INPUT);//echo

    // Board-specific initialization
    #if !defined(ARDUINO_UNOWIFIR4)
        WiFi.mode(WIFI_STA);
    #else
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
    #endif
    
    WiFi.disconnect();
    delay(1000);

    /* Connect to WiFi */
    Serial.println();
    Serial.println();
    Serial.print("Menyambungkan ke: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("-");
        delay(500);
    }

    Serial.println();
    Serial.println("WiFi Tersambung");
    Serial.println();

    // Turn on built-in LED for UNO R4 WiFi
    #if defined(ARDUINO_UNOWIFIR4)
        digitalWrite(LED_BUILTIN, HIGH);
    #endif

    /* ===== JSON SERIALIZATION: CREATE AND SEND DATA ===== */
    
    Serial.println("Membuat Data JSON");

    // Create a JSON document to hold the output data
    StaticJsonDocument<255> leddoc;

    // Add various data types to the JSON document
    leddoc["merah"] = 0;
    leddoc["hijau"] = 0;
    leddoc["biru"] = 0;
    // Mengirim langsung karena hanya 1 data
    fb.setInt("servo/servo", 0);
    fb.setInt("jarak/jarak", 0);
  
    // Create a string to hold the serialized JSON data
    String ledjson;

    // Serialize the JSON document to a string
    serializeJson(leddoc, ledjson);
    Serial.println("Data JSON yang dibuat:");
    Serial.println(ledjson);
    Serial.println();

    // Set the serialized JSON data in Firebase
    Serial.println("Mengirim JSON ke Firebase");
    int resLED   = fb.setJson("LED", ledjson);

    if (resLED == 200 && resServo == 200 && resJarak == 200) {
        Serial.println("JSON berhasil dikirim!");
    } else {
        Serial.println("Ada JSON yang gagal dikirim!");
    }
    Serial.println();
}

void loop() {
  Serial.println("Mengambil data JSON dari Firebase");

  // Retrieve the serialized JSON data from Firebase
  String ledjson, servojson;
  int resLED = fb.getJson("LED", ledjson);
  int resServo = fb.getJson("servo", servojson);

  if (resLED == 200 && resServo == 200) {
      Serial.println("JSON berhasil diterima!");
  } else {
      Serial.println("Ada JSON yang gagal diterima!");
  }

  Serial.println("data JSON yang diterima:");
  Serial.println(ledjson);
  Serial.println(servojson);
  Serial.println();

  // Create a JSON document to hold the deserialized data
  StaticJsonDocument<255> docled, docservo;

  // Menguraikan Data JSON dalam bentuk String
  if (deserializeJson(docled, ledjson)) {
      Serial.println("LED JSON error");
      return;
  }

  if (deserializeJson(docservo, servojson)) {
      Serial.println("Servo JSON error");
      return;
  }

  Serial.println("Mengurai data JSON");

  // Mengambil Data dari JSON yang sudah di urai
  int LED1 = docled["merah"];
  int LED2 = docled["hijau"];  
  int LED3 = docled["biru"];
  pos = docservo["servo"]; 
  
  /* Menampilkan Data yang sudah di urai */
  Serial.println("---Data yang terurai---");
  Serial.print(" merah: ");
  Serial.println(LED1);
  Serial.print(" hijau: ");
  Serial.println(LED2);
  Serial.print(" biru: ");
  Serial.println(LED3);
  Serial.print("Posisi Servo: ");
  Serial.println(pos);
  
  // membaca data LED dari firebase
  Serial.println();
  digitalWrite(D1, LED1);
  digitalWrite(D2, LED2);
  digitalWrite(D3, LED3);
  // Membaca data posisi dari Firebase
  pos = constrain(pos, 0, 90);
  myservo.write(pos);
  // Bersihkan trigPin
  digitalWrite(D6, LOW);
  delayMicroseconds(2);
  digitalWrite(D6, HIGH);
  delayMicroseconds(10);
  digitalWrite(D6, LOW);

  durasi= pulseIn(D7, HIGH);
  jarak = durasi * 0.034 / 2; // Hitung cm

  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");
  fb.setInt("jarak/jarak", jarak);
  delay(5000);

}
