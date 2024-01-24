/*#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Crypto.h>
#include <SHA256.h>
#include <AES.h>


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

  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");
}

void readAllBlocks(uint8_t uidLength, uint8_t uid[]) {
  // Read all blocks
  for (uint8_t block = 0; block < 64; block++) {
    uint8_t data[16];
    uint8_t success;

    if (uidLength == 4) {
      // Authenticate with default key for Mifare Classic
      uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 0, 0, key);
    } else if (uidLength == 7) {
      success = true; // No authentication needed for Mifare Ultralight
    } else {
      Serial.println("Unknown card type");
      return;
    }

    if (success) {
      success = nfc.mifareclassic_ReadDataBlock(block, data);

      if (success) {
        Serial.print("Block ");
        Serial.print(block);
        Serial.print(": ");
        nfc.PrintHexChar(data, 16);
        Serial.println("");
      } else {
        Serial.print("Failed to read Block ");
        Serial.println(block);
        break; // Stop reading if a block fails
      }
    } else {
      Serial.println("Authentication failed");
      break; // Stop reading if authentication fails
    }
  }
}

uint32_t multMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 0;
    uint32_t dblVal = a % m;
    uint32_t newB = b;

    // This is the result of working through the worksheet.
    // Notice the extreme similarity with powmod.
    while (newB > 0) {
        if (newB & 1) {
            result = (result + dblVal) % m;
        }
        dblVal = (dblVal << 1) % m;
        newB = (newB >> 1);
    }

    return result;
}

uint32_t powMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 1 % m;
    uint32_t sqrVal = a % m;  // stores a^{2^i} values, initially 2^{2^0}
    uint32_t newB = b;

    // See the lecture notes for a description of why this works.
    while (newB > 0) {
        if (newB & 1) {  // evalutates to true iff i'th bit of b is 1 in the i'th iteration
            result = multMod(result, sqrVal, m);
        }
        sqrVal = multMod(sqrVal, sqrVal, m);
        newB = (newB >> 1);
    }

    return result;
}

char decrypt(uint32_t x, uint32_t d, uint32_t n) {
    return (char) powMod(x, d, n);
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // Récupération clé publique
    byte key[16];
    nfc.mifareclassic_ReadDataBlock(3, key);

    // Déchiffrement données
    byte data[16];
    nfc.mifareclassic_ReadDataBlock(4, data);

    // Utilisation de la fonction decrypt
    char decryptedChar = decrypt(data[0], key[0], 256);  // Exemple de valeurs à ajuster en fonction de votre application

    // Affichage données déchiffrées
    Serial.println("Données déchiffrées:");
    Serial.print("Caractère : ");
    Serial.println(decryptedChar);

    Serial.println("Données chiffrées:");
    nfc.PrintHexChar(data, 16);

    Serial.println("Clé publique:");
    nfc.PrintHexChar(key, 16);

    // Stop reading for a moment
    delay(1000);
  }
}*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Crypto.h>
#include <SHA256.h>
#include <AES.h>

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

  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");
}

void readAllBlocks(uint8_t uidLength, uint8_t uid[]) {
  // Read all blocks
  for (uint8_t block = 0; block < 64; block++) {
    uint8_t data[16];
    uint8_t success;

    if (uidLength == 4) {
      // Authenticate with default key for Mifare Classic
      uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 0, 0, key);
    } else if (uidLength == 7) {
      success = true; // No authentication needed for Mifare Ultralight
    } else {
      Serial.println("Unknown card type");
      return;
    }

    if (success) {
      success = nfc.mifareclassic_ReadDataBlock(block, data);

      if (success) {
        Serial.print("Block ");
        Serial.print(block);
        Serial.print(": ");
        nfc.PrintHexChar(data, 16);
        Serial.println("");

        // Récupérer la clé publique depuis les données lues
        CryptoKey publicKey;
        memcpy(publicKey.asByteArray, data, sizeof(publicKey));

        // Déchiffrer les données avec la clé publique RSA
        byte encryptedData[256 / 8];
        nfc.mifareclassic_ReadDataBlock(block + 1, encryptedData);  // Supposons que les données chiffrées sont dans le bloc suivant

        byte decryptedData[256 / 8];
        Crypto.decryptRSA(publicKey, encryptedData, sizeof(encryptedData), decryptedData);

        Serial.print("Données déchiffrées : ");
        nfc.PrintHexChar(decryptedData, 16);
        Serial.println("");
      } else {
        Serial.print("Failed to read Block ");
        Serial.println(block);
        break; // Stop reading if a block fails
      }
    } else {
      Serial.println("Authentication failed");
      break; // Stop reading if authentication fails
    }
  }
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // Lire et traiter tous les blocs
    readAllBlocks(uidLength, uid);

    // Arrêter la lecture pendant un moment
    delay(1000);
  }
}

