#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 *db;

char *state1 =
    "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========";
char *state2 =
    "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========";
char *state3 =
    "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========";
char *state4 =
    "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========";
char *state5 =
    "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========";
char *state6 =
    "  +---+\n  |   |\n  O   |\n /|\\  |\n /   |\n      |\n=========";
char *state7 =
    "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========";

char *userChooseWord() {
  char *mot = NULL;
  size_t taille = 0;

  fflush(stdin);

  printf("Entrez un mot : ");
  getline(&mot, &taille, stdin);

  // Supprimez le caractère de nouvelle ligne à la fin de la chaîne.
  size_t longueur = strlen(mot);
  if (longueur > 0 && mot[longueur - 1] == '\n') {
    mot[longueur - 1] = '\0';
  }
  return mot;
}

int checkIfWordExist(char *mot) {
  sqlite3_stmt *stmt;
  const char *insert_sql = "SELECT mot FROM mots WHERE mot like ?";
  int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Impossible de préparer la requête : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_bind_text(stmt, 1, mot, -1, SQLITE_STATIC);

  int rowCount = 0;

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    rowCount++;
  }

  if (rc != SQLITE_DONE) {
    fprintf(stderr,
            "Erreur lors de l'exécution de la requête de verification : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  if (rowCount > 0) {
    fprintf(stderr, "Le mot est présent dans la base de données: %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_finalize(stmt);

  return 0;
}

int insertWord(char *mot) {
  if (checkIfWordExist(mot) == 0) {
    sqlite3_stmt *stmt;
    const char *insert_sql = "INSERT INTO mots (mot) VALUES (?)";
    int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
      fprintf(stderr, "Impossible de préparer la requête : %s\n",
              sqlite3_errmsg(db));
      return 1;
    }

    sqlite3_bind_text(stmt, 1, mot, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
              sqlite3_errmsg(db));
      return 1;
    }

    sqlite3_finalize(stmt);
    fprintf(stdout, "Le mot %s a bien été inséré\n", mot);
    return 0;
  } else {
    fprintf(stderr, "Le mot %s existe déjà\n", mot);
    return 1;
  }
}

int deleteWord(char *mot) {
  if (checkIfWordExist(mot)) {
    sqlite3_stmt *stmt;
    const char *insert_sql = "DELETE FROM mots WHERE mot LIKE (?)";
    int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
      fprintf(stderr, "Impossible de préparer la requête : %s\n",
              sqlite3_errmsg(db));
      return 1;
    }

    sqlite3_bind_text(stmt, 1, mot, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      fprintf(stderr,
              "Erreur lors de l'exécution de la requête de suppression : %s\n",
              sqlite3_errmsg(db));
      return 1;
    }

    sqlite3_finalize(stmt);
    fprintf(stdout, "Le mot %s a bien été supprimé", mot);
    return 0;
  } else {
    fprintf(stderr, "Le mot %s n'existe pas...", mot);
    return 1;
  }
}

int createTableWord() {
  const char *sql = "CREATE TABLE IF NOT EXISTS mots (id INTEGER PRIMARY KEY "
                    "AUTOINCREMENT, mot TEXT);";
  int rc = sqlite3_exec(db, sql, 0, 0, 0);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return 1;
  } else {
    fprintf(stdout, "Done\n");
  }

  return 0;
}

int createTableAsciiPendu() {
  const char *sql = "CREATE TABLE IF NOT EXISTS imgascii (id INTEGER PRIMARY "
                    "KEY AUTOINCREMENT, img TEXT);";
  int rc = sqlite3_exec(db, sql, 0, 0, 0);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return 1;
  } else {
    fprintf(stdout, "Done\n");
  }

  return 0;
}

int insertAscii(char *ascii) {
  sqlite3_stmt *stmt;
  const char *insert_sql = "INSERT INTO imgascii (img) VALUES (?)";
  int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Impossible de préparer la requête : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_bind_text(stmt, 1, ascii, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_finalize(stmt);

  return 0;
}

int countNbLine() {
  sqlite3_stmt *stmt;
  const char *count_sql = "SELECT COUNT(*) FROM mots";

  int rc = sqlite3_prepare_v2(db, count_sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Impossible de préparer la requête : %s\n",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    int rowCount = sqlite3_column_int(stmt, 0);
    printf("Nombre de lignes dans la table : %d\n", rowCount);
  } else {
    fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
            sqlite3_errmsg(db));
  }

  sqlite3_finalize(stmt);
  return 0;
}

int listWords() {

  sqlite3_stmt *stmt;
  const char *select_sql = "SELECT mot FROM mots";

  int rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Impossible de préparer la requête : %s\n",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    const unsigned char *mot = sqlite3_column_text(stmt, 0);
    printf("%s\n", mot);
  }

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_finalize(stmt);

  return 0;
}

int menudb() {
  int result = 100;

  while (result != 0) {
    fprintf(stdout, "\n1: Inser un nouveau mot\n");
    fprintf(stdout, "2: Supprimer un mot\n");
    fprintf(stdout, "3: Lister tous les mots\n");
    fprintf(stdout, "4: Afficher le nombre de mot de la bd\n");
    fprintf(stdout, "0: Retour au menu\n");
    fprintf(stdout, "Que voulez vous faire: ");
    scanf("%d", &result);

    switch (result) {
    case 1:
      system("clear");
      insertWord(userChooseWord());
      break;
    case 2:
      system("clear");
      deleteWord(userChooseWord());
      break;
    case 3:
      system("clear");
      listWords();
      break;
    case 4:
      system("clear");
      countNbLine();
      break;
    case 99:
      insertAscii(state1);
      insertAscii(state2);
      insertAscii(state3);
      insertAscii(state4);
      insertAscii(state5);
      insertAscii(state6);
      insertAscii(state7);
      break;
    case 0:
      return 0;
      break;
    default:
      system("clear");
      fprintf(stderr, "Ce numéro ne correspond à rien");
      break;
    }
  }
  return 0;
}

int databaseConnect() {
  int rc = sqlite3_open("pendu.db", &db);

  if (rc) {
    fprintf(stderr, "Erreur d'ouverture: %s\n", sqlite3_errmsg(db));
    return rc;
  }

  createTableWord();
  createTableAsciiPendu();
  menudb();

  sqlite3_close(db);
  return 0;
}
