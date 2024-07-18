#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* struttura grafo */
typedef struct {
    int n;              /* numero di nodi               */
    int m;              /* numero di archi              */
} Grafo;
typedef struct {
    int x;
    int y;
}cella:
typedef struct{
    long double costo_H;
    cella *cella;
}Nodo;
Grafo *graph_read_from_file(FILE *file_in)
{
    int n, m, costo_fisso, costo_variabile;
    int src, dst;
    int i; /* numero archi letti dal file */
    double weight;
    Grafo *g;

    assert(file_in != NULL);

    if (fscanf(file_in, "%lf", &costo_fisso) != 1 || fscanf(file_in, "%lf", &costo_variabile) != 1) {
        fprintf(stderr, "Errore nella lettura dei costi\n");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    // Lettura del numero di righe e colonne
    if (fscanf(file_in, "%d", &n) != 1 || fscanf(file_in, "%d", &m) != 1) {
        fprintf(stderr, "Errore nella lettura del numero di righe e colonne\n");
        fclose(file_in);
        return EXIT_FAILURE;
    }
    assert( n > 0 );
    assert( m >= 0 );

    g = graph_create(n, t);
    /* Ciclo di lettura degli archi. Per rendere il programma più
       robusto, meglio non fidarsi del valore `m` nell'intestazione
       dell'input. Leggiamo informazioni sugli archi fino a quando ne
       troviamo, e poi controlliamo che il numero di archi letti (i)
       sia uguale a quello dichiarato (m) */
    i = 0;
    while (3 == fscanf(f, "%d %d %lf", &src, &dst, &weight)) {
        graph_add_edge(g, src, dst, weight);
        i++;
    }
    if (i != m) {
        fprintf(stderr, "WARNING: ho letto %d archi, ma l'intestazione ne dichiara %d\n", i, m);
    }
    /*
    fprintf(stderr, "INFO: Letto grafo %s con %d nodi e %d archi\n",
            (t == GRAPH_UNDIRECTED) ? "non orientato" : "orientato",
            n,
            m);
    */
    return g;
}



#include <stdio.h>
#include <stdlib.h>

#define MAX_N 250
#define MAX_M 250

// Funzione per leggere la matrice da un file
void read_matrix(const char *filename, int *n, int *m, int matrix[MAX_N][MAX_M]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Errore nell'apertura del file.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", n, m);
    for (int i = 0; i < *n; ++i) {
        for (int j = 0; j < *m; ++j) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
}

// Funzione per costruire la matrice di adiacenza
void build_adjacency_matrix(int n, int m, int matrix[MAX_N][MAX_M], int adj[MAX_N * MAX_M][MAX_N * MAX_M]) {
    for (int i = 0; i < n * m; ++i) {
        for (int j = 0; j < n * m; ++j) {
            adj[i][j] = 0;
        }
    }

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int u = i * m + j;

            for (int d = 0; d < 4; ++d) {
                int ni = i + directions[d][0];
                int nj = j + directions[d][1];

                if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                    int v = ni * m + nj;
                    adj[u][v] = 1; // Indica la presenza di un arco da u a v
                }
            }
        }
    }
}

