#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include <limits.h>
#include <assert.h>

#ifdef _WIN32
#include<stdlib.h>
#else
#include <unistd.h>
#endif

#include"Puissance4.h"

#define Colonnes ((uint8_t) 7)
#define Lignes ((uint8_t) 6)
#define profondeurMAX 5


void clear_screen() {
    #ifdef _WIN32
    system("cls");
    #else
    printf("\033[H\033[J");
    #endif
}

/*********************************************************************/
/***************** gestion de la grille de jeu ***********************/


//--- Méthode de création d'une grille 
lePuissance creerGrille(uint8_t nbrColonnes, uint8_t nbrLignes){ 
    /*Création de la partie  et initialisation de ses variables*/
    lePuissance grilledeJeu;
    grilledeJeu.nbColonnes = nbrColonnes;
    grilledeJeu.nblignes = nbrLignes;
    grilledeJeu.grille = NULL;
    grilledeJeu.saturationGrille = 0;

    uint8_t i;
    // Allocation dynamique du tableau de colonne (grille)
    grilledeJeu.grille = (colonne*)malloc((grilledeJeu.nbColonnes)*sizeof(colonne));
    // Vérification de l'allocation du tableau
    if(grilledeJeu.grille == NULL)
    {
        assert(0);
    }

    //Initialisation des colonnes de la grille
    for(i=0; i<grilledeJeu.nbColonnes; i++){

        grilledeJeu.grille[i].saturation = 0;
        //Allocation et initialisation des colonnes à 0 
        grilledeJeu.grille[i].tabCol =(uint8_t*)calloc(grilledeJeu.nblignes,sizeof(uint8_t));
        //Vérification de l'allocation du tableau des valeurs des colonnes
        if(grilledeJeu.grille[i].tabCol == NULL)
        {
            assert(0);
        }

    }

return grilledeJeu;
}

//--- Méthode de duplication d'une grille 
void dupliquerGrille(lePuissance grilledeJeu, lePuissance* grilleDup){
    
    // Initialisation des valeurs de la grilledeJeu dans son double
    uint8_t i, j;

    grilleDup->nbColonnes = grilledeJeu.nbColonnes;
    grilleDup->nblignes = grilledeJeu.nblignes;
    grilleDup->saturationGrille = grilledeJeu.saturationGrille;
    // Allocation dynamique du tableau de colonne (grille)
    grilleDup->grille = (colonne*)malloc((grilleDup->nbColonnes)*sizeof(colonne));
    // Vérification que le tableau est bien alloué
    if(grilledeJeu.grille == NULL)
    {
        assert(0);
    }

    //Initialisation des colonnes de la grille
    for(i=0; i<grilleDup->nbColonnes; i++){
         //Initialiser de la saturation de la colonne à sa valeurs de la grille originale
        grilleDup->grille[i].saturation = grilledeJeu.grille[i].saturation;
        grilleDup->grille[i].tabCol =(uint8_t*)calloc(grilleDup->nblignes,sizeof(uint8_t));
    
        if(grilleDup->grille[i].tabCol == NULL)
        {
            assert(0);
        }

    }
    // Parcourire la grille et affecté à la grille double les valeurs de la grille originale
    for(i=0; i<grilleDup->nbColonnes; i++){

        for(j=0; j<grilleDup->nblignes; j++){

            grilleDup->grille[i].tabCol[j] = grilledeJeu.grille[i].tabCol[j];
        }

    }  
    
}


//--- Méthode d'affichage de la grille
void afficherGrille(lePuissance grilledeJeu){ /// cet affichage est transposé pour pouvoir visualiser la grille réelement  
    
    uint8_t i, j;
    
    //affichage des nombre des colonnes
    printf("\n\n ");
    for(i = 0; i<grilledeJeu.nbColonnes; i++){

        printf(" %d  ", i+1); 
    }
    printf("\n\n");
    /***********/
    for(j = 0; j<grilledeJeu.nblignes; j++){
        printf("|");
            for(i = 0; i<grilledeJeu.nbColonnes; i++){

                if(grilledeJeu.grille[i].tabCol[grilledeJeu.nblignes-j-1] == 1){
            
                    printf("\033[1;104m J ");
                    printf("\033[0m"); // Réinitialise la couleur à celle par défaut du terminal
                }
                else if(grilledeJeu.grille[i].tabCol[grilledeJeu.nblignes-j-1] == 2){

                    printf("\033[1;101m O ");           
                    printf("\033[0m"); // Réinitialise la couleur à celle par défaut du terminal     
                }
                else{

                    printf("   ");
                }
                printf("|");
        }

        printf("\n\n");
    }
    printf("\033[0m"); 
}



