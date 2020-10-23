# babble

 Projet de sculpture électronique. Production de fréquences aléatoires dans la bande audio-fréquences par utilisation d'un OU EXCUSIF entre 2 radio-fréquences. Cette idée est inspirée du [thérémine](https://fr.wikipedia.org/wiki/Th%C3%A9r%C3%A9mine). 
La *tour de babble* est sensible au interférences électromagnétique. Lorsqu'une électromagnétique suffisamment puissante pour charger **C6** à niveau suffisant pour être détecté par **RA0** comme un changmenent de niveau babble se met à babiller pendant
environ 30 secondes.   

Ce petit circuit illustre une utilisation du périphique CLC du PIC10F322.

## schématique

![babble](babble.png)

Le circuit peut-être alimenté avec un voltage entre 3 et 5 volts.

## Principe de fonctionnement.

2 fréquences de 125Khz avec un rapport cyclique de 50% sont générées. La première est générée par le **PWM1 canal 1** et la deuxième par le **NCO**. La sortie des 2 générateurs est envoyé vers la **CLC** qui est configurée en **OU EXCLUSIF**. L'utilisation d'un **OU EXCLUSIF** sur ces signaux a pour effet de moduler le rapport cyclique selon la différence de fréquence entre les 2 signaux selon une progression triangulaire. Si on applique un filtre passe-bas sur la sortie on obtient une onde triangulaire de fréquence = Fnco-Fpwm.  Puisque les 2 fréquences utilisées sont dans les radio-fréquences, soit environ 125Khz on peut se passer du filtre passe-bas et connecter directement à travers un condenstateur au haut-parleur. 

![principe](principe.png)

La fréquence du **NCO** est modifiée de manière aléatoire pour produire une succession rapide de fréquences audio par battement avec la fréquence du PWM, d'où le nom de **jacasseux** (*jacasseur*).

## générateur aléatoire.

Comme générateur aléatoire, le circuit utilise la mesure du temps de charge du condensateur **C2** à travers la résistance *pullup* de l'entrée **RA2**. Le cycle est le suivant. **RA2** en configurée en entrée avec pullup ce qui permet de charger **C2** vers Vdd. Le **TIMER0** est initialisée au début du cycle. Une lecture répéritive de l'entrée **PORTA:2** est effectuée jusqu'à ce que la lecture arrive à **1**. À ce moment ont configure **RA2** en mode sortie à 0 volt pour décharger **C2** afin qu'il soit prèt pour le prochain cycle. Pendant la charge de **C2** vers **Vdd** le **TIMER0** incrémente. À la fin du cycle de charge la lecture du registre **TMR0** est utilisée pour modifier la valeur de l'incrément du **NCO**. Les paramètres sont choisient pour que la différence de fréquence entre les générateurs **PWM1_CH1** et **NCO** demeure dans l'intervalle des fréquences audibles.

Avec un condensateur de **100nF** pour **C2** il y a suffisamment de variabilitée entre les lectures pour produire l'effet aléatoire désiré.

# Babble

