#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

char *userChooseWord() {
  char *mot = NULL;
  size_t taille = 0;

  printf("Entrez un mot : ");
  getline(&mot, &taille, stdin);

  // Supprimez le caractère de nouvelle ligne à la fin de la chaîne.
  size_t longueur = strlen(mot);
  if (longueur > 0 && mot[longueur - 1] == '\n') {
    mot[longueur - 1] = '\0';
  }
  return mot;
}

int checkIfWordExist(sqlite3 *db, char *mot) {
  sqlite3_stmt *stmt;
  const char *insert_sql = "SELECT mot FROM mots WHERE mot like (?)";
  int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Impossible de préparer la requête : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_bind_text(stmt, 1, mot, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    int rowCount = sqlite3_column_int(stmt, 0);

    if (rowCount > 0) {
      printf("Le mot est déjà présent dans la base de données\n");
      return 1;
    }
  } else {
    fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
            sqlite3_errmsg(db));
    return 1;
  }

  sqlite3_finalize(stmt);

  return 0;
}

int insertWord(sqlite3 *db, char *mot) {
  if (checkIfWordExist(db, mot)) {
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

int deleteWord(sqlite3 *db, char *mot) {
  if (checkIfWordExist(db, mot)) {
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
      fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
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

int createTableWord(sqlite3 *db) {
  const char *sql = "CREATE TABLE IF NOT EXISTS mots (id INTEGER PRIMARY KEY "
                    "AUTOINCREMENT, mot TEXT);";
  int rc = sqlite3_exec(db, sql, 0, 0, 0);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return 1;
  } else {
    fprintf(stdout, "Table created successfully\n");
  }

  return 0;
}

int countNbLine(sqlite3 *db) {
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

int databaseConnect() {
  sqlite3 *db;
  int rc = sqlite3_open("pendu.db", &db);

  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return rc;
  }

  createTableWord(db);
  insertWord(db, userChooseWord());
  countNbLine(db);

  sqlite3_close(db);
  return 0;
}
