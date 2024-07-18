/*
Giovanni Maria Rava 
*Matricola: 000 111 6816
*Classe A
*Mail: giovannimaria.rava@studio.unibo.it
*
*LOGICA DEL CODICE:
*L'algoritmo che ho implementato prende in input il nome del file come da standard e poi dal file prendi tutti i dati necessari
*per creare un grafo.  
*Viene poi utilizzato l'algoritmo di Dijkastra per il calcolo dei cammini minimi calcolato con la formula data dal professore.
*Stampo poi a video tutto il percorso e infine anche il costo totale.
*
* gcc -std=c90 -Wall -Wpedantic 0001116816.c -o 0001116816
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 250

/* struttura cella */
typedef struct {
    int x, y;
} Cella;

/* struttura Nodo */
typedef struct {
    Cella cella;
    long costo;
} Nodo;

/* struttura per la gestione della coda a priorità */
typedef struct {
    Nodo *dati;
    int dimensione;
    int capacita;
} CodaPriorita;

/* 
 * inizializza la coda di priorità con una capacità specificata
 * cp: puntatore alla coda di priorità
 * capacita: capacità iniziale della coda
 */
void inizializzaCoda(CodaPriorita *cp, int capacita) {
    cp->dati = (Nodo *)malloc(capacita * sizeof(Nodo));
    cp->dimensione = 0;
    cp->capacita = capacita;
}

/*
 * libera la memoria utilizzata dalla coda di priorità
 * cp: puntatore alla coda
 */
void liberaCoda(CodaPriorita *cp) {
    free(cp->dati);
}

/*
 * Inserisco un nodo nella coda di priorità
 * cp: puntatore alla coda di priorità.
 * nodo: nodo da inserire
 */
void inserisciNodo(CodaPriorita *cp, Nodo nodoDaInserire) {
    int i = cp->dimensione++;
    int genitore;
    while (i > 0) {
        genitore = (i - 1) / 2;
        if (cp->dati[genitore].costo <= nodoDaInserire.costo) {
            break;
        }
        cp->dati[i] = cp->dati[genitore];
        i = genitore;
    }
    cp->dati[i] = nodoDaInserire;
}

/*
 * Estrae il nodo con il costo minimo dalla coda di priorità.
 * cp: puntatore alla coda di priorità.
 * Restituisce il nodo con il costo minimo.
 */
Nodo estraiMinimo(CodaPriorita *cp) {
    Nodo nodoMin = cp->dati[0];
    Nodo ultimoNodo = cp->dati[--cp->dimensione];
    int i = 0;
    int minore, sinistra, destra;
    while (1) {
        sinistra = 2 * i + 1;
        destra = 2 * i + 2;
        if (sinistra >= cp->dimensione) break;
        minore = sinistra;
        if (destra < cp->dimensione && cp->dati[destra].costo < cp->dati[sinistra].costo) {
            minore = destra;
        }
        if (cp->dati[minore].costo >= ultimoNodo.costo) {
            break;
        }
        cp->dati[i] = cp->dati[minore];
        i = minore;
    }
    cp->dati[i] = ultimoNodo;
    return nodoMin;
}

/*
 * Controlla se la coda di priorità è vuota.
 * cp: puntatore alla coda di priorità.
 * Restituisce 1 se la coda è vuota, 0 altrimenti.
 */
int codaVuota(CodaPriorita *cp) {
    return cp->dimensione == 0;
}

/*
 * Implemento l'algoritmo di Dijkstra per trovare il percorso di costo minimo in una griglia
 * altezze: matrice delle altezze delle celle
 * n: numero di righe della matrice
 * m: numero di colonne della matrice
 * costoCella: costo fisso per attraversare una cella
 * costoAltezza: costo basato sul dislivello tra celle adiacenti
 * precedente: array per tracciare il percorso ottimale
 * costo: array per mantenere il costo minimo per raggiungere ogni cella
 */
