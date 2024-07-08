  //Giovanni Maria Rava 
//Matricola: 000 111 6816
//Classe A
//Mail: giovannimaria.rava@studio.unibo.it

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#define LINE 1024
typedef struct  {
    int costo_fisso; //costo fisso per ogni cella attraversata
    int costo_altezza; //costo variabile in base all'altezza delle due celle
    int costo_H;
}cella;

void leggi_input(FILE* file_in, char nome_file){
    int array_dati_input[4];
    // Lettura dei costi
    int costo_fisso;
    int costo_variabile;
    if (fscanf(file_in, "%lf", &costo_fisso) != 1 || fscanf(file_in, "%lf", &costo_variabile) != 1) {
        fprintf(stderr, "Errore nella lettura dei costi\n");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    // Lettura del numero di righe e colonne
    int n, m;
    if (fscanf(file_in, "%d", &n) != 1 || fscanf(file_in, "%d", &m) != 1) {
        fprintf(stderr, "Errore nella lettura del numero di righe e colonne\n");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    // Allocazione dinamica della matrice
    int **matrice = malloc(n * sizeof(int *));
    if (matrice == NULL) {
        perror("Errore di allocazione della memoria");
        fclose(file_in);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        cella* matrice[i] = malloc(m * sizeof(int));
        if (matrice[i] == NULL) {
            perror("Errore di allocazione della memoria");
            // Libera la memoria già allocata prima di uscire
            for (int j = 0; j < i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            fclose(file_in);
            return EXIT_FAILURE;
        }
    }

    // Lettura della matrice
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (fscanf(file_in, "%d", &matrice[i][j]) != 1) {
                fprintf(stderr, "Errore nella lettura della matrice alla posizione [%d][%d]\n", i, j);
                // Libera la memoria allocata
                for (int k = 0; k < n; k++) {
                    free(matrice[k]);
                }
                free(matrice);
                fclose(file_in);
                return EXIT_FAILURE;
            }
        }
    }

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL) {
        perror("Errore nell'apertura del file");
        return EXIT_FAILURE;
    }
    fclose(file_in);

}
