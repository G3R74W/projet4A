void setup() {
  Serial.begin(9600);  // Initialiser la communication série à 9600 bauds
}

void loop() {
  // Envoyer une commande à l'ordinateur
  Serial.println("RUN_SCRIPT");
  
  delay(1000);  // Attendre 1 secondes avant d'envoyer à nouveau la commande
  Serial.println("Test");
  Serial.println("Test2");
  delay(1000);
}