void dijkstra(int **altezze, int n, int m, int costoCella, int costoAltezza, Cella **precedente, long **costo) {
    int direzioneX[] = {-1, 1, 0, 0};
    int direzioneY[] = {0, 0, -1, 1};
    int i,j, uX, uY, vX, vY;
    CodaPriorita cp;
    Nodo nodoIniziale, nuovoNodo;

    inizializzaCoda(&cp, n * m);

    /*iniziallizzo la matrice dei costi e imposta la cella di partenza*/
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            costo[i][j] = LONG_MAX;
        }
    }
    costo[0][0] = costoCella;  /*includo anche il costo della cella iniziale direttamente qui*/

    /*Nodo iniziale*/
    nodoIniziale.cella.x = 0;
    nodoIniziale.cella.y = 0;
    nodoIniziale.costo = costo[0][0];
    inserisciNodo(&cp, nodoIniziale);

    while (!codaVuota(&cp)) {
        Nodo nodoMin = estraiMinimo(&cp);
        uX = nodoMin.cella.x;
        uY = nodoMin.cella.y;

        /*esplora le celle adiacenti*/
        for (i = 0; i < 4; i++) {
            vX = uX + direzioneX[i];
            vY = uY + direzioneY[i];
            /*controllo che le coordinate siano valide, ovvero stiano dentro i limiti della matrice*/
            if (vX >= 0 && vX < n && vY >= 0 && vY < m) {
                long diffAltezza = altezze[uX][uY] - altezze[vX][vY];
                long costoDiffAltezza = costoAltezza * diffAltezza * diffAltezza;
                long nuovoCosto = costo[uX][uY] + costoCella + costoDiffAltezza;
                /*Aggiorno il percorso se ne trovo uno meno costoso*/
                if (nuovoCosto < costo[vX][vY]) {
                    costo[vX][vY] = nuovoCosto;
                    precedente[vX][vY].x = uX;
                    precedente[vX][vY].y = uY;
                    nuovoNodo.cella.x = vX;
                    nuovoNodo.cella.y = vY;
                    nuovoNodo.costo = nuovoCosto;
                    inserisciNodo(&cp, nuovoNodo);
                }
            }
        }
    }
    /*libera tutta la memoria utilizzata*/
    liberaCoda(&cp);
}



/*Stampo tutto il percorso dalla sorgente alla destinazione e infine stampo il costo totale*/
void stampa_percorso(Cella **precedente, long **costo, int n, int m) {
    int x = n - 1;
    int y = m - 1;
    Cella c;
    int *percorsoX = (int *)malloc((n * m) * sizeof(int));
    int *percorsoY = (int *)malloc((n * m) * sizeof(int));
    int indice = 0;
    int i;
    /*stampo il percorso al contrario*/
    while (x != 0 || y != 0) {
        percorsoX[indice] = x;
        percorsoY[indice] = y;
        indice++;
        c = precedente[x][y];
        x = c.x;
        y = c.y;
    }
    /*Aggiungi il punto di partenza all'inizio del percorso*/
    percorsoX[indice] = 0;
    percorsoY[indice] = 0;
    
    /*Stampa il percorso dal punto di partenza al punto di arrivo*/
    for (i = indice; i >= 0; i--) {
        printf("%d %d\n", percorsoX[i], percorsoY[i]);
    }

    printf("-1 -1\n");
    printf("%ld\n", costo[n-1][m-1]);/*Stampa il costo totale del percorso*/

    /*Libero la memoria allocata per i percorsi*/
    free(percorsoX);
    free(percorsoY);
}

int main(int argc, char *argv[]) {
    FILE *file_in;
    int costoCella, costoAltezza, n, m;
    int **altezze;
    Cella **precedente;
    long **costo;
    int i, j;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <file_di_input>\n", argv[0]);
        return EXIT_FAILURE;
    }

    file_in = fopen(argv[1], "r");
    if (!file_in) {
        fprintf(stderr, "Errore nell'apertura del file %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    /*leggo dal file i costi per cella e per altezza, le dimensioni della griglia*/
    fscanf(file_in, "%d", &costoCella);
    fscanf(file_in, "%d", &costoAltezza);
    fscanf(file_in, "%d", &n);
    fscanf(file_in, "%d", &m);
    /*allocazione dinamica della matrice delle altezze*/
    altezze = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        altezze[i] = (int *)malloc(m * sizeof(int));
        for (j = 0; j < m; j++) {
            /*leggo le altezze dalla griglia dal file*/
            fscanf(file_in, "%d", &altezze[i][j]);
        }
    }

    fclose(file_in);
    /*allocazione delle strutture dati per tenere traccia del percorso e dei costi*/
    precedente = (Cella **)malloc(n * sizeof(Cella *));
    costo = (long **)malloc(n * sizeof(long *));
    for (i = 0; i < n; i++) {
        precedente[i] = (Cella *)malloc(m * sizeof(Cella));
        costo[i] = (long *)malloc(m * sizeof(long));
    }

    dijkstra(altezze, n, m, costoCella, costoAltezza, precedente, costo);
    stampa_percorso(precedente, costo, n, m);
    
    /*libero la memoria allocata dinamicamente*/
    for (i = 0; i < n; i++) {
        free(altezze[i]);
        free(precedente[i]);
        free(costo[i]);
    }
    free(altezze);
    free(precedente);
    free(costo);

    return EXIT_SUCCESS;
}
