#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  13
#define PN532_MOSI 11
#define PN532_SS   10
#define PN532_MISO 12

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void setup(void) {
  Serial.begin(115200);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  /*Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");*/
}

void readBlocks(uint8_t uidLength, uint8_t uid[], uint8_t block, uint8_t* data) {
  uint8_t success;

  if (uidLength == 4) {
    // Authenticate with default key for Mifare Classic
    uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, block, 0, key);
  } else if (uidLength == 7) {
    success = true; // No authentication needed for Mifare Ultralight
  } else {
    Serial.println("Unknown card type");
    return;
  }

  if (success) {
    success = nfc.mifareclassic_ReadDataBlock(block, data);

    if (success) {
      /*Serial.print("Block ");
      Serial.print(block);
      Serial.print(": ");
      nfc.PrintHexChar(data, 16);
      Serial.println("");*/
    } else {
      Serial.print("Failed to read Block ");
      Serial.println(block);
    }
  } else {
    Serial.println("Authentication failed");
  }
}

void writeData(uint8_t uidLength, uint8_t uid[]) {
  uint8_t success;

  // Authenticate with default key for Mifare Classic
  uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  //bloc 1
  success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 1, 0, key);

  if (success) {
    success = nfc.mifareclassic_WriteDataBlock(1, "5137708111347203");

    if (success) {
      //Serial.println("Data written successfully.");
    } else {
      Serial.println("Failed to write data.");
    }

    // Re-authenticate to make sure the write was successful
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 1, 0, key);
    if (!success) {
      Serial.println("Re-authentication failed.");
    }
  } else {
    Serial.println("Authentication failed");
  }

  //bloc 2 

  success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 2, 0, key);

  if (success) {
    success = nfc.mifareclassic_WriteDataBlock(2, "12/20");

    if (success) {
      //Serial.println("Data written successfully.");
    } else {
      Serial.println("Failed to write data.");
    }

    // Re-authenticate to make sure the write was successful
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 2, 0, key);
    if (!success) {
      Serial.println("Re-authentication failed.");
    }
  } else {
    Serial.println("Authentication failed");
  }
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    /*Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");*/

    writeData(uidLength, uid);

    uint8_t num_card[16];
    uint8_t date[5];

    readBlocks(uidLength, uid, 1, num_card);
    readBlocks(uidLength, uid, 2, date);

    // Convertir le tableau d'octets en une chaîne de caractères
    String num_card_str = "";
    for (int i = 0; i < sizeof(num_card); ++i) {
      num_card_str += char(num_card[i]);
    }

    
    // Convertir le tableau d'octets en une chaîne de caractères
    String date_str = "";
    for (int i = 0; i < sizeof(date); ++i) {
      date_str += char(date[i]);
    }

    // Afficher la chaîne de caractères
    Serial.println(num_card_str + " " + date_str);

    /*// Inverser l'ordre des octets dans le tableau
    for (int i = 0; i < sizeof(num_card) / 2; ++i) {
      uint8_t temp = num_card[i];
      num_card[i] = num_card[sizeof(num_card) - i - 1];
      num_card[sizeof(num_card) - i - 1] = temp;
    }

    // Convertir le tableau hexadécimal inversé en décimal
    long num_card_dec = 0;
    for (int i = 0; i < sizeof(num_card); ++i) {
      num_card_dec = (num_card_dec << 8) | num_card[i];
    }

    // Convertir la valeur décimale en chaîne de caractères
    String num_card_str = String(num_card_dec, DEC);

  
  Serial.print("Valeur finale : ");
  Serial.println(num_card);*/

    // Stop reading for a moment
    delay(1000);
  }
}
