#include <stdlib.h>

#include "Fila.h"

//Cria a estrutura que representa a fila
FILA *criar_fila() {
    FILA *fila = (FILA *) malloc(sizeof(FILA));     //Aloca espaço na memória
    if (fila != NULL) {
        fila->inicio = 0;                           //Inicia a fila
        fila->fim = 0;
    }
    return fila;
}

//Apaga a fila e os clientes que estão nela
void apagar_fila(FILA **pfila) {
    for (int i = (*pfila)->inicio; (i % QTE_MAX_CLIENTES) != (*pfila)->fim; i++) //Varre os clientes em uma fila estática circular
        apagar_cliente(&((*pfila)->vetor[i % QTE_MAX_CLIENTES]));                //Apaga os clientes
    free(*pfila);                                                            //Libera espaço na memória da fila
    *pfila = NULL;                                                           //Coloca NULL no ponteiro da fila
}

//Enfileira cliente na fila
int enfileirar_fila(FILA *fila, CLIENTE *cliente) {
    if (cheia_fila(fila))                           //Só enfileirar se houver lugar na fila
        return 0;
    fila->vetor[fila->fim] = cliente;               //Coloca cliente na fila
    fila->fim = (fila->fim + 1) % QTE_MAX_CLIENTES;     //Define novo fim da fila
    return 1;
}

//Desenfileira cliente da fila
CLIENTE *desenfileirar_fila(FILA *fila) {
    if (vazia_fila(fila))                               //Só desenfileira se houver cliente na fila
        return NULL;
    CLIENTE *cliente = fila->vetor[fila->inicio];       //Copia o endereço do primeiro da fila
    fila->vetor[fila->inicio] = NULL;                   //Remove o primeiro cliente da fila
    fila->inicio = (fila->inicio + 1) % QTE_MAX_CLIENTES;   //Define novo início para a fila
    return cliente;
}

//Verifica se a fila está cheia
int cheia_fila(FILA *fila) {
    return ((fila->fim + 1) % QTE_MAX_CLIENTES == fila->inicio);
}

//Verifica se a fila está vazia
int vazia_fila(FILA *fila) {
    return (fila->inicio == fila->fim);
}

//Calcula o tamanho da fila
int tamanho_fila(FILA *fila) {
    if (fila->inicio <= fila->fim)
        return (fila->fim - fila->inicio);
    return (QTE_MAX_CLIENTES + fila->fim - fila->inicio);
}

//Acrescenta 'tempo' ao 'tempo_espera' de cada cliente na fila
int somar_tempo_fila(double tempo, FILA *fila) {

    if (vazia_fila(fila))       //Verifica se a fila não está vazia para poder somar
        return 0;

    for (int i = fila->inicio; (i % QTE_MAX_CLIENTES) != fila->fim; i++)        //Varre os clientes em uma fila estática circular
        fila->vetor[i % QTE_MAX_CLIENTES]->tempo_espera += tempo;               //Adiciona ao tempo_espera de cada cliente
    return 1;
}
