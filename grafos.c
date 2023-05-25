#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define TAM 6

typedef struct {
    unsigned int numVertices;
    int **arestas;
} tGrafo;

void destruirGrafo(tGrafo *grafo) {
    if (grafo->arestas == NULL) {
        free(grafo);
        return;
    }

    for (int i = 0; i < grafo->numVertices; i++)
        if (grafo->arestas[i] != NULL)
            free(grafo->arestas[i]);

    free(grafo->arestas);
    free(grafo);
}

tGrafo *criarGrafo(unsigned int numVertices) { 
    tGrafo *grafo = malloc(numVertices * sizeof(tGrafo));

    if (grafo == NULL)
        return NULL;

    grafo->numVertices = numVertices;

    grafo->arestas = calloc(sizeof(int*), grafo->numVertices);

    if (grafo->arestas == NULL) {
        free(grafo);
        return NULL;
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        grafo->arestas[i] = calloc(sizeof(int), grafo->numVertices);

        if (grafo->arestas[i] == NULL) {
            destruirGrafo(grafo);
            return NULL;
        }
    }

    return grafo;
}

bool possuiAresta(tGrafo *grafo, unsigned int verticeInicial, unsigned int verticeFinal) {
    assert(grafo != NULL);
    assert(verticeInicial < grafo->numVertices);
    assert(verticeFinal < grafo->numVertices);

    return grafo->arestas[verticeInicial][verticeFinal];
}

bool addAresta(tGrafo *grafo, unsigned int verticeInicial, unsigned int verticeFinal, unsigned int peso) {
    assert(grafo != NULL);
    assert(verticeInicial < grafo->numVertices);
    assert(verticeFinal < grafo->numVertices);

    if (possuiAresta(grafo, verticeInicial, verticeFinal))
        return false;

    grafo->arestas[verticeInicial][verticeFinal] = peso;

    return true;
}

bool removerAresta(tGrafo *grafo, unsigned int verticeInicial, unsigned int verticeFinal) {
    assert(grafo != NULL);
    assert(verticeInicial < grafo->numVertices);
    assert(verticeFinal < grafo->numVertices);

    if (!possuiAresta(grafo, verticeInicial, verticeFinal))
        return false;

    grafo->arestas[verticeInicial][verticeFinal] = 0;

    return true;
}

void printMatrixDeAdjacencia(tGrafo *grafo) {
    printf("- Matrix de Adjacência:\n\n");

    printf("  ");
    for (int j = 0; j < grafo->numVertices; j++)
        printf("%3d ", j);
    printf("\n");

    for (int i = 0; i < grafo->numVertices; i++) {
        printf("%d ", i);
        for (int j = 0; j < grafo->numVertices; j++) {
            printf("%3d ", grafo->arestas[i][j]);
        }
        printf("\n");
    }
}

void gerarGrafoDOT(tGrafo *grafo) {
    FILE *arquivoDOT;

    arquivoDOT = fopen("display/grafo.dot","w");

    if (arquivoDOT == NULL)
        exit(1);

    fprintf(arquivoDOT, "digraph {\n");
    fprintf(arquivoDOT, "rankdir=LR;\n");
    fprintf(arquivoDOT, "K=0.6;\n");

    for (int inicial = 0; inicial < grafo->numVertices; inicial++) {
        for (int final = 0; final < grafo->numVertices; final++) {
            if (grafo->arestas[inicial][final]) {
                fprintf(arquivoDOT, "%d -> %d [label = %d];\n", inicial, final, grafo->arestas[inicial][final]);
            }
        }
    }

    fprintf(arquivoDOT, "}\n");

    fclose(arquivoDOT);
}

void gerarGrafoPDF(tGrafo *grafo) {
    printf("Gerando grafo.pdf...\n");
    system("dot display/grafo.dot -Tpdf > display/grafo.pdf");

    printf("Abrindo grafo.pdf...\n");
    system("open display/grafo.pdf");
}

int main() {
    tGrafo *grafo = criarGrafo(TAM);

    addAresta(grafo, 0, 1, 4);
    addAresta(grafo, 0, 2, 2);

    printMatrixDeAdjacencia(grafo);

    gerarGrafoDOT(grafo);

    gerarGrafoPDF(grafo);

    return 0;
}