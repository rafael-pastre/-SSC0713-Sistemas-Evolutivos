#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct cliente {
    double tempo_chegada;       //Tempo que chegará após o último cliente entrar na fila
    double tempo_servico;       //Tempo que levará no caixa
    double tempo_espera;        //Tempo que levará esperando na fila
} CLIENTE;

CLIENTE* criar_cliente();
void apagar_cliente(CLIENTE** cliente);
void imprimir_cliente(CLIENTE* cliente);

#endif // CLIENTE_H
