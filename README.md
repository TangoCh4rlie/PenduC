# Pendu en C
Ce pendu a été programmé exclusivement en C et connecté à une base de donnée SQLite (qui a la pire doc ever pour le connecteur C ).

## Compilation
```gcc src/dbAction.c src/dbFunctions.c src/main.c -o ../pendu -lsqlite3```
