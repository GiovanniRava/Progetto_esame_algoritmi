#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* struttura grafo */
typedef struct {
    int n;              /* numero di nodi               */
    int m;              /* numero di archi              */
} Graph;
Graph *graph_read_from_file(FILE *file_in)
{
    int n, m, costo_fisso, costo_variabile;
    int src, dst;
    int i; /* numero archi letti dal file */
    double weight;
    Graph *g;

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