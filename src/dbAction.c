#include <sqlite3.h>
#include <stdio.h>

sqlite3 *db;

int openDb() {
  int rc = sqlite3_open("pendu.db", &db);

  if (rc) {
    fprintf(stderr, "Erreur d'ouverture: %s\n", sqlite3_errmsg(db));
    return 1;
  }
  return 0;
}

int closeDb() {
  sqlite3_close(db);
  return 0;
}

char *randomWord() {
  openDb();

  sqlite3_stmt *stmt;
  const char *select_sql = "SELECT mot FROM mots ORDER BY RANDOM() LIMIT 1";

  int rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Impossible de préparer la requête : %s\n",
            sqlite3_errmsg(db));
    sqlite3_close(db);
  }

  rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    return (char *)sqlite3_column_text(stmt, 0);
  }

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n",
            sqlite3_errmsg(db));
  }

  sqlite3_finalize(stmt);

  closeDb();
  return 0;
}