//--- Méthode de vérification de la grille
_Bool saturationDeGrille(lePuissance grilledeJeu){ //Vérifie si la grille est saturé ou pas

    return (grilledeJeu.saturationGrille == (grilledeJeu.nbColonnes*grilledeJeu.nblignes));
}


//---Méthode de libération de la grille
void libererGrille(lePuissance* grille) {
    // Libérer chaque colonne de la grille
    for (uint8_t i = 0; i < grille->nbColonnes; i++) {
        free(grille->grille[i].tabCol);
    }
    // Libérer la mémoire de la grille
    free(grille->grille);
}




/*********************************************************************/
/*********************************************************************/



/*********************************************************************/
/***************** gestion d'une partie de jeu ***********************/

//---Méthode de vérification de la validité du coup 
int8_t estAutorise(lePuissance grilledeJeu, uint8_t colonne){
    // Si la colonne est saturé donc mouvement impossible
    return !(grilledeJeu.nblignes == grilledeJeu.grille[colonne].saturation);

}

//--- Jouer un coup 
_Bool jouer(lePuissance* grilledeJeu, uint8_t colonne, uint8_t codeJoueur){

    if(estAutorise(*grilledeJeu, colonne)){// Vérifier que le mouvement est valide
        //Affecter le colonne choisi à la ligne libre ( niveau de saturation de la colonne)
        grilledeJeu->grille[colonne].tabCol[grilledeJeu->grille[colonne].saturation]=codeJoueur;
        grilledeJeu->grille[colonne].saturation ++; //incrémentation de la saturation
        grilledeJeu->saturationGrille++; // garder la trace de la saturation de la grille pour pouvoir détecter la partie nulle
        return 1;
    }
    else return 0;

}
/* la fonction aGagne consiste à vérifier s'il existe un cas de victoire */
_Bool aGagne(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur){
    //Réalise les test de victoire (4 cases de même couleurs alignés) horizontalement, verticalement et sur les diagonales a partir du coup joué
    return (testDiagonalDroiteGauche(grilledeJeu, colonne, codeJoueur)||testDiagonalGaucheDroite(grilledeJeu, colonne, codeJoueur)||testHorizontal(grilledeJeu, colonne, codeJoueur)||testVertical(grilledeJeu, colonne, codeJoueur));  
}




/* le test vertical consiste seulement a tester verticalemnt les cases arrières de la colonne*/
_Bool testVertical(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur){

    int8_t i = grilledeJeu.grille[colonne].saturation-2; //On se met sur la case avant le dernier coup
    uint8_t score = 0;
    //Parcour de la colonne partant par la case qui précéde le jeton le plus récemment placé
    while(i>=0){
    
        if(grilledeJeu.grille[colonne].tabCol[i] == codeJoueur){ //vérifié que la case posséde le même code que le joueur
                    score++; //Incrémente score
                    i--; //On recule une ligne
                    if (score == 3) return 1;
        }
        else return 0;

    }
return 0;
}

