#ifndef DRMAURO_H
#define DRMAURO_H

#define RIGHE 16
#define COLONNE 8
#define RIGHE_NO_MOSTRI 5

enum contenuto { VUOTO, MOSTRO, PASTIGLIA };
enum colore { ROSSO, GIALLO, BLU, NUMERO_COLORI };
enum mossa { NONE, DESTRA, SINISTRA, GIU, ROT_DX, ROT_SX };
enum stato { IN_CORSO, VITTORIA, SCONFITTA };
enum senso { ORARIO, ANTIORARIO };

struct cella {
  enum contenuto tipo;
  enum colore colore;
  /* ---- */
  int id;
};

struct gioco {
  struct cella campo[RIGHE][COLONNE];
  int punti;
  /* ---- */
  int active_id;
};


void stampa_campo(struct gioco *gioco);
void carica_campo(struct gioco *gioco, char *percorso);
void riempi_campo(struct gioco *gioco, int difficolta);
void step(struct gioco *gioco, enum mossa comando);
enum stato vittoria(struct gioco *gioco);

#endif
