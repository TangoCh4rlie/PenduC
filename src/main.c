#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dbAction.h"
#include "dbFunctions.h"

char *mot;
char *motDevoile;
char listeLettreFausse[26];
int nbErreur;

void initVar() {
  srand(time(NULL));

  mot = randomWord();
  int sizeOfWord = strlen(mot);

  motDevoile = (char *)malloc(sizeOfWord + 1);
  for (int i = 0; i < sizeOfWord; i++) {
    motDevoile[i] = '_';
  }

  motDevoile[sizeOfWord] = '\0';

  nbErreur = 0;
}

char saisieLettre() {
  char lettre;
  printf("\nEntrez une lettre -> ");
  scanf(" %c", &lettre);
  for (int i = 0; i < nbErreur; i++) {
    if (listeLettreFausse[i] == lettre) {
      fprintf(stdout, "Attention la lettre a déja été utilisé\n");
      return '\0';
    }
  }
  int hexVal = lettre;
  if (hexVal >= 65 && hexVal <= 90) {
    lettre = lettre + 32;
  } else if (hexVal < 97 || hexVal > 122) {
    fprintf(stderr, "Le caractère renté n'est pas valable");
  }
  // TODO faire la verification si c'est bien une lettre
  return lettre;
}

int finJeu() {
  if (strcmp(mot, motDevoile) == 0) {
    system("clear");
    printf("Bravo le mot était bien %s\n", mot);
    return 0;
  } else if (nbErreur >= 11) {
    system("clear");
    printf("Perdu le mot était %s\n", mot);
    return 1;
  }
  return 0;
}

void lettreEstDansMot(char lettre, int sizeOfWord) {
  if (lettre == '\0') {
    return;
  }
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

  while (strcmp(mot, motDevoile) != 0 && nbErreur < 11) {
    system("clear");

    printf("Le mot contient %d lettres: %s", sizeOfWord, motDevoile);

    printf("\nLa liste des lettres déja utilisé est: ");
    for (int i = 0; i < nbErreur; i++) {
      printf("%c ", listeLettreFausse[i]);
    }

    printf("\nVous avez %d erreurs", nbErreur);

    lettreEstDansMot(saisieLettre(), sizeOfWord);
  }

  finJeu();

  free(motDevoile);
}

int pendu() {
  initVar();
  jeuPendu();
  return 0;
}

int menu() {
  int result = 100;

  while (result != 0) {
    fprintf(stdout, "1: Jouer au pendu\n");
    fprintf(stdout, "2: Gérer la base de donnée\n");
    fprintf(stdout, "0: Exit\n");
    fprintf(stdout, "Que voulez vous faire: ");
    scanf("%d", &result);

    switch (result) {
    case 1:
      pendu();
      break;
    case 2:
      system("clear");
      databaseConnect();
      break;
    case 0:
      exit(EXIT_SUCCESS);
      break;
    default:
      system("clear");
      fprintf(stderr, "Ce numéro ne correspond à rien");
      break;
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  menu();
  return 0;
}