/* horizontalement, on teste a gauche, puis a droite toute en gardant le score */
_Bool testHorizontal(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur){

    int8_t i = grilledeJeu.grille[colonne].saturation-1; //On se met sur la case du dernier coup
    int8_t j = colonne-1; 
    uint8_t score = 0;

    //Parcour à droite de la colonne du mouvement joué
    while(j>=0){
        
        if(grilledeJeu.grille[j].tabCol[i] == codeJoueur){ //vérifié que la case posséde le même code que le joueur
                    score++;//Incrémenter score
                    j--;//Decalé d'une colonne a droite
                    if (score == 3) return 1;
        }
        else break;// On sort de la boucle lorsque la case adjacente ne posséde pas le même codeJoueur
    }

    j = colonne+1;
    //Parcour à gauche de la colonne du mouvement joué
    while(j<grilledeJeu.nbColonnes){
    
        if(grilledeJeu.grille[j].tabCol[i] == codeJoueur){ //vérifié que la case posséde le même code que le joueur
                    score++;//Incrémenter score
                    j++;//Decalé d'une colonne a gauche
                    if (score == 3) return 1;
        }
        else break;// On sort de la boucle lorsque la case adjacente ne posséde pas le même codeJoueur
    }
return 0;
}

/* Diagonal Gauche-Droite, on teste a gauche, puis a droite toute en gardant le score */
_Bool testDiagonalGaucheDroite(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur){

    int8_t i = grilledeJeu.grille[colonne].saturation-2; //On se met sur la ligne avant le dernier coup
    int8_t j = colonne-1;//On se met sur la colonne à gauche de celle du dernier coup
    uint8_t score = 0;

    //Parcour à gauche de la colonne du mouvement joué
    while(j>=0 && i>=0){
        
        if(grilledeJeu.grille[j].tabCol[i] == codeJoueur){ //vérifié que la case posséde le même code que le joueur
                    score++; 
                    j--;//Decalé d'une colonne a gauche
                    i--;//On recule d'une ligne
                    if (score == 3) return 1;
        }
        else break;// On sort de la boucle lorsque la case adjacente ne posséde pas le même codeJoueur
    }

    i = grilledeJeu.grille[colonne].saturation; //On se met sur la ligne en dessus du dernier coup
    j = colonne+1;//On se met sur la colonne à droite de celle du dernier coup
    //Parcour à droite de la colonne du mouvement joué
    while(j<grilledeJeu.nbColonnes && i<grilledeJeu.nblignes){
    
        if(grilledeJeu.grille[j].tabCol[i] == codeJoueur){ 
                    score++;
                    j++;//Decalé d'une colonne a droite
                    i++;//On monte d'une ligne
                    if (score == 3) return 1;
        }
        else break;// On sort de la boucle lorsque la case ne posséde pas le même codeJoueur
    }
return 0;
}


/* Diagonal Gauche-Droite, on teste a gauche, puis a droite toute en gardant le score */
_Bool testDiagonalDroiteGauche(lePuissance grilledeJeu, uint8_t colonne, uint8_t codeJoueur){

    int8_t i = grilledeJeu.grille[colonne].saturation-2; //On se met sur la ligne avant le dernier coup
    int8_t j = colonne+1;//On se met sur la colonne à droite de celle du dernier coup
    uint8_t score = 0;

    //Parcour à droite de la colonne du mouvement joué
    while(j<grilledeJeu.nbColonnes && i>=0){
        
        if(grilledeJeu.grille[j].tabCol[i] == codeJoueur){ //vérifié que la case posséde le même code que le joueur
                    score++;
                    j++;//Decalé d'une colonne a droite
                    i--;//On recule d'une ligne
                    if (score == 3) return 1;
        }
        else break;// On sort de la boucle lorsque la case adjacente ne posséde pas le même codeJoueur
    }

    i = grilledeJeu.grille[colonne].saturation; //On se met sur la ligne en dessus du dernier coup
    j = colonne-1;//On se met sur la colonne à gauche de celle du dernier coup

    while(j>=0 && i<grilledeJeu.nblignes){
    
        if(grilledeJeu.grille[j].tabCol[i] == codeJoueur){ //vérifié que la case posséde le même code que le joueur
                    score++;
                    j--;//Decalé d'une colonne a gauche
                    i++;//On monte d'une ligne
                    if (score == 3) return 1;
        }
        else break;// On sort de la boucle lorsque la case adjacente ne posséde pas le même codeJoueur
    }
return 0;
}



/*********************************************************************/
/*********************************************************************/


/*********************************************************************/
/***************** gestion de l'arbre ********************************/

