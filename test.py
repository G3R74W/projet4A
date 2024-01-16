import pyautogui
import time
import keyboard



chaine = "Bonjour"

for i in range(0,10) :
    time.sleep(1)
    
    # Parcours de chaque caractère dans la chaîne
    keyboard.wait('enter')  # Attend que la touche "Entrée" soit enfoncée
    for caractere in chaine:
        print(caractere)

        pyautogui.press(caractere)
    pyautogui.press("enter")



