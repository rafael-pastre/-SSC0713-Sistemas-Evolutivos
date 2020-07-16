#include <stdio.h>
#include <stdlib.h>

#include "Def.h"
#include "Normal.h"
#include "Cliente.h"

//Cria estrutura para representar o cliente
CLIENTE* criar_cliente(){
    CLIENTE* cliente = (CLIENTE*) malloc(sizeof (CLIENTE));                 //Aloca memória para estrutura
    if (cliente != NULL){
        cliente->tempo_chegada = Normal(TEMPO_CHEGADA, DESVIO_CHEGADA);     //Define tempo de chegada
        cliente->tempo_servico = Normal(TEMPO_SERVICO, DESVIO_SERVICO);     //Define tempo de serviço
        cliente->tempo_espera = 0.0;                                        //Tempo de espera é nulo no começo
    }
    return cliente;
}

//Apaga a estrutura que representa o cliente
void apagar_cliente(CLIENTE** cliente){
    if (cliente != NULL && *cliente != NULL){       //Verifica se há cliente para ser apagado
        free(*cliente);     //Apaga cliente
        *cliente = NULL;    //Coloca NULL no ponteiro do cliente
    }
}

//Imprime na tela os campos da estrutura cliente
void imprimir_cliente(CLIENTE* cliente){
    printf("\nchegada = %lf\
    servico = %lf\
    espera = %lf",cliente->tempo_chegada, cliente->tempo_servico, cliente->tempo_espera);
}
