# LIFAMI-PROJET

###### Jérémy SIMOND

Mon objectif est de faire un programme qui permet de faire une simulation de gravité entre planètes en 2D, ainsi qu'une sumilation de champ d'énergie avec une équation simplifiée

afin de plus facilement comprendre quel includes sont mes code ou des librerie publiques j'ai indiquer en commentaire en dessus et aussi les librairies publique ont leur nom entre <> et ce que j'ai codé moi est entre ""

je m'excuse pour l'anglais en commentaire.

---

## - explication

### - Gravité

#### - pourquoi

J'ai créer une simulation de gravité car je me desais que c'était à ma porté et que je voulais faire une simulation "simple" avant d'en faire une plus compliquer afin d'être sur de rendre quelque chose.

#### - explication

J'ai d'abord créer deux structure une de planete et une de galaxy (décrie en commentaire) en suite quand la galaxie est initialisée elle initiallise ses planète qui elle initiallise leur traces quand le bouton bleu est appuié la galaxy est réinitialliser (mais pas la planète sélectionnée) à chaque tour de la boucle chaque gravité entre planète est calculée ```((G * m1 * m2) / distance²)``` en suite avec ca on calcule la vélocité puis la nouvelle position. Après ça la nouvelle position est ajoutée dans le tableau de trace à l'index de la trace qui lui augmante de un. en suite les planète son dessinée sur le rendu (avec la position et le rayon) et les traces (un point a chaque position de la trace). Pour la decentralisation du dessin (quand je veux que le centre de l'écran soit une planète) j'ai utilisé une fonction avant de dessiné une planète (getDrawPosition dans utils.cpp)

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
