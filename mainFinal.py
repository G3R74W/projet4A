import serial
import pyautogui
import keyboard

Ser = serial.Serial('COM3', 115200)

while Ser.isOpen():
    # Lire la commande depuis l'Arduino
    Command = Ser.readline().strip().decode('utf-8')

    if Command == "RUN_SCRIPT":
        print("data received")
        Data = Ser.read(22).decode('utf-8')#On recoit les donnees sous la forme "Chifres Mois/Annee"
        print("\n " + Data)
        DataList = Data.split(" ")#On separe pour avoir 2 chaines une "Chiffre" et une "Mois/Annee"
        # Parcours de chaque caractère dans la chaîne
        keyboard.wait("enter")  # Attend que la touche "Entrée" soit enfoncée
        for DataBloc in DataList:
            for DataUnit in DataBloc:
                if DataUnit != "/": #on fait ca pour ne pas avoir de probleme avec Mois/Annee
                    print(DataUnit + "\n")
                    pyautogui.press(DataUnit)
            keyboard.wait("enter")
        pyautogui.press("enter")
    # pour fermer le script une fois les données envoyées
    elif Command == "end":
        print("end")
        Ser.close()
