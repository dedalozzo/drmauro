#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "drmauro.h"


/**
 * @brief Ritorna un colore a caso tra quelli disponibili.
 * @details I suddetti colori vengono usati sia per i mostri che per le pastiglie.
 * @return int
 */
int genera_nuovo_colore() {
    return rand() % NUMERO_COLORI;
}


/**
 * @brief Assegna un nuovo colore al mostro presente alle coordinate (x, y) del campo di gioco.
 * @param gioco Puntatore all'istanza del gioco.
 * @param r Riga in cui assegnare il mostro.
 * @param c Colonna in cui assegnare il mostro.
 */
void cambia_colore_mostro(struct gioco *gioco, int r, int c) {
    int colore_attuale = gioco->campo[r][c].colore;
    int nuovo_colore;

    do {
        nuovo_colore = genera_nuovo_colore();
    } while (nuovo_colore == colore_attuale);

    gioco->campo[r][c].colore = (enum colore) nuovo_colore;
}


/**
 * @brief Riorgannizza i mostri per evitare che vi siano più di due mostri adiacenti del medesimo colore nella stessa
 * fila. La regola vale sia per le righe che per le colonne.
 * @param gioco Puntatore all'istanza del gioco.
 */
void riorganizza_mostri(struct gioco *gioco) {
    int i, j;

    int prima_riga = RIGHE_NO_MOSTRI;
    int prima_colonna = 2;

    for (i = prima_riga; i < RIGHE; i++) {
        for (j = prima_colonna; j < COLONNE; j++) {

            // Se non è presente un mostro nella cella, allora continua.
            if (gioco->campo[i][j].tipo == VUOTO)
                continue;

            // Colore del mostro della presente cella.
            int colore = gioco->campo[i][j].colore;

            // Vero se, sulla medesima riga, i due mostri nelle celle adiacenti sono dello stesso colore.
            bool trio_riga = gioco->campo[i][j-1].tipo == MOSTRO && colore == gioco->campo[i][j-1].colore &&
                                  gioco->campo[i][j-2].tipo == MOSTRO && colore == gioco->campo[i][j-2].colore;

            // Vero se, sulla medesima colonna, i due mostri nelle celle adiacenti sono dello stesso colore.
            bool trio_colonna = gioco->campo[i-1][j].tipo == MOSTRO && gioco->campo[i-1][j].colore &&
                                     gioco->campo[i-2][j].tipo == MOSTRO && colore == gioco->campo[i-2][j].colore;

            // Se il mostro della presente cella è del medesimo colore di quelli delle due celle precedenti della stessa
            // riga o colonna, allora assegna al mostro un altro colore.
            if ((trio_riga || trio_colonna)) {
                cambia_colore_mostro(gioco, i, j);
            }
        }
    }
}


/**
 * @brief Ritorna la lettera corrispondente al colore.
 * @param colore Uno dei colori di mostri e pastiglie.
 */
char lettera_colore(enum colore colore) {
    switch (colore) {
        case ROSSO:
            return 'R';
        case GIALLO:
            return 'G';
        case BLU:
            return 'B';
        default:
            fprintf(stderr, "Colore non supportato.\n");
            exit(1);
    }
}


/**
 * @brief Stampa il campo di gioco.
 * @param gioco Puntatore all'istanza del gioco.
 */
void stampa_campo(struct gioco *gioco) {
    // Stampa l'intestazione.
    for (int j = 0; j < COLONNE; j++)
        printf("=");

    printf("\nCAMPO\n");

    for (int j = 0; j < COLONNE; j++)
        printf("=");

    printf("\n");

    // Stampa il campo di gioco vero e proprio.
    for (int i = 0; i < RIGHE; i++) {

        // Stampa una riga di celle.
        for (int j = 0; j < COLONNE; j++) {
            // Se nella cella c'è un mostro, stampa la lettera che corrisponde al colore, as esempio `R`. In caso
            // contrario stampa uno spazio.
            if (gioco->campo[i][j].tipo == MOSTRO)
                printf("%c", lettera_colore(gioco->campo[i][j].colore));
            else
                printf(" ");
        }

        printf("\n");
    }
}


