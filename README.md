# gameboy_spy
# Projet Interactif Arduino

Ce projet Arduino combine un écran OLED, un clavier matriciel et une communication WiFi UDP pour une expérience interactive. Le code est écrit en C++.

## Composants

- **Écran OLED :** Utilise la bibliothèque Adafruit_SSD1306 pour configurer et afficher des informations sur l'écran.
  
- **Clavier Matriciel :** Défini avec des broches spécifiques pour les lignes et les colonnes, et utilise un tableau de caractères pour représenter les touches.

- **Communication WiFi UDP :** Configure l'appareil en tant que point d'accès ou client, en utilisant des paramètres spécifiques pour la connexion.

## Initialisation

Le programme stocke des phrases prédéfinies dans un tableau et utilise des variables pour suivre la phrase actuellement sélectionnée, gérant le défilement du texte sur l'écran OLED.

## Configuration WiFi

Dans la fonction setup, le programme initialise la communication série, configure les paramètres WiFi selon le mode choisi (point d'accès ou client), et vérifie le bon fonctionnement de l'écran OLED. La communication UDP est également initialisée ici.

## Fonctions Principales

- **scrollText() :** Fait défiler le texte sur l'écran OLED.

- **isSomeoneConnected() :** Vérifie si un utilisateur est connecté au point d'accès WiFi.

## Utilisation

La fonction loop permet de faire défiler le texte lorsque celui-ci est trop long pour rentrer sur une seule ligne. Lorsqu'une phrase est sélectionnée, ses couleurs sont inversées. Lorsque l'utilisateur appuie sur "select", la phrase est envoyée au serveur et affichée en grand sur l'écran de l'autre utilisateur.

**Note :** En raison de problèmes matériels, le code peut présenter des retards et des problèmes, tels que la réception du message par les deux utilisateurs et l'affichage incorrect sur l'écran.
La vidéo de présentation n'est également pas finie car le prototype n'a pas pu être filmé.
