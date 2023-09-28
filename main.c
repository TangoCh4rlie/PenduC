#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pendu.h"

char *listMot[2] = {"bonjour", "salut"};
char *mot;
char *motDevoile;
char listeLettreFausse[26];

void initVar() {
  srand(time(NULL));

  mot = listMot[rand() % 2];
  int sizeOfWord = strlen(mot);

  motDevoile = (char *)malloc(sizeOfWord + 1);
  for (int i = 0; i < sizeOfWord; i++) {
    motDevoile[i] = '_';
  }

  motDevoile[sizeOfWord] = '\0';
}

char saisieLettre() {
  char lettre;
  printf("\nEntrez une lettre -> ");
    scanf(" %c", &lettre);
  //TODO faire la verification si c'est bien une lettre 
  //TODO mettre la lettre en minuscule
  return lettre;
}

void finJeu(int nbErreur) {
    if (strcmp(mot, motDevoile) == 0) {
    system("clear");
    printf("Bravo le mot était bien %s", mot);
  } else if (nbErreur >= 11) {
    printf("Perdu le mot était %s", mot);
  }
}

void lettreEstDansMot(int nbErreur, char lettre, int sizeOfWord) {
    bool estDedans = false;
    for (int i = 0; i < sizeOfWord; i++) {
      if (lettre == mot[i]) {
        motDevoile[i] = lettre;
        estDedans = true;
      }
    }
    if (!estDedans) {
      listeLettreFausse[nbErreur] = lettre;
      nbErreur++;
    }
}

void jeuPendu() {
  int sizeOfWord = strlen(mot);
  int nbErreur = 0;

  while (strcmp(mot, motDevoile) != 0 && nbErreur < 11) {
    system("clear");

    printf("Le mot contient %d lettres: %s", sizeOfWord, motDevoile);

    printf("\nLa liste des lettres déja utilisé est: ");
    for (int i = 0; i < nbErreur; i++) {
      printf("%c ", listeLettreFausse[i]);
    }

    printf("\nVous avez %d erreurs", nbErreur);

    lettreEstDansMot(nbErreur, saisieLettre(), sizeOfWord);
  }

  finJeu(nbErreur);

  free(motDevoile);  
}

int main(int argc, char *argv[]) {
    initVar();
    jeuPendu();
  
    return 0;
}