/**
 * @brief Inizializza eventuali caselle vuote nella riga, qualora lo schema termini con una nuova linea prima
 * del dovuto.
 * @param gioco Puntatore all'istanza del gioco.
 * @param r Riga corrente.
 * @param c Colonna corrente.
 */
void completa_riga(struct gioco *gioco, int r, int c) {
    for (int i = c; i < COLONNE; i++) {
        gioco->campo[r][i].tipo = VUOTO;
    }
}


/**
 * @brief Inizializza eventuali caselle vuote nel campo, qualora lo schema termini con una nuova linea prima
 * del dovuto.
 * @param gioco Puntatore all'istanza del gioco.
 * @param r Riga corrente.
 * @param c Colonna corrente.
 */
void completa_campo(struct gioco *gioco, int r, int c) {
    for (int i = r; i < RIGHE; i++) {
        completa_riga(gioco, i, c);
        c = 0;
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

    // Coordinate cartesiane della cella nella matrice che rappresenta il campo di gioco.
    int r = 0, c = 0;

    do {
        carattere = fgetc(fp);

        switch (carattere) {
            case 'R':
                gioco->campo[r][c].tipo = MOSTRO;
                gioco->campo[r][c].colore = ROSSO;
                c++;
                break;
            case 'G':
                gioco->campo[r][c].tipo = MOSTRO;
                gioco->campo[r][c].colore = GIALLO;
                c++;
                break;
            case 'B':
                gioco->campo[r][c].tipo = MOSTRO;
                gioco->campo[r][c].colore = BLU;
                c++;
                break;
            case ' ':
                gioco->campo[r][c].tipo = VUOTO;
                c++;
                break;
            case '\n':
                completa_riga(gioco, r, c);
                r++;
                c = 0;
                break;
            case EOF:
                completa_campo(gioco, r, c);
                break;
            default:
                fprintf(stderr, "Il file contiene un carattere invalido.\n");
                exit(1);
        }

    } while (carattere != EOF);

    fclose(fp);

    riorganizza_mostri(gioco);

    stampa_campo(gioco);
}


/**
 * @brief Inizializza un vettore assegnando ad ogni elemento un valore compreso tra 0 e 2.
 * @param vettore Un vettore di interi.
 * @param n Dimensione del vettore.
 */
void inizializza_vettore(int *vettore, int n) {
    for (int i = 0; i < n; i++) {
        // Assegna un numero compreso tra 0 e 2 alla cella, che rappresenta il colore del mostro.
        vettore[i] = (rand() % 3);
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

    while (k < r) {
        int i = rand() % n;

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
void assegna_mostri(struct gioco *gioco, const int *vettore) {
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
                gioco->campo[i][j].tipo = MOSTRO;
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

	// Dichiara il vettore che contiene le sole celle disponibili del campo da gioco. Questo vettore verrà usato per
	// l'assegnazione dei mostri al campo di gioco.
	int celle[num_celle];

    inizializza_vettore(celle, num_celle);
    sfronda_vettore(celle, num_celle, num_mostri);
    mescola_vettore(celle, num_celle);

    assegna_mostri(gioco, celle);
    stampa_campo(gioco);
    riorganizza_mostri(gioco);
    stampa_campo(gioco);
}


/**
 * @brief Scambia il colore dei due pezzi che compongono la pastiglia.
 * @param pastiglia Una pastiglia.
 */
void scambia_colore(struct pastiglia *pastiglia) {
    enum colore tmp = pastiglia->pezzo1.colore;
    pastiglia->pezzo1.colore = pastiglia->pezzo2.colore;
    pastiglia->pezzo2.colore = tmp;
}


/**
 * @brief Rielabora il campo di gioco.
 * @details A seguito di ogni mossa valida il campo va aggiornato, poiché la ricollocazione di una pastiglia può aver
 * determinato l'eliminazione di un mostro e d'altri pezzi di pastiglie.
 * @param gioco Puntatore all'istanza del gioco.
 */
void rielabora_campo(struct gioco *gioco) {

}


/**
 * @brief Cambia il contenuto dei pezzi che costituiscono la pastiglia attiva.
 * @param gioco Puntatore all'istanza del gioco.
 */
void cambia_tipo(struct gioco *gioco, enum contenuto tipo) {
    struct pastiglia *p = &gioco->pastiglia;

    if (!p->attiva)
        return;

    gioco->campo[p->pezzo1.riga][p->pezzo1.colonna].tipo = tipo;
    gioco->campo[p->pezzo2.riga][p->pezzo2.colonna].tipo = tipo;
};


/**
 * @brief Rimuove la pastiglia corrente dal campo.
 * @param gioco Puntatore all'istanza del gioco.
 */
void rimuovi_pastiglia(struct gioco *gioco) {
    cambia_tipo(gioco, VUOTO);
}


/**
 * @brief Ripristina la pastiglia corrente sul campo.
 * @param gioco Puntatore all'istanza del gioco.
 */
void ripristina_pastiglia(struct gioco *gioco) {
    cambia_tipo(gioco, PASTIGLIA);
}


/**
 * @brief Aggiorna il campo da gioco, riposizionando i due pezzi che compongono la posizione della
 * pastiglia sul campo di gioco.
 * @param gioco Puntatore all'istanza del gioco.
 */
void aggiorna_campo(struct gioco *gioco) {
    if (!gioco->aggiorna_campo)
        return;

    gioco->aggiorna_campo = false;

    struct pastiglia *pastiglia = &gioco->pastiglia;
    struct pastiglia *temp = &gioco->pastiglia_temp;

    // Vecchie coordinate dei pezzi di pastiglia.
    int or1 = pastiglia->pezzo1.riga;
    int oc1 = pastiglia->pezzo1.colonna;
    int or2 = pastiglia->pezzo2.riga;
    int oc2 = pastiglia->pezzo2.colonna;

    // Nuove coordinate dei pezzi di pastiglia.
    int r1 = temp->pezzo1.riga;
    int c1 = temp->pezzo1.colonna;
    int r2 = temp->pezzo2.riga;
    int c2 = temp->pezzo2.colonna;

    // Rimuove temporaneamente la pastiglia, così quando verifica il campo non vi sono interferenze.
    rimuovi_pastiglia(gioco);

    // Se la pastiglia è fuori campo significa che la mossa è invalida, dunque il controllo ritorna alla funzione
    // chiamante.
    if (r1 > RIGHE-1 || c1 > COLONNE-1 || r2 > RIGHE-1 || c2 > COLONNE-1 || c1 < 0 || c2 < 0) {
        ripristina_pastiglia(gioco);
        return;
    }

    // La mossa è invalida quando anche solo un pezzo della pastiglia occupa una cella che non è vuota.
    // (r1, c1) e (r2, c2) sono le coordinate delle celle che la pastiglia dovrebbe andare ad occupare.
    // Il controllo `r2 >= 0` serve ad evitare che il programma termini nel qual caso la pastiglia sia in verticale
    // e sfori dalla griglia. `r2` infatti potrebbe essere uguale a `-1`.
    // fuori dalla griglia.
    if ((gioco->campo[r1][c1].tipo != VUOTO) || (r2 >= 0 && gioco->campo[r2][c2].tipo != VUOTO)) {

        // In questo caso viene fatto un ulteriore controllo per verificare che la pastiglia si trovi proprio nel
        // mezzo della prima riga. Se così, in virtù del fatto che le celle sono già occupate, la partita è persa,
        // pertanto lo stato del gioco viene modificato.
        if (r1 == 0 && c1 == (COLONNE / 2) - 1) {
            gioco->stato = SCONFITTA;
        }

        pastiglia->attiva = false;
        ripristina_pastiglia(gioco);

        return;
    }


    // RICOLLOCA EFFETTIVAMENTE LA PASTIGLIA

    // Elimina i pezzi di pastiglia dal campo, poiché la pastiglia ha cambiato posizione.
    gioco->campo[or1][oc1].tipo = VUOTO;
    gioco->campo[or2][oc2].tipo = VUOTO;

    // Inserisce il primo pezzo di pastiglia nella sua nuova posizione sul campo di gioco.
    gioco->campo[r1][c1].tipo = PASTIGLIA;
    //gioco->campo[r1][c1].tipo = VUOTO;
    gioco->campo[r1][c1].colore = temp->pezzo1.colore;

    // Se il secondo pezzo della pastiglia non è fuori campo allora inserisce anche il secondo pezzo sul campo.
    if (r2 >= 0) {
        gioco->campo[r2][c2].tipo = PASTIGLIA;
        //gioco->campo[r2][c2].tipo = VUOTO;
        gioco->campo[r2][c2].colore = temp->pezzo2.colore;
    }


    // Una pastiglia viene disattivata quando arriva a fine corsa, dunque nei seguenti casi:
    //   - si trova sul fondo, ovverosia sull'ultima riga del campo di gioco;
    //   - sotto al primo pezzo della pastiglia non c'è una cella vuota, bensì un mostro o un altro pezzo di pastiglia;
    //   - quando l'orientamento della pastiglia è orizzontale e sotto al secondo pezzo della medesima non c'è una
    //     cella vuota.
    if (r1 == RIGHE - 1 ||
            (gioco->campo[r1+1][c1].tipo != VUOTO) ||
            (temp->orientamento == ORIZZONTALE && gioco->campo[r2+1][c2].tipo != VUOTO)) {
        temp->attiva = false;
    }

    // Assegna alla pastiglia del gioco la copia modificata.
    memcpy(pastiglia, temp, sizeof(struct pastiglia));

    gioco->stato = IN_CORSO;

    if (!pastiglia->attiva)
        rielabora_campo(gioco);
}


/**
 * @brief Ruota la pastiglia in senso orario o antiorario.
 * @details La rotazione avviene sempre mantenendo l'ascissa costante, tranne nel caso in cui
 * @param gioco Puntatore all'istanza del gioco.
 * @param senso_rotazione Senso di rotazione.
 */
void ruota_pastiglia(struct gioco *gioco, enum senso senso_rotazione) {
    if (!gioco->pastiglia.attiva) {
        gioco->aggiorna_campo = false;
        return;
    }

    // Memorizza la pastiglia corrente in una variabile temporanea.
    struct pastiglia temp = gioco->pastiglia;

    switch (gioco->pastiglia.orientamento) {
        case ORIZZONTALE:
            // Il primo pezzo della pastiglia rimane al suo posto mentre il secondo pezzo viene
            // spostato nella riga sopra il primo pezzo.
            temp.pezzo2.riga--;
            temp.pezzo2.colonna--;

            if (senso_rotazione == ORARIO)
                scambia_colore(&temp);

            temp.orientamento = VERTICALE;

            break;
        case VERTICALE:
            temp.pezzo2.riga++;
            temp.pezzo2.colonna++;

            // Se il secondo pezzo di pastiglia finisce in una cella che non è vuota, allora shifta a sinistra
            // l'intera pastiglia di una colonna.
            if (gioco->campo[temp.pezzo2.riga][temp.pezzo2.colonna].tipo != VUOTO) {
                temp.pezzo1.colonna--;
                temp.pezzo2.colonna--;
            }

            if (senso_rotazione == ANTIORARIO)
                scambia_colore(&temp);

            temp.orientamento = ORIZZONTALE;

            break;
    }

    gioco->pastiglia_temp = temp;
    gioco->aggiorna_campo = true;
}


/**
 * @brief Sposta la pastiglia.
 * @details Il giocatore può muovere la pastiglia a destra e sinistra, oppure farla scendere di una posizione o ancora
 * farla cadere verso il basso sino a che non incontra un'altra pastiglia, un virus o il fondo del campo di gioco.
 * @param gioco Puntatore all'istanza del gioco.
 * @param direzione La direzione in cui la pastiglia dovrebbe spostarsi.
 */
void sposta_pastiglia(struct gioco *gioco, enum mossa direzione) {
    if (!gioco->pastiglia.attiva) {
        gioco->aggiorna_campo = false;
        return;
    }

    // Memorizza la pastiglia corrente in una variabile temporanea.
    struct pastiglia temp = gioco->pastiglia;

    switch (direzione) {
        case DESTRA:
            temp.pezzo1.colonna++;
            temp.pezzo2.colonna++;
            break;
        case SINISTRA:
            temp.pezzo1.colonna--;
            temp.pezzo2.colonna--;
            break;
        case GIU: {
                int i = temp.pezzo1.riga + 1;

                while (i < RIGHE) {
                    // Se il posto della pastiglia non è vuoto, allora si ferma.
                    if (gioco->campo[i][temp.pezzo1.colonna].tipo != VUOTO && gioco->campo[i][temp.pezzo2.colonna].tipo != VUOTO)
                        break;

                    i++;
                }

                // Sposta verso il basso le pastiglie.
                temp.pezzo1.riga = i - 1;

                if (temp.orientamento == ORIZZONTALE)
                    temp.pezzo2.riga = i - 1;
                else
                    temp.pezzo2.riga = i - 2;
            }
            break;
        case NONE:
            temp.pezzo1.riga++;
            temp.pezzo2.riga++;
            break;
        default:
            return;
    }

    gioco->pastiglia_temp = temp;
    gioco->aggiorna_campo = true;
}


/**
 * @brief Crea una nuova pastiglia.
 * @details L'orientamento di ogni nuova pastiglia è sempre orizzontale.
 * @param gioco Puntatore all'istanza del gioco.
 */
void crea_pastiglia(struct gioco *gioco) {
    gioco->pastiglia.orientamento = ORIZZONTALE;

    // L'ascissa è `-1` poiché la pastiglia viene immediatamente spostata nella prima riga del campo.
    gioco->pastiglia.pezzo1.riga = -1;
    gioco->pastiglia.pezzo2.riga = -1;

    gioco->pastiglia.pezzo1.colonna = (COLONNE / 2) - 1;
    gioco->pastiglia.pezzo2.colonna = gioco->pastiglia.pezzo1.colonna + 1;

    gioco->pastiglia.pezzo1.colore = (enum colore) genera_nuovo_colore();
    gioco->pastiglia.pezzo2.colore = (enum colore) genera_nuovo_colore();

    gioco->pastiglia.attiva = true;
}


/**
 * @brief Implementa la logica del videogioco.
 * @details Il giocatore può muovere la pastiglia a destra e sinistra, ruotarla in senso orario e antiorario, farla
 * cadere verso il basso, oppure non fare nulla, nel qual caso il programma farà scendere verso il basso la pastiglia
 * attiva di una posizione. Se non vi è nessuna pastiglia attiva la funzione ne crea una e la posiziona a metà della
 * prima riga.
 * @param gioco Puntatore all'istanza del gioco.
 * @param comando Comando impartito dal giocatore.
 */
void step(struct gioco *gioco, enum mossa comando) {

    switch (comando) {
        case DESTRA:
            sposta_pastiglia(gioco, DESTRA);
            break;

        case SINISTRA:
            sposta_pastiglia(gioco, SINISTRA);
            break;

        case GIU:
            sposta_pastiglia(gioco, GIU);
            break;

        case ROT_DX:
            ruota_pastiglia(gioco, ORARIO);
            break;

        case ROT_SX:
            ruota_pastiglia(gioco, ANTIORARIO);
            break;

        case NONE:
            if (!gioco->pastiglia.attiva)
                crea_pastiglia(gioco);
            else
                sposta_pastiglia(gioco, NONE);
            break;

        default:
            break;
    }

}


/**
 * @brief Ritorna lo stato del gioco.
 * @return stato
 * @param gioco Puntatore all'istanza del gioco.
 */
enum stato vittoria(struct gioco *gioco) {
	return gioco->stato;
}
