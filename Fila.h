#ifndef FILA_H
#define FILA_H


#include "Def.h"
#include "Cliente.h"

typedef struct {
    CLIENTE *vetor[QTE_MAX_CLIENTES];
    int inicio;
    int fim;
} FILA;


FILA *criar_fila();
void apagar_fila(FILA **pfila);

int enfileirar_fila(FILA *fila, CLIENTE *cliente);
CLIENTE *desenfileirar_fila(FILA *fila);

int cheia_fila(FILA *fila);
int vazia_fila(FILA *fila);
int tamanho_fila(FILA *fila);

int somar_tempo_fila(double tempo, FILA *fila);    //Acrescenta 'tempo' ao 'tempo_espera' de cada cliente na fila

#endif    // FILA_H
