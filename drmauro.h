#ifndef DRMAURO_H
#define DRMAURO_H

#define RIGHE 16
#define COLONNE 8
#define RIGHE_NO_MOSTRI 5

#include <stdbool.h>

enum contenuto { VUOTO, MOSTRO, PASTIGLIA };
enum colore { ROSSO, GIALLO, BLU, NUMERO_COLORI };
enum mossa { NONE, DESTRA, SINISTRA, GIU, ROT_DX, ROT_SX };
enum stato { IN_CORSO, VITTORIA, SCONFITTA };
enum senso { ORARIO, ANTIORARIO };
enum orientamento { ORIZZONTALE, VERTICALE };


struct pezzo {
    int riga;
    int colonna;
    enum colore colore;
};

struct pastiglia {
    enum orientamento orientamento;
    struct pezzo pezzo1;
    struct pezzo pezzo2;
    bool attiva;
};

struct cella {
  enum contenuto tipo;
  enum colore colore;
};

struct gioco {
  struct cella campo[RIGHE][COLONNE];
  struct pastiglia pastiglia;
  enum stato stato;
  int punti;
};


void stampa_campo(struct gioco *gioco);
void carica_campo(struct gioco *gioco, char *percorso);
void riempi_campo(struct gioco *gioco, int difficolta);
void step(struct gioco *gioco, enum mossa comando);
enum stato vittoria(struct gioco *gioco);

#endif