/*newSommetIA consiste à initialiser une structure sommetIA*/
sommetIA* newSommetIA(lePuissance grilledeJeu){
    //Allocation de la structure sommetIA
    sommetIA *sommet = malloc(sizeof(sommetIA));
    if(sommet == NULL){ // Vérification de l'allocation 
        
        assert(0);
    }

    dupliquerGrille(grilledeJeu, &sommet->grille); // Dupliquer la grille actuelle dans celle du sommet

    // Initialiser les valeurs du sommets
    sommet->score = 0;
    sommet->nbrFils = 0;
    sommet->pere = NULL;
    // Allocation du tableau des pointeur sur les fils
    sommet->fils = (sommetIA**)malloc(sizeof(sommetIA*)*grilledeJeu.nbColonnes);
    int i;
    // Initialisation des pointeur à null
    for(i = 0; i<grilledeJeu.nbColonnes; i++){
        sommet->fils[i] = NULL;
    }


return sommet;
}

/*La fonction consiste à initialiser une structure arbreIA*/
arbreIA* newArbreIA(){
    // Allocation de la structure arbreIA
    arbreIA *arbre = malloc(sizeof(arbreIA));
    if(arbre == NULL){ // Vérification de l'allocation
    
        assert(0);
    }

    arbre->racine = NULL; //Initialisation à NULL de la racine (structure sommetIA)
return arbre;
}

/* Cette fonction consiste à ajouter une racine à une structure arbreIA */
void addRacine(arbreIA* arbre, lePuissance grilledeJeu){
    // Création d'un sommet à partir de la grille passé en paramétre avec la fonction newSommetIA
    sommetIA* sommet = newSommetIA(grilledeJeu);
    arbre->racine = sommet;// Affecter le nouveau sommet créé à la racine de arbre  
}

/*Cette fonction consiste à ajouter un sommet fils à un sommet père données*/
void addFils(sommetIA* sommetPere, sommetIA* sommetFils, uint8_t numFils){
    //Vérifié si le père peut encore avoir des fils
    if(sommetPere->nbrFils < sommetFils->grille.nbColonnes){

        sommetPere->fils[numFils] = sommetFils;//Affecter le pointeur sur sommet fils au tableau des fils à la position numFils 
        sommetPere->nbrFils++;//Incrémenter le nombre des fils
        sommetFils->pere = sommetPere;// Affecter au sommetFils comme père le sommetPere
    }
    else{//Cas ou le nombre de fils est égale au nombre de colonnes de la grille
        printf("impossible d'ajouter un fils");
    }
}





/*La fonction consiste à construire l'arbre de décision*/
void construireArbre(arbreIA* arbre){



    sommetIA* sommetCourant = arbre->racine;
    sommetIA* som;
    uint8_t i =0, etage = 2, victoire = 0, nonAutorise = 1; // avec la variable étage, on peut garder l'étage actuel de l'arbre qu'on est entrain de remplir, ce qui nous permet de savoir le joueur qui jouera (a partir de l'étage)


    uint32_t cpt=0;

    while (sommetCourant != NULL){

        nonAutorise = 1;
        
           for(i=0; i<sommetCourant->grille.nbColonnes; i++){
              
                if((estAutorise(sommetCourant->grille, i)) && (sommetCourant->fils[i] == NULL)){

                    nonAutorise = 0;
                    break;
                }
                
            }


            if(nonAutorise|| victoire || etage == profondeurMAX+2){     /**** le max profondeur --> define, ici c'est mis à titre provisoire ****/
                
                if(sommetCourant->pere != NULL){

                    sommetCourant = sommetCourant->pere;
                    victoire = 0;
                    etage--; 
                }
                else{


                    sommetCourant = NULL;
                }

            }
            
            else{
                 som = newSommetIA(sommetCourant->grille);
                
                 jouer(&som->grille, i, 2-(etage%2));
                 // l'expression 2-(etage%2) donne comme résultat soit 2 ou 1, et ceci déprendera de l'etage actuel de l'arbre

                 
                 // si le joueur actuel gagne, on ne dévelope pas ces fils;
                 if (!victoire){  
                     addFils(sommetCourant, som, i); cpt++;

                     victoire = aGagne(som->grille, i, 2-(etage%2));

                     sommetCourant = sommetCourant->fils[i];
                     etage++; 
                    
                 }
                 
                
            }

    }

}


/*La fonction permet d'affecter des scores aux feuilles*/
void remplirScoresFeuilles(sommetIA* sommet) {
    if (sommet == NULL) {
        return;
    }

    if (sommet->nbrFils == 0) {
        // C'est une feuille, remplir le score en fonction de la grille
        sommet->score = evaluationTotale(sommet->grille);
        return;
    }

    // Remplit les scores des fils (uniquement les feuilles)
    uint8_t i=0;
    while(i<sommet->grille.nbColonnes){  
        if(sommet->fils[i]!=NULL){
          remplirScoresFeuilles(sommet->fils[i]);
          
        }
        i++;
    }
}


/*libererArbre consiste à liberer en totale l'arbre de décision de la mémoire*/
void libererArbre(sommetIA* sommet) {
    if (sommet == NULL) {
        return;
    }

    uint8_t i = 0; // Initialise i à zéro

    // Libérer les fils récursivement
    while (i < sommet->grille.nbColonnes) {
        if (sommet->fils[i] != NULL) {
            libererArbre(sommet->fils[i]);// Appelle récursif
        }
        i++;
    }

    // Libérer la grille du sommet
    libererGrille(&sommet->grille);
    free(sommet->fils);
    // Libérer le sommet lui-même
    free(sommet);
}
/*libererArbreRacine consiste à liberer la structure arbre de la mémoire*/
void libererArbreRacine(arbreIA* arbre) {
    libererArbre(arbre->racine);
    free(arbre);
}




/*********************************************************************/
/*********************************************************************/


/*********************************************************************/
/***************** gestion de l'IA ***********************************/

/*Minimax permet d’associer aux noeuds leur score */
int minimax(sommetIA* sommet, int IAjoue) {
    uint8_t i;
    int meilleurScore;

    if (sommet->nbrFils == 0) {
        return sommet->score;  // Renvoie le score des feuilles
    }

    if (IAjoue) {
        meilleurScore = INT_MIN;

        for (i = 0; i < sommet->grille.nbColonnes; i++) {
            if (sommet->fils[i] != NULL) {
                int scoreFils = minimax(sommet->fils[i], 0);// Appelle récursif
                if (scoreFils > meilleurScore) {
                    meilleurScore = scoreFils;
                    //decision = i;
                }
            }
        }

        sommet->score = meilleurScore;  // Met à jour le score du nœud actuel.
    
        return meilleurScore;
    } else {
        meilleurScore = INT_MAX;

        for (i = 0; i < sommet->grille.nbColonnes; i++) {
            if (sommet->fils[i] != NULL) {
                int scoreFils = minimax(sommet->fils[i], 1);// Appelle récursif
                if (scoreFils < meilleurScore) {
                    meilleurScore = scoreFils;
                    //decision = i;
                }
            }
        }

        sommet->score = meilleurScore;  // Met à jour le score du nœud actuel.

        return meilleurScore;
    }

}



int32_t calculateurScore(uint8_t score, uint8_t vides, uint8_t codeJoueur){

            if(codeJoueur==2){
         
                if (score == 4) {
                    return 100;
                }
                else if (score == 3 && vides == 1) {
                    return 5;
                } 
                else if (score == 2 && vides == 2) {
                    return 2;
                }

            }
            else if(codeJoueur==1){
               
                if (score == 4) {
                    return -95;
                } 
                else if (score == 3 && vides == 1) {
                    return -4;
                } 
                else if (score == 2 && vides == 2) {
                    return -1;
                }

            }
return 0;
}

int32_t evaluationHorizontale(lePuissance grilledeJeu, uint8_t codeJoueur) {
    int32_t scoreTotal = 0;

    // Parcours horizontal de la grille
    for (uint8_t i = 0; i < grilledeJeu.nblignes; i++) {
        for (uint8_t j = 0; j <= grilledeJeu.nbColonnes - 4; j++) {
            // Test à gauche
            uint8_t score = 0, vides = 0;
            uint8_t k = j;
            while (k < j + 4) {
                if (grilledeJeu.grille[k].tabCol[i] == codeJoueur) {
                    score++;
                } else if (grilledeJeu.grille[k].tabCol[i] == 0) {
                    vides++;
                } else {
                    break;
                }
                k++;
            }

            // Calcul du score selon les critères
            scoreTotal += calculateurScore(score, vides, codeJoueur);

        }
    }

    return scoreTotal;
}


