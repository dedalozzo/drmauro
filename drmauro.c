#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "drmauro.h"


/**
 * @brief Assegna un nuovo colore al mostro presente alle coordinate `x`, `y` del campo di gioco.
 * @param gioco Puntatore all'istanza del gioco.
 * @param x Ascissa del mostro.
 * @param y Ordinata del mostro.
 */
void assegna_nuovo_colore(struct gioco *gioco, int x, int y) {
    int colore_attuale = gioco->campo[x][y].colore;
    int nuovo_colore;

    do {
        nuovo_colore = rand() % (NUMERO_COLORI + 1);
    } while (nuovo_colore == colore_attuale);

    gioco->campo[x][y].colore = (enum colore) nuovo_colore;
}


/**
 * @brief Riorgannizza i mostri per evitare che vi siano più di due mostri adiacenti del medesimo colore nella stessa
 * fila. La regola vale sia per le righe che per le colonne.
 * @param gioco Puntatore all'istanza del gioco.
 */
void riorganizza_mostri(struct gioco *gioco) {
    int i, j;

    int prima_riga = RIGHE_NO_MOSTRI + 2;
    int prima_colonna = 2;

    for (i = prima_riga; i < RIGHE; i++) {
        for (j = prima_colonna; j < COLONNE; j++) {
            int colore = gioco->campo[i][j].colore;

            if ((colore == gioco->campo[i][j-1].colore && colore == gioco->campo[i][j-2].colore) ||
                (colore == gioco->campo[i-1][j].colore && colore == gioco->campo[i-2][j].colore)) {
                assegna_nuovo_colore(gioco, i, j);
            }
        }
    }
}


/**
 * @brief Inizializza eventuali caselle vuote nella riga, qualora lo schema termini con una nuova linea prima
 * del dovuto.
 * @param gioco Puntatore all'istanza del gioco.
 * @param x Ascissa della cella corrente.
 * @param y Ordinata della cella corrente.
 */
void completa_riga(struct gioco *gioco, int x, int y) {
    for (int i = x; i < RIGHE; i++) {
        gioco->campo[i][y].tipo = VUOTO;
    }
}


/**
 * @brief Inizializza eventuali caselle vuote nel campo, qualora lo schema termini con una nuova linea prima
 * del dovuto.
 * @param gioco Puntatore all'istanza del gioco.
 * @param x Ascissa della cella corrente.
 * @param y Ordinata della cella corrente.
 */
void completa_campo(struct gioco *gioco, int x, int y) {
    for (int i = y; i < COLONNE; i++) {
        completa_riga(gioco, x, i);
        x = 0;
    }
}


/**
  * @brief Carica il campo gioco leggendone lo schema da un file di testo passato a linea di comando con l'opzione `-f`.
  * @details Il file contiene tante righe quante il campo da gioco. Ogni riga può esclusivamente contenere i seguenti
  * caratteri:\n
  *   - `R` identifica un virus rosso;\n
  *   - `G` identifica un virus giallo;\n
  *   - `B` identifica un virus blu;\n
  *   - lo spazio, identifica una cella vuota.\n
  * La funzione verifica sia la presenza di caratteri non consentiti, nel cui caso ritorna un errore e termina
  * l'esecuzione del programma.\n
  * La funzione accetta il file anche in caso di mancato inserimento di eventuali spazi prima del fine linea, quando
  * ad esempio non è indicato il colore del mostro per l'ultima cella di una riga qualunque.\n
  * Lo schema viene poi riorganizzato per prevenire che siano stati inseriti più di due mostri consecutivi dello stesso
  * colore. Si è preferito utilizzare questo approccio invece che generare un errore di schema invalido. Le specifiche,
  * infatti, non fanno menzione a nessun controllo, assumendo che il file contenente lo schema del campo sia
  * necessariamente corretto. L'implementazione corrente consente una maggiore elasticità.
  * @param gioco Puntatore all'istanza del gioco.
  * @param percorso Path del file di testo da cui caricare la posizione dei mostri sul campo da gioco.
  */
void carica_campo(struct gioco *gioco, char *percorso) {
    FILE *fp = fopen(percorso, "r");

    // Nel caso il file non si riesca ad aprire, stampa a video un errore e termina l'esecuzione del programma.
    if (!fp) {
        fprintf(stderr, "Non è stato possibile aprire il file.\n");
        exit(1);
    }

    // Variabile usata per memorizzare il carattere letto.
    int carattere;

    // Coordinate cartesiane della cella nella matrice del campo.
    int x = 0, y = 0;

    do {
        carattere = fgetc(fp);

        switch (carattere) {
            case 'R':
                gioco->campo[x][y].tipo = MOSTRO;
                gioco->campo[x][y].colore = ROSSO;
                x++;
                break;
            case 'G':
                gioco->campo[x][y].tipo = MOSTRO;
                gioco->campo[x][y].colore = GIALLO;
                x++;
                break;
            case 'B':
                gioco->campo[x][y].tipo = MOSTRO;
                gioco->campo[x][y].colore = BLU;
                x++;
                break;
            case ' ':
                gioco->campo[x][y].tipo = VUOTO;
                x++;
                break;
            case '\n':
                completa_riga(gioco, x, y);
                y++;
                x = 0;
                break;
            case EOF:
                completa_campo(gioco, x, y);
                break;
            default:
                fprintf(stderr, "Il file contiene un carattere invalido.\n");
                exit(1);
        }

    } while (carattere != EOF);

    fclose(fp);

    riorganizza_mostri(gioco);
}


