import serial
import subprocess
import pyautogui
import time
import keyboard


ser = serial.Serial('COM5', 9600)
condition = True
try:
    while condition:
        # Lire la commande depuis l'Arduino
        command = ser.readline().strip().decode('utf-8')

        if command == "RUN_SCRIPT":
            print("data received")
            command = ser.readline().decode('utf-8')
            # Parcours de chaque caractère dans la chaîne
            keyboard.wait('enter')  # Attend que la touche "Entrée" soit enfoncée
            for caractere in command:
                print(caractere)
                pyautogui.press(caractere)
            pyautogui.press("enter")
        #pour fermer le script une fois les données envoyées
        if command == "end" :
            print("end")
            condition = False

        
finally:
    # Fermer le port série à la fin de l'utilisation
    ser.close()       
        