int32_t evaluationVerticale(lePuissance grilledeJeu, uint8_t codeJoueur) {
    int32_t scoreTotal = 0;

    // Parcours vertical de la grille
    for (uint8_t i = 0; i <= grilledeJeu.nblignes - 4; ++i) {
        for (uint8_t j = 0; j < grilledeJeu.nbColonnes; ++j) {
            // Test vers le bas
            uint8_t score = 0, vides = 0;
            uint8_t k = i;
            while (k < i + 4) {
                if (grilledeJeu.grille[j].tabCol[k] == codeJoueur) {
                    score++;
                } else if (grilledeJeu.grille[j].tabCol[k] == 0) {
                    vides++;
                } else {
                    break;
                }
                k++;
            }

            scoreTotal += calculateurScore(score, vides, codeJoueur);
        }
    }

    return scoreTotal;
}



int32_t evaluationDiagonale(lePuissance grilledeJeu, uint8_t codeJoueur) {
    int32_t scoreTotal = 0;

    // Parcours des diagonales descendantes (de la droite vers la gauche)
    for (uint8_t i = 0; i <= grilledeJeu.nblignes - 4; ++i) {
        for (uint8_t j = 0; j <= grilledeJeu.nbColonnes - 4; ++j) {
            uint8_t score = 0, vides = 0;
            for (uint8_t k = 0; k < 4; ++k) {
                if (grilledeJeu.grille[j + k].tabCol[i + k] == codeJoueur) {
                    score++;
                } else if (grilledeJeu.grille[j + k].tabCol[i + k] == 0) {
                    vides++;
                } else {
                    break;
                }
            }

            scoreTotal += calculateurScore(score, vides, codeJoueur);
        }
    }

    // Parcours des diagonales ascendantes (de la gauche vers la droite)
    for (uint8_t i = 3; i < grilledeJeu.nblignes; ++i) {
        for (uint8_t j = 0; j <= grilledeJeu.nbColonnes - 4; ++j) {
            uint8_t score = 0, vides = 0;
            for (uint8_t k = 0; k < 4; ++k) {
                if (grilledeJeu.grille[j + k].tabCol[i - k] == codeJoueur) {
                    score++;
                } else if (grilledeJeu.grille[j + k].tabCol[i - k] == 0) {
                    vides++;
                } else {
                    break;
                }
            }

            scoreTotal += calculateurScore(score, vides, codeJoueur);
        }
    }

    return scoreTotal;
}

int32_t evaluationTotale(lePuissance grilledeJeu){

    return (evaluationHorizontale(grilledeJeu,1)+evaluationHorizontale(grilledeJeu,2)+evaluationVerticale(grilledeJeu,1)+evaluationVerticale(grilledeJeu,2)+evaluationDiagonale(grilledeJeu,1)+evaluationDiagonale(grilledeJeu,2));
}






uint8_t prendreDecision(arbreIA* arbre){
        
       int8_t decision = -1, indice;

       int32_t scoreActuel = INT_MIN;
      
        //Parcour fils 
       for(indice = 0; indice < arbre->racine->grille.nbColonnes; indice++){

            if(arbre->racine->fils[indice] != NULL){
                if((arbre->racine->fils[indice]->score) >= scoreActuel){//Chercher le score max
                        
                    decision = indice; //Garder l'indice du fils avec le score max
                    scoreActuel = arbre->racine->fils[indice]->score;
                }
            }
       }

return decision;
}




/*********************************************************************/
/*********************************************************************/



/*********************************************************************/
/***************** Lancement d'une partie ****************************/


