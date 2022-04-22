LIFAMI-PROJET
=======

Mon objectif est de faire un programme qui permet de faire une simulation de gravité entre planètes en 2D.
Il y aura posibilité:
 - de faire des mettre en pause la simulation (bouton rouge en bas a gauche) et de deplacer les planetes le bouton bleu remet a zéro la galaxie mais pas la planete selectionnée.
 - de selectionner les planètes avec la souris et de voir la simulation centrée sur cette planete(clique droit de la souris) ou de changer leur position (clique gauche de la souris).
 - de voir la trajectoir des planètes grâce a des traces

Je prévois aussi si j'ai le temps de faire une simulation de la fusion de deux champ éléctromagnétique (le bouton vert pour y accéder si il est implémenté)

afin de plus facilement comprendre quel includes sont mes code ou des librerie publiques j'ai indiquer en commentaire en dessus et aussi les librairies publique ont leur nom entre <> et ce que j'ai codé moi est entre ""

je m'excuse pour l'anglais en commentaire.

Téléchargement
--------

Vous pouver trouver une version a jour sur mon github à https://github.com/Jeremy3333/lifami-project-2 ou le téléchargé avec la commande

    git clone https://github.com/Jeremy3333/lifami-project-2.git

Installation / compilation
-------
pour que le programme fonctionne ils vus faudra SDL2, SDL2 image et Mingw (lien dans l'onglet sources)

afin de le compiler en mode debug (est plus rapide a compilé) il faudra créer un dossier debug dans le dossier bin et y mettre les dll qu'il y a dans le fichier dll utiliser la commande(en remplacent [SDL2 include] par la position du dossier include de SDL2 sur votre ordinateur):

    g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I [SDL2 include]  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main

afin de le compiler en mode release (prend moins de place une fois compilé) il faudra créer un dossier release dans le dossier bin et y mettre les dll qu'il y a dans le fichier dll utiliser la commande(en remplacent [SDL2 include] par la position du dossier include de SDL2 sur votre ordinateur):

    g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I [SDL2 include]  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main

Source
-------

-SDL2:
 https://www.libsdl.org/download-2.0.php

-SDL2 image:
 https://www.libsdl.org/projects/SDL_image/

-Mingw-w64:
 https://www.mingw-w64.org
