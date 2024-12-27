#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<limits.h>
#include<assert.h>


/*************** structure de la grille de jeu ******************/
typedef struct colonne{
    
    uint8_t* tabCol;
    uint8_t saturation;

}colonne;

typedef struct lePuissance{
    
    colonne* grille;
    uint8_t nbColonnes;
    uint8_t nblignes;
    
    //J'ai ajouté ce paramètre pour pouvoir contrôler la saturation de la grille, après ci cette dernière est complètement remplie sans aucun des joueurs gagne alors la partie est déclarée nulle
    uint8_t saturationGrille;

}lePuissance;


/*********************************************************************************************/

/*************** structure de l'arbre ****************************/
typedef struct _sommetIA{

    lePuissance grille;
    struct _sommetIA** fils; // un tableau des fils
    struct _sommetIA* pere;
    uint8_t nbrFils;
    int32_t score;
   
}sommetIA;


typedef struct arbreIA{

    sommetIA* racine;


}arbreIA;

/*********************************************************************/
/***************** gestion de la grille de jeu ***********************/

lePuissance creerGrille(uint8_t nbrColonnes, uint8_t nbrLignes);
void dupliquerGrille(lePuissance grilledeJeu, lePuissance* grilleDup);
void afficherGrille(lePuissance grilledeJeu);
_Bool saturationDeGrille(lePuissance grilledeJeu);
void libererGrille(lePuissance* grille);


/*********************************************************************/
/***************** gestion d'une partie de jeu ***********************/

int8_t estAutorise(lePuissance grilledeJeu, uint8_t colonne);
_Bool jouer(lePuissance* grilledeJeu, uint8_t colonne, uint8_t codeJoueur);
_Bool aGagne(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur);
_Bool testVertical(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur);
_Bool testHorizontal(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur);
_Bool testDiagonalGaucheDroite(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur);
_Bool testDiagonalDroiteGauche(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur);


/*********************************************************************/
/***************** gestion de l'arbre ********************************/

sommetIA* newSommetIA(lePuissance grilledeJeu);
arbreIA* newArbreIA();
void addRacine(arbreIA* arbre, lePuissance grilledeJeu);
void addFils(sommetIA* sommetPere, sommetIA* sommetFils, uint8_t numFils);
void construireArbre(arbreIA* arbre);
void remplirScoresFeuilles(sommetIA* sommet);
void libererArbre(sommetIA* sommet);
void libererArbreRacine(arbreIA* arbre);


/*********************************************************************/
/***************** gestion de l'IA ***********************************/

int minimax(sommetIA* sommet, int IAjoue);
int32_t calculateurScore(uint8_t score, uint8_t vides, uint8_t codeJoueur);
int32_t evaluationHorizontale(lePuissance grilledeJeu, uint8_t codeJoueur);
int32_t evaluationVerticale(lePuissance grilledeJeu, uint8_t codeJoueur);
int32_t evaluationDiagonale(lePuissance grilledeJeu, uint8_t codeJoueur);
int32_t evaluationTotale(lePuissance grilledeJeu);
uint8_t prendreDecision(arbreIA* arbre);



/*********************************************************************/
/***************** Lancement d'une partie ****************************/
void partieHumainContreIA(uint8_t nbrColonnes, uint8_t nbrLignes);
void partieHumainContreHumain(uint8_t nbrColonnes, uint8_t nbrLignes);
void lancerPartie();