void partieHumainContreIA(uint8_t nbrColonnes, uint8_t nbrLignes){

    lePuissance grilledeJeu = creerGrille(nbrColonnes, nbrLignes);

    int stop = 0, decision, joueur, permis;
    
    
    clear_screen();
    afficherGrille(grilledeJeu);
    joueur = 1;
    while (!stop){
    
        if(joueur == 1){
            printf("\nC'est à vous de jouer, veuillez choisir une colonne entre 1 et %d : ",grilledeJeu.nbColonnes);
verification: 
            while (scanf("%d", &decision) != 1 || !(decision >= 1 && decision <= grilledeJeu.nbColonnes)) {
                clear_screen();
                afficherGrille(grilledeJeu);
                printf("\nVotre choix est invalide. Veuillez entrer un chiffre entre 1 et %d : ",grilledeJeu.nbColonnes);
                while (getchar() != '\n'); // Vide le buffer d'entrée
            }
            
            clear_screen();
         
            permis = jouer(&grilledeJeu, decision-1, joueur);

            if(!permis){
                afficherGrille(grilledeJeu);
                printf("\nCette action ne peut pas etre effectuer, la colonne choisie est pleine\nveuillez choisir une autre colonne : ");
                while (getchar() != '\n'); 
                goto verification;

            }

            if(aGagne(grilledeJeu, decision-1, joueur)){
                printf("\nFélicitation! vous avez battu l'ordinateur");
                stop = 1;
            }
            else if(saturationDeGrille(grilledeJeu)){
                printf("\nPartie nulle, personne ne gange\n");
                stop = 1;
            }
            else{
                joueur = (joueur%2)+1;
            }
        
        }
        else{


            arbreIA* arbre;
            arbre = newArbreIA();
            addRacine(arbre, grilledeJeu);
            construireArbre(arbre);
            remplirScoresFeuilles(arbre->racine);

            minimax(arbre->racine, 1);

            printf("\n\n");
            decision = prendreDecision(arbre);

            jouer(&grilledeJeu, decision, joueur);
            libererArbreRacine(arbre);
            clear_screen();

            afficherGrille(grilledeJeu);

            printf("L'ordinateur a joué dans la colonne %d\n", decision+1);
            

            if(aGagne(grilledeJeu, decision, joueur)){
                printf("\nMalheureusement, L'ordinateur vous a gagné\n\n");
                stop = 1;
            }

            else if(saturationDeGrille(grilledeJeu)){
                printf("\nPartie nulle, personne n'a gangé\n");
                stop = 1;
            }
            else{
                joueur = (joueur%2)+1;
            }
            
        }

    }

libererGrille(&grilledeJeu);
printf("\n");
}


void partieHumainContreHumain(uint8_t nbrColonnes, uint8_t nbrLignes){

 
    lePuissance grilledeJeu = creerGrille(nbrColonnes, nbrLignes);


    int stop = 0, decision, joueur, permis;
    
    clear_screen();
    afficherGrille(grilledeJeu);
    joueur = 1;
    while (!stop){
       
        printf("\nLe tour est au joueur");
        if(joueur == 1){

            printf("\033[1;104m J ");           
            printf("\033[0m"); // Réinitialise la couleur à celle par défaut du terminal     
            
        }
        else if(joueur == 2){
            
            printf("\033[1;101m O ");           
            printf("\033[0m"); // Réinitialise la couleur à celle par défaut du terminal    
    
        }
        printf("\nveuillez choisir une colonne entre 1 et %d :", grilledeJeu.nbColonnes);

verification: 
        while (scanf("%d", &decision) != 1 || !(decision >= 1 && decision <= grilledeJeu.nbColonnes)) {
            clear_screen();
            afficherGrille(grilledeJeu);
            printf("\nVotre choix est invalide. Veuillez entrer un chiffre entre 1 et %d : ",grilledeJeu.nbColonnes);
            while (getchar() != '\n'); // Vide le buffer d'entrée
        }

        clear_screen();
        permis = jouer(&grilledeJeu, decision-1, joueur);

        if(!permis){
            afficherGrille(grilledeJeu);
            printf("\nCette action ne peut pas etre effectuer, la colonne choisie est pleine\nveuillez choisir une autre colonne : ");
            while (getchar() != '\n'); // Vide le buffer d'entrée
            goto verification;

        }

        afficherGrille(grilledeJeu);
        if(aGagne(grilledeJeu, decision-1, joueur)){
            printf("\nFélicitation au joueur ");
            if(joueur == 1){

                printf("\033[1;104m J ");           
                printf("\033[0m"); // Réinitialise la couleur à celle par défaut du terminal     
                
            }
            else if(joueur == 2){
                
                printf("\033[1;101m O ");           
                printf("\033[0m"); // Réinitialise la couleur à celle par défaut du terminal    
        
            }    
            printf(" !, vous avez gagner la partie\n");    
            stop = 1;
        }
        else if(saturationDeGrille(grilledeJeu)){
            printf("\nPartie nulle, personne ne gange\n");
            stop = 1;
        }
        else{
            joueur = (joueur%2)+1;
        }
        
    
    }

libererGrille(&grilledeJeu);
printf("\n");
}


