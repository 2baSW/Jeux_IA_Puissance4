# Puissance 4

Ce projet implémente le jeu de Puissance 4 en C avec une interface de jeu pour deux joueurs humains ou un joueur humain contre l'ordinateur.

## Fichiers

- `Puissance4.c` : Contient les fonctions principales pour gérer la grille de jeu, les parties, et l'IA.
- `Puissance4.h` : Contient les définitions des structures et des prototypes de fonctions.
- `main.c` : Point d'entrée du programme.

## Compilation

Pour compiler le projet, utilisez la commande suivante :

```sh
gcc -o puissance4 main.c Puissance4.c
```

## Exécution

Pour lancer le jeu, exécutez le fichier compilé :

```sh
./puissance4
```

## Fonctionnalités

### Gestion de la grille de jeu

- **Création de la grille** : `lePuissance creerGrille(uint8_t nbrColonnes, uint8_t nbrLignes)`
- **Duplication de la grille** : `void dupliquerGrille(lePuissance grilledeJeu, lePuissance* grilleDup)`
- **Affichage de la grille** : `void afficherGrille(lePuissance grilledeJeu)`
- **Vérification de la saturation de la grille** : `_Bool saturationDeGrille(lePuissance grilledeJeu)`
- **Libération de la grille** : `void libererGrille(lePuissance* grille)`

### Gestion d'une partie de jeu

- **Vérification de la validité d'un coup** : `int8_t estAutorise(lePuissance grilledeJeu, uint8_t colonne)`
- **Jouer un coup** : `_Bool jouer(lePuissance* grilledeJeu, uint8_t colonne, uint8_t codeJoueur)`
- **Vérification de la victoire** : `_Bool aGagne(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur)`
- **Tests de victoire** : `testVertical`, `testHorizontal`, `testDiagonalGaucheDroite`, `testDiagonalDroiteGauche`

### Gestion de l'arbre de décision

- **Création d'un sommet** : `sommetIA* newSommetIA(lePuissance grilledeJeu)`
- **Création d'un arbre** : `arbreIA* newArbreIA()`
- **Ajout d'une racine** : `void addRacine(arbreIA* arbre, lePuissance grilledeJeu)`
- **Ajout d'un fils** : `void addFils(sommetIA* sommetPere, sommetIA* sommetFils, uint8_t numFils)`
- **Construction de l'arbre** : `void construireArbre(arbreIA* arbre)`
- **Remplissage des scores des feuilles** : `void remplirScoresFeuilles(sommetIA* sommet)`
- **Libération de l'arbre** : `void libererArbre(sommetIA* sommet)`, `void libererArbreRacine(arbreIA* arbre)`

### Gestion de l'IA

- **Algorithme Minimax** : `int minimax(sommetIA* sommet, int IAjoue)`
- **Calcul des scores** : `int32_t calculateurScore`, `int32_t evaluationHorizontale`, `int32_t evaluationVerticale`, `int32_t evaluationDiagonale`, `int32_t evaluationTotale`
- **Prise de décision** : `uint8_t prendreDecision(arbreIA* arbre)`

### Lancement d'une partie

- **Partie Humain contre IA** : `void partieHumainContreIA(uint8_t nbrColonnes, uint8_t nbrLignes)`
- **Partie Humain contre Humain** : `void partieHumainContreHumain(uint8_t nbrColonnes, uint8_t nbrLignes)`
- **Lancer une partie** : `void lancerPartie()`

## Auteur

Baba SOW
