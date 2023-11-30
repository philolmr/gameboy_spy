#include <Keypad.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>  
#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C
#define SSD1306_BLACK 0
#define SSD1306_WHITE 1

Adafruit_SSD1306 display(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

#define ROWS 3
#define COLS 3

byte rowPins[ROWS] = {20, 10, 0};
byte colPins[COLS] = {7, 8, 9};

char keys[ROWS][COLS] = {
  {'U', 'L', 'D'},
  {'R', 'SL', 'S'},
  {'A', 'B', ' '}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String phrase[] = {"comment \x82tait votre blanquette ?", "la blanquette est bonne", "je suis bourr\x82 ", "Je rentre \x85 la coloc", "\x85 l'aide, viens m'aider ","skipilipop yes yes skupulupi skupulupa","et ca fait crik crik crik pan pan pan"};

int selectedIndex = 0;  // Index de l'élément sélectionné
int scrollPosition = 0;  // Position actuelle pour le défilement
unsigned long lastScrollTime = 0;  // Dernière mise à jour du défilement



#include <WiFi.h>
#include <WiFiUdp.h>

// Paramètres du point d'accès WiFi
const char* ssid = "ESP32-AP";
const char* password = "12345678";

// Paramètres UDP
WiFiUDP udp;
unsigned int localUdpPort = 4210;  // Port UDP local
IPAddress remoteIp(192, 168, 4, 2); // IP de la carte en mode STA

// Enum pour les modes
enum Mode {EMETTEUR, RECEPTEUR};
Mode mode = RECEPTEUR; // Changer pour RECEPTEUR selon la carte

bool isConnected = false;

void sendMessage(String message) {
  udp.beginPacket(remoteIp, localUdpPort);
  udp.write((const uint8_t*)message.c_str(), message.length()); // Utiliser le cast et spécifier la longueur
  udp.endPacket();
}

// Fonction pour recevoir des messages UDP
String receiveMessage() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255]; // Taille maximale du paquet
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0; // Null-terminer la chaîne
    }
    return String(incomingPacket);
  }
  return "";
}
void setup() {
  Serial.begin(115200);

  if (mode == EMETTEUR) {
    // Mode EMETTEUR: Configurer comme un point d'accès
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré");
    Serial.print("IP du point d'accès: ");
    Serial.println(WiFi.softAPIP());
  } else {
    // Mode RECEPTEUR: Se connecter au point d'accès
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connecté au point d'accès");
    isConnected = true;
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) {
    Serial.println(F("Erreur lors de l'initialisation de l'écran OLED"));
    for (;;);
  }
  display.clearDisplay();
  display.display();


  // Initialisation UDP
  udp.begin(localUdpPort);
  Serial.printf("UDP à l'écoute sur le port %d\n", localUdpPort);
}

void scrollText(String text, int yPosition) {
  if (millis() - lastScrollTime > 300) {
    lastScrollTime = millis();
    scrollPosition++;
    if (scrollPosition > text.length() + 20) scrollPosition = 0;
  }

  String visibleText = text.substring(scrollPosition) + "  " + text;
  visibleText = visibleText.substring(0, min((int)visibleText.length(), 20));


  display.setCursor(0, yPosition);
  display.println(visibleText);

}
bool isSomeoneConnected() {
  return (WiFi.softAPgetStationNum() > 0);
}
void loop() {
  if (isConnected && mode == RECEPTEUR  ||(isSomeoneConnected() && mode == EMETTEUR)){

  
  char key = keypad.getKey();
  if (key) {
    switch (key) {
      case 'U':
        if (selectedIndex > 0) selectedIndex--;
        scrollPosition = 0;
        break;
      case 'D':
        if (selectedIndex < (sizeof(phrase) / sizeof(phrase[0])) - 1) selectedIndex++;
        scrollPosition = 0;
        break;
      case 'L': // Lorsque la touche 'SL' est appuyée
        sendMessage(phrase[selectedIndex]); // Envoyer la phrase sélectionnée
        Serial.println(phrase[selectedIndex]);
        break;
    }
  }

  display.clearDisplay();

  int numPhrases = sizeof(phrase) / sizeof(phrase[0]);
  int phrasesToDisplay = min(numPhrases - selectedIndex, numPhrases);  // Nombre de phrases à afficher

  for (int i = 0; i < phrasesToDisplay; i++) {
    int displayIndex = selectedIndex + i;
    
    if (i == 0) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      scrollText(phrase[displayIndex], 10 * i);
    } else {
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0,2 + 10 * i);
      display.println(phrase[displayIndex]);
    }
  }
  String msg = receiveMessage();
  if (msg.length() > 0) {
    // Afficher le message reçu sur l'écran
    display.clearDisplay();
    display.setTextSize(1); // Ajuster la taille du texte selon les besoins
    display.setCursor(0,0);
    display.println(msg);
    delay(2000);
  }
  display.display();
  }

  else{
    Serial.println("nope");
    delay(1000);
  }
}