void lancerPartie(){

    int choix1, choix2;
    int  nbCol, nbLig;

    
    printf("\n\nBienvenue au jeu lePuissance4");
repeat1:
    printf("\n\n1 - Jouer une partie classique 6x7\n");
    printf("2 - Définir les coordonnées de votre grille\n");
    printf("3 - Quitter\n\n");

    printf("Veuillez entrer votre choix :");

    while (scanf("%d", &choix1) != 1 || !(choix1 >= 1 && choix1 <= 3)) {
        clear_screen();
        printf("Votre choix est invalide\n");
        while (getchar() != '\n'); // Vide le buffer d'entrée
        goto repeat1;
    }
    clear_screen();
    switch(choix1){

        case 1:{
            repeat2:   
            printf("\n\n1- jouer une partie contre un ami\n");
            printf("2- jouer une partie contre l'ordinateur\n");
            printf("3 - Retour au menu\n\n");

            printf("Veuillez entrer votre choix :");

            while (scanf("%d", &choix2) != 1 || !(choix2 >= 1 && choix2 <= 3)) {
                clear_screen();
                printf("Votre choix est invalide\n");
                while (getchar() != '\n'); // Vide le buffer d'entrée
                goto repeat2;
            }
            
            clear_screen();
            switch(choix2){

                case 1:{
            
                    partieHumainContreHumain(Colonnes,Lignes);
                    return;
                }
                
                case 2:{
                    
                    partieHumainContreIA(Colonnes,Lignes);
                    return;
                }

                case 3:goto repeat1;    
            }
        }

        case 2: {
            col:
                printf("\n\nVeuillez entrer le nombre de Colonnes: ");

                while (scanf("%d", &nbCol) != 1) {
                    clear_screen();
                    printf("Votre choix est invalide\n");
                    while (getchar() != '\n'); // Vide le buffer d'entrée
                    goto col;
                }          
                
            lig:
                printf("\n\nVeuillez entrer le nombre de Lignes: ");

                while (scanf("%d", &nbLig) != 1) {
                    clear_screen();
                    printf("Votre choix est invalide\n");
                    while (getchar() != '\n'); // Vide le buffer d'entrée
                    goto lig;
                }      

            
            repeat3:   
            clear_screen();
            printf("\n\n1- jouer une partie contre un ami\n");
            printf("2- jouer une partie contre l'ordinateur\n");
            printf("3 - Retour au menu\n\n");

            printf("Veuillez entrer votre choix :");

            while (scanf("%d", &choix2) != 1 || !(choix2 >= 1 && choix2 <= 3)) {
                clear_screen();
                printf("Votre choix est invalide\n");
                while (getchar() != '\n'); // Vide le buffer d'entrée
                goto repeat3;
            }
            
            clear_screen();
            switch(choix2){

                case 1:{
            
                    partieHumainContreHumain((uint8_t)nbCol,(uint8_t)nbLig);
                    return;
                }
                
                case 2:{
                    
                    partieHumainContreIA((uint8_t)nbCol,(uint8_t)nbLig);
                    return;
                }

                case 3:goto repeat1;    
            }              

        }

        case 3: return;
    }
    
}
