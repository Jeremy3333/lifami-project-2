# LIFAMI-PROJET

###### Jérémy SIMOND

Mon objectif est de faire un programme qui permet de faire une simulation de gravité entre planètes en 2D, ainsi qu'une sumilation de champ d'énergie avec une équation simplifiée

afin de plus facilement comprendre quel includes sont mes code ou des librerie publiques j'ai indiquer en commentaire en dessus et aussi les librairies publique ont leur nom entre <> et ce que j'ai codé moi est entre ""

je m'excuse pour l'anglais en commentaire.

---

## - Fonctionnement

### - Gravité

#### - pourquoi

J'ai créer une simulation de gravité car je me desais que c'était à ma porté et que je voulais faire une simulation "simple" avant d'en faire une plus compliquer afin d'être sur de rendre quelque chose.

#### - explication

J'ai d'abord créer deux structure une de planete et une de galaxy (décrie en commentaire) en suite quand la galaxie est initialisée elle initiallise ses planète qui elle initiallise leur traces quand le bouton bleu est appuié la galaxy est réinitialliser (mais pas la planète sélectionnée) à chaque tour de la boucle chaque gravité entre planète est calculée

![equation-gravité](data/gravityEquation.png?raw=true "gravté")

en suite avec ca on calcule la vélocité puis la nouvelle position. Après ça la nouvelle position est ajoutée dans le tableau de trace à l'index de la trace qui lui augmante de un. en suite les planète son dessinée sur le rendu (avec la position et le rayon) et les traces (un point a chaque position de la trace). Pour la decentralisation du dessin (quand je veux que le centre de l'écran soit une planète) j'ai utilisé une fonction avant de dessiné une planète (getDrawPosition dans utils.cpp)

### - Electromagnetisme

#### - pourquoi

Je voulais un défi plus complexe pour ma deuxième parti mais je n'ai eux l'idée que en tombant sur une vidéo parlant d'animation de cercle qui fusionne et montrant qu'il utilisait une équation simplifiée des chamlp électromagnetique mais aussi comment dessiné des équation (https://youtu.be/6oMZb3yP_H8)

#### - explication

tout d'abord pour le dessin des équations. mon draw calcule l'équation au point (x, y), (x + 1, y), (x, y + 1) et (x + 1, y + 1) puis si il y a un point inférieur à la réponse de l'équation (en l'occurance 1 mais j'expliquerais pourquoi sur une autre partie) on sais que la courbe passe entre ses deux point et avec une cascade de test on peux facilement differencier tout les cas possible et en suite pouvoir placer un point entre les deux de la manière la plus proche possible grace a une interpolation puis tiré des ligne entre ses point.

pour le calcule j'ai utiliser la fonction simplifiée

![equation particule simplifiée](data/electromagnetismEquation.png?raw=true "électromagnetisme")

pour chaque particule et comme ca calcule un vecteur pour les faire interagire entre eux il suffit de tous les additionné a chaque point (avant de se servire du résulta pour faire un rendu) et pour le 1 il s'agit juste d'un changement entre le rayon qui est normalement en résulta et le 1 qui est normalement en haut de la fraction afin de rendre la fraction plous simple a calculer on peux les echangé (car il s'agit d'un inequation). l'équation final est donc (avec x le nombre de particule)

![equation particule simplifiée somme](data/electromagnetismEquationSomme.png?raw=true "électromagnetisme somme")

---

## - Visualisation

![screen01](Capture01.PNG?raw=true "screen 01")
![screen02](Capture02.PNG?raw=true "screen 02")

---

## - Téléchargement

Vous pouver trouver une version a jour sur mon github à https://github.com/Jeremy3333/lifami-project-2 ou le téléchargé avec la commande

    git clone https://github.com/Jeremy3333/lifami-project-2.git

---

## - Installation / compilation

pour que le programme fonctionne ils vus faudra SDL2, SDL2 image et Mingw (lien dans l'onglet sources)

afin de le compiler en mode debug (est plus rapide a compilé) il faudra créer un dossier debug dans le dossier bin et y mettre les dll qu'il y a dans le fichier dll utiliser la commande(en remplacent [SDL2 include] par la position du dossier include de SDL2 sur votre ordinateur):

    g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I [SDL2 include]  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main

afin de le compiler en mode release (prend moins de place une fois compilé) il faudra créer un dossier release dans le dossier bin et y mettre les dll qu'il y a dans le fichier dll utiliser la commande(en remplacent [SDL2 include] par la position du dossier include de SDL2 sur votre ordinateur):

    g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I [SDL2 include]  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main

---

## - Source

-SDL2:
 https://www.libsdl.org/download-2.0.php

-SDL2 image:
 https://www.libsdl.org/projects/SDL_image/

-Mingw-w64:
 https://www.mingw-w64.org