/**
 * @brief Inizializza un vettore assegnando ad ogni elemento un valore compreso tra 0 e 2.
 * @param vettore Un vettore di interi.
 * @param n Dimensione del vettore.
 */
void inizializza_vettore(int *vettore, int n) {
    for (int i = 0; i < n; i++) {
        vettore[i] = (rand() % 3); // Assegna un numero compreso tra 0 e 2 alla cella, che rappresenta il tipo di mostri.
    }
}


/**
 * @brief Mescola un vettore.
 * @param vettore Un vettore di interi.
 * @param n Dimensione del vettore.
 * @details La funzione utilizza l'algoritmo Fisher–Yates shuffle.
 * @see https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
 */
void mescola_vettore(int *vettore, int n) {
    int i, j, tmp;

    for (i = n - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = vettore[j];
        vettore[j] = vettore[i];
        vettore[i] = tmp;
    }
}


/**
 * @brief Assegna il valore `-1` ad un numero `r` di elementi del vettore scelti casualmente.
 * @param vettore Un vettore di interi.
 * @param n Dimensione del vettore.
 * @param r Numero di elementi da rimuovere.
 */
void sfronda_vettore(int *vettore, int n, int r) {
    int k = 0;

    while (k < n) {
        int i = rand() % r;

        if (vettore[i] == -1)
            continue;

        vettore[i] = -1; // -1 indica che il mostro non è stato rimosso dal vettore.

        k++;
    }
}


/**
 * @brief Assegna i mostri al campo di gioco.
 * @param gioco Puntatore all'istanza del gioco.
 * @param vettore Un vettore di interi.
 */
void assegna_mostri(struct gioco *gioco, int *vettore) {
    int i, j;

    // Le celle del prime 5 righe del campo sono vuote poiché non possono contenere mostri.
    for (i = 0; i < RIGHE_NO_MOSTRI; i++) {
        for (j = 0; j < COLONNE; j++)
            gioco->campo[i][j].tipo = VUOTO;
    }

    // Alle altre celle del campo da gioco viene assegnato il mostri.
    int k = 0;

    for (i = RIGHE_NO_MOSTRI; i < RIGHE; i++) {
        for (j = 0; j < COLONNE; j++) {

            if (vettore[k] != -1) {
                gioco->campo[i][j].tipo = VUOTO;
                gioco->campo[i][j].colore = (enum colore) vettore[k];
            }
            else {
                gioco->campo[i][j].tipo = VUOTO;
            }

            k++;
        }
    }
}


/**
 * @brief Inizializza il campo da gioco posizionando i mostri a partire dalla sesta riga.
 * @details I mostri vengono distribuiti sul campo usando la seguente strategia:\n
 *   - crea un vettore con un numero di elementi pari alle celle della matrice del campo utilizzabili per l'assegnazione
 *   dei mostri, dunque escluse le prime cinque righe della stessa;\n
 *   - inizializza il vettore assegnando ad ogni elemento un valore compreso tra 0 e 2, il quale rappresenta il tipo di
 *   mostri;\n
 *   - mescola il vettore utilizzando l'algoritmo Fisher-Yates shuffle;\n
 *   - "rimuove" dal vettore i mostri in surplus, scengliendo casualmente a quali degli elementi assegnare il valore -1.\n
 *   - verifica che non siano presenti due mostri adiacenti sulla stessa fila.\n
 * L'algoritmo si assicura inoltre che non siano presenti due mostri dello stesso tipo adiacenti, ovverosia due mostri
 * identificati dallo stesso colore, uno dietro l'altro.
 * @param gioco Puntatore all'istanza del gioco.
 * @param difficolta Livello di difficoltà scelto per il gioco. Varia tra 0 e 15.
 * @note L'algoritmo assume che non possano esservi piu di due mostri consecutivi, del medesimo tipo, sulla stessa riga
 * o colonna. Le specifiche in merito sono poche chiare, visto che si parla di fila. Dalle immagine ho desunto che la
 * verifica vada fatta sia sulla riga che sulla colonna.
 */
void riempi_campo(struct gioco *gioco, int difficolta) {
    // Verifica che il livello di difficoltà sia compreso tra 0 e 15. Se non lo è ritorna un errore.
	assert(difficolta >= 0 && difficolta <= 15);

	// Numero di celle disponibili nel campo da gioco. Le prime 5 righe infatti non vanno usate.
	const int num_celle = (RIGHE * COLONNE) - (RIGHE_NO_MOSTRI * COLONNE);

	// Numero di mostri da rimuovere, che è dato dalla differenza tra il numero totale di celle ed il numero
	// di mostri calcolato in base alla difficoltà scelta.
	const int num_mostri = num_celle - (4 * (difficolta + 1));

	// Crea il vettore che contiene le celle del campo da gioco.
	int celle[num_celle];

	// Utilizza `srand()` con `time(NULL)` affinché l'allocazione non sia la medesima ogni volta che si esegue il gioco.
	srand(time(NULL));

    inizializza_vettore(celle, num_celle);
    sfronda_vettore(celle, num_celle, num_mostri);
    mescola_vettore(celle, num_celle);
    assegna_mostri(gioco, celle);
    riorganizza_mostri(gioco);
}


void step(struct gioco *gioco, enum mossa comando) {

}


enum stato vittoria(struct gioco *gioco) {
	return IN_CORSO;
}