// Funzione per stampare la matrice di adiacenza
void print_adjacency_matrix(int n, int m, int adj[MAX_N * MAX_M][MAX_N * MAX_M]) {
    for (int i = 0; i < n * m; ++i) {
        for (int j = 0; j < n * m; ++j) {
            printf("%d ", adj[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int matrix[MAX_N][MAX_M];
    int n, m;

    read_matrix(argv[1], &n, &m, matrix);

    int adj[MAX_N * MAX_M][MAX_N * MAX_M];
    build_adjacency_matrix(n, m, matrix, adj);

    print_adjacency_matrix(n, m, adj);

    return 0;
}





















#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 250

typedef struct {
    int x, y;
} Cella;

typedef struct {
    Cella cella;
    long costo;
} Nodo;
/*
 * Confronto due nodi in base al loro costo.Utilizzata dalla funzione qsort per mantenere la coda di priorità ordinata.
 * Restituisce -1 se a < b, 1 se a > b, e 0 se sono uguali.
 */
int confronta(const void *a, const void *b) {
    Nodo *nodoA = (Nodo *)a;
    Nodo *nodoB = (Nodo *)b;
    if (nodoA->costo > nodoB->costo) return 1;
    if (nodoA->costo < nodoB->costo) return -1;
    return 0;
}
 /* 
 * Implemento l'algoritmo di Dijkstra per trovare il percorso di costo minimo in una griglia rappresentata da una matrice di altezze. Utilizza una coda di
 * priorità per esplorare i nodi con il minor costo accumulato.
 * 
 * altezze: Matrice delle altezze delle celle.
 * n: Numero di righe della matrice.
 * m Numero di colonne della matrice.
 * costoCella: Costo fisso per attraversare una cella.
 * costoAltezza: Costo basato sul dislivello tra celle adiacenti.
 * precedente: Array per tracciare il percorso ottimale.
 * costo: Array per mantenere il costo minimo per raggiungere ogni cella.
 */
void dijkstra(int **altezze, int n, int m, int costoCella, int costoAltezza, Cella **precedente, long **costo) {
    /*Direzioni per spostarsi tra le celle: su, giù, sinistra, destra*/
    int direzioneX[] = {-1, 1, 0, 0};
    int direzioneY[] = {0, 0, -1, 1};
    Nodo *coda = (Nodo *)malloc(n * m * sizeof(Nodo)); /* Coda di priorità per i nodi da esplorare*/
    int dimensioneCoda = 0, i, j, uX, uY, vX, vY;
    long nuovoCosto;
    Nodo nodoMin, nodoDaAggiungere;
    /*Inizializza la matrice dei costi a infinito, eccetto la cella di partenza*/
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            costo[i][j] = LONG_MAX;

    costo[0][0] = 0;
    nodoDaAggiungere.cella.x = 0;
    nodoDaAggiungere.cella.y = 0;
    nodoDaAggiungere.costo = 0;
    coda[dimensioneCoda++] = nodoDaAggiungere;

    while (dimensioneCoda) {
        nodoMin = coda[0];/*Estrae il nodo con il costo minimo*/
        coda[0] = coda[--dimensioneCoda];
        qsort(coda, dimensioneCoda, sizeof(Nodo), confronta);/*Mantiene ordinata la coda di priorità*/
        /*Coordinate attuali*/
        uX = nodoMin.cella.x;
        uY = nodoMin.cella.y;
        /*Esploro tutti i nodi adiacenti*/
        for (i = 0; i < 4; i++) {
            /*Cacolo le coordinate del nodo adicente*/
            vX = uX + direzioneX[i];
            vY = uY + direzioneY[i];
            /*controllo che le coordinate siano valide, ovvero stiano dentro i limiti della matrice*/
            if (vX >= 0 && vX < n && vY >= 0 && vY < m) {
                nuovoCosto = costo[uX][uY] + costoCella + costoAltezza * (altezze[uX][uY] - altezze[vX][vY]) * (altezze[uX][uY] - altezze[vX][vY]);
                /*Aggiorno il percorso se ne trovo uno meno costoso*/
                if (nuovoCosto < costo[vX][vY]) {
                    costo[vX][vY] = nuovoCosto;
                    precedente[vX][vY].x = uX;
                    precedente[vX][vY].y = uY;
                    nodoDaAggiungere.cella.x = vX;
                    nodoDaAggiungere.cella.y = vY;
                    nodoDaAggiungere.costo = nuovoCosto;
                    coda[dimensioneCoda++] = nodoDaAggiungere;
                    qsort(coda, dimensioneCoda, sizeof(Nodo), confronta);
                }
            }
        }
    }
    /*libero tutta la memoria che ho utilizzato per la coda*/
    free(coda);
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

    /*Costruiamo il percorso al contrario*/
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
    printf("%ld\n", costo[n-1][m-1]);  /*Stampa il costo totale del percorso*/

    /*Libera la memoria allocata per i percorsi*/
    free(percorsoX);
    free(percorsoY);
}

int main(int argc, char *argv[]) {
    FILE *file_in;
    int costoCella, costoAltezza, n, m, i, j;
    int **altezze;
    Cella **precedente;
    long **costo;

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
    /*liberola memoria allocata dinamicamente*/
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