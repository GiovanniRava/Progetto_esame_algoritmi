#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct {
    int n;              /* numero di nodi               */
    int m;              /* numero di archi              */
} Graph;

typedef struct cella  {
    double costo_fisso; /*costo fisso per ogni cella attraversata*/
    double costo_altezza; /*costo variabile in base all'altezza delle due celle*/
    double costo_H;
}cella;

int leggi_input(FILE* file_in){
    /*Lettura dei costi*/
    double costo_fisso;
    double costo_variabile;
    int i, j, k;
    int n; /* numero righe*/
    int m; /*numero colonne*/
    cella**matrice;
    if (fscanf(file_in, "%lf", &costo_fisso) != 1 || fscanf(file_in, "%lf", &costo_variabile) != 1) {
        fprintf(stderr, "Errore nella lettura dei costi\n");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    /*Lettura del numero di righe e colonne*/
    if (fscanf(file_in, "%d", &n) != 1 || fscanf(file_in, "%d", &m) != 1) {
        fprintf(stderr, "Errore nella lettura del numero di righe e colonne\n");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    /*Allocazione dinamica della matrice*/
    matrice = malloc(n * sizeof(cella *));
    if (matrice == NULL) {
        perror("Errore di allocazione della memoria");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    for ( i = 0; i < n; i++) {
        matrice[i] = malloc(m * sizeof(cella));
        if (matrice[i] == NULL) {
            perror("Errore di allocazione della memoria");
            /* Libera la memoria già allocata prima di uscire*/
            for (j = 0; j < i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            fclose(file_in);
            return EXIT_FAILURE;
        }
    }

    /*Lettura della matrice*/
    for ( i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (fscanf(file_in, "%lf", &matrice[i][j].costo_H) != 1) {
                fprintf(stderr, "Errore nella lettura della matrice alla posizione [%d][%d]\n", i, j);
                /*Libera la memoria allocata*/
                for (k = 0; k < n; k++) {
                    free(matrice[k]);
                }
                free(matrice);
                fclose(file_in);
                return EXIT_FAILURE;
            }
        }
    }
    return 1;
}
int main(int argc, char *argv[]){
    
    FILE* file_in=stdin;
    if(argc!=2){
        fprintf(stderr, "Invocare il programma con %s file_grafo\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "-") != 0) {
        file_in = fopen(argv[1], "r");
        if (file_in == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }
    leggi_input(file_in);
    if(file_in!=stdin) fclose(file_in);
    return EXIT_SUCCESS;
}




g = graph_create(n);
    /* Ciclo di lettura degli archi. Per rendere il programma più
       robusto, meglio non fidarsi del valore `m` nell'intestazione
       dell'input. Leggiamo informazioni sugli archi fino a quando ne
       troviamo, e poi controlliamo che il numero di archi letti (i)
       sia uguale a quello dichiarato (m) */
    i = 0;
    while (3 == fscanf(file_in, "%d %d %lf", &src, &dst, &weight)) {
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
static Edge *new_edge(int src, int dst, double weight, Edge *next)
{
    Edge *edge = (Edge*)malloc(sizeof(Edge));
    assert(edge != NULL);

    edge->src = src;
    edge->dst = dst;
    edge->weight = weight;
    edge->next = next;
    return edge;
}
static int graph_adj_insert(Graph *g, int src, int dst, double weight)
{
#if 0
    const Edge *e;

    /* Se si vuole rendere l'operazione più robusta, controlliamo che
       l'arco non esista già. */
    e = graph_adj(g, src);
    while ((e != NULL) && (e->dst != dst)) {
        e = e->next;
    }
    /* Arrivati qui abbiamo che e == NULL se e solo se l'arco
       non è stato trovato. */
    if (e != NULL) {
        return 1;
    }
#endif
    /* Inseriamo l'arco all'inizio della lista di adiacenza.  Se non
       ci fosse il controllo precedente, l'inserimento di un arco
       richiederebbe tempo O(1) */
    g->edges[src] = new_edge(src, dst, weight, g->edges[src]);
    g->in_deg[dst]++;
    g->out_deg[src]++;
    return 0;
}
void graph_add_edge(Graph *g, int src, int dst, double weight)
{
    int status = 0;

    assert(g != NULL);

    assert((src >= 0) && (src < graph_n_nodes(g)));
    assert((dst >= 0) && (dst < graph_n_nodes(g)));

    status = graph_adj_insert(g, src, dst, weight);
    if (graph_type(g) == GRAPH_UNDIRECTED) {
        status |= graph_adj_insert(g, dst, src, weight);
    }
    if (status == 0)
        g->m++;
    else
        fprintf(stderr, "Ho ignorato l'arco duplicato (%d,%d)\n", src, dst);
}