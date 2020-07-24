#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "Def.h"
#include "Normal.h"
#include "Cliente.h"
#include "Fila.h"

#define TAM_POP 10			//Tamanho da população
#define TAX_MUT 0.25		//Taxa de mutação inicial
#define TEMPO_PRED 10		//Tempo para predação
#define PLOTAR_DESEMPENHO	//Define se vai plotar gráficos
#define gnuplot C:\\\"Program Files\"\\gnuplot\\bin\\gnuplot.exe	//Caminho para o gnuplot

//Tamanho da fila com incerteza
#define tam_fila(F) (tamanho_fila(F) + (rand() % 3)*2 - 1)
//#define tam_fila(F) ((int) (tamanho_fila(F) * (1 + rand()/RAND_MAX)))

#define abrir_arq_escrita(f, arq) if ((f = fopen(arq,"wb")) == NULL) { perror("Imp. criar arquivo"); exit(0); }	//Macro para abrir arquivo para escrita

void elitismo(int the_best, int* pop);
void torneio2(int the_best, int* pop, float* fit);
void predacao(int the_best, int* pop, float* fit);
int filas_vazias(int num_filas, FILA** filas);
double fitness (int, double, double);

int max_caixas, qte_clientes;	//Máx. de caixas e clientes possíveis
double tax_mut;					//Taxa de mutação variável


int main(){
	char ch = 's';
	int i, k, J, the_best, geracao;
	int num_caixas, num_clientes, num_filas;
	double tempo_max, tempo_media, fit_anterior, d_fit, media;
	double* caixas;		//Tempos para finalizar o atendimento no caixa
	FILA** filas;		//Filas de espera

	#ifdef PLOTAR_DESEMPENHO
	FILE *melhor_arq, *media_arq;
	abrir_arq_escrita(melhor_arq, "melhor.txt");
	abrir_arq_escrita(media_arq, "media.txt");
	#endif

	//Lê a quantidade de clientes e máximo de caixas
	printf("Insira a quantidade de clientes por dia (Maximo %d): ", QTE_MAX_CLIENTES);
    scanf("%d", &qte_clientes);
    printf("\nInsira a quantidade maxima de caixas: ");
    scanf("%d", &max_caixas);

    reset_normal(time(NULL));     //Semente números aleatórios
    
    tax_mut = TAX_MUT;
    fit_anterior = DBL_MIN;
    
    //Gera população de 10 lojas
    float fit_loja[TAM_POP];
    int lojas[TAM_POP];
    geracao = 0;
    for (k = 0; k < TAM_POP; k++)  {
		lojas[k] = rand() % (max_caixas + 1);
		if (lojas[k] == 0 ) lojas[k] = 1;
	}
    
    //Evolui loja
    do {
	    geracao++;		//Conta geração
	    
		for (k = 0; k < TAM_POP; k++) {
		    tempo_max = 0.0;       //Tempo máximo de espera
		    tempo_media = 0.0;     //Tempo médio de atendimento
		
		    num_clientes = qte_clientes;		//Conta quantos clientes faltam para serem atendidos
		    num_filas =  num_caixas = lojas[k];
		    
			caixas = (double*) malloc(num_caixas * sizeof(double));          
		    filas = (FILA**) malloc(num_filas * sizeof(FILA*));   
			
	
		    printf("\nLoja %d	Caixas: %d", (k + 1), num_caixas);    //Imprime alguns parâmetros da simulação
		
		    for (i = 0; i < num_caixas; i++)
		        caixas[i] = 0.0;                    //Inicializa os caixas vazios
		
		    for (i = 0; i < num_filas; i++)
		        filas[i] = criar_fila();            //Cria as filas de esperas
		
		    CLIENTE* cliente_entrada = NULL;        //Cliente que está para entrar na fila
		    CLIENTE* cliente_atendimento = NULL;    //Cliente que será atendido no caixa
		
			//A simulação ocorre enquanto houver clientes a serem atendidos
		    while (num_clientes > 0 || !filas_vazias(num_filas, filas)) {
		
		        //Gera novo cliente que espera seu tempo de chegada para entrar na fila
		        if (num_clientes > 0 && cliente_entrada == NULL) {
		            cliente_entrada = criar_cliente();  
		            if (cliente_entrada != NULL){           
		                //printf("\n\nCriou cliente esperando para entrar"); imprimir_cliente(cliente_entrada);  //Imprime novo cliente
		                num_clientes--;
		            }
		        }
		
		        //Verifica o menor tempo a ser transcorrido em uma iteração
		        int M = -1;     //A princípio, o menor tempo é do cliente a chegar
		        double menor_tempo = (cliente_entrada != NULL) ? cliente_entrada->tempo_chegada : DBL_MAX;    //Define o menor tempo como o do cliente se existir
		        
		        //Verifica se algum caixa tem tempo de espera menor
				for (i = 0; i < num_caixas; i++){
		            J = (num_filas == 1) ? 0 : i;    //J é o índice de fila, no caso de fila única será sempre 0
		            					
					//Caixa possui o menor tempo, mas não está livre e sem fila
					if (caixas[i] <= menor_tempo && !(vazia_fila(filas[J]) && caixas[i] == 0.0)){
		                M = i;
		                menor_tempo = caixas[i];
		            }
		        }
		        
		        
				/*
		        //Imprime o menor tempo a ser transcorrido e sua origem
		        if(M < 0)   printf("\n\nMenor tempo: Cliente a entrar : %lf", menor_tempo);
		        else    printf("\n\nMenor tempo: Caixa %d : %lf", M, menor_tempo);
				*/
				
								
				//Soma e decrementa menor_tempo
		        if(menor_tempo > 0){ 
		            //Soma o menor_tempo aos tempos de espera dos clientes
		            for (i = 0; i < num_filas; i++)
		                somar_tempo_fila(menor_tempo, filas[i]);
		
		            //Decrementa o menor_tempo aos tempos do caixa e de chegada do cliente_entrada
		            if (cliente_entrada != NULL) cliente_entrada->tempo_chegada -= menor_tempo;
		            for (i = 0; i < num_caixas; i++)
		               if (caixas[i] != 0) caixas[i] -= menor_tempo;
		        }
		
		        //Caso o menor tempo seja do cliente a entrar, procura a menor fila (com incerteza)
		        if (M == -1) {
		            int K = 0;          //Marca da menor fila
		            int tam_fila_i;
		            
					int tam_menor_fila = tam_fila(filas[0]);
		            
		            for (i = 1; i < num_filas; i++) {                     
		                tam_fila_i = tam_fila(filas[i]);
		                if (tam_fila_i < tam_menor_fila) {
		                    K = i;    
		                    tam_menor_fila = tam_fila_i;
		                }
		                
						//Duas filas vazias, mas uma com o caixa livre e a outra não
						else if (tam_fila_i == tam_menor_fila && caixas[i] == 0.0 && caixas[K] != 0.0) {
		                	K = i;
		                    tam_menor_fila = tam_fila_i;
		                }
		            }
					
					//Cliente entra na menor fila
		            if (enfileirar_fila(filas[K], cliente_entrada) == 1) {
		                //printf("\n\nCliente entrou na fila");  imprimir_cliente(cliente_entrada);                  //Imprime cliente que entrou na fila
		                cliente_entrada = NULL;
		            }
		        }
		        
		        //Caso o menor tempo seja de um dos caixas
		        else {
		            J = (num_filas == 1) ? 0 : M;                        //J marca a fila do cliente
		            cliente_atendimento = desenfileirar_fila(filas[J]);  //Chama o primeiro cliente da fila
					
					//Caso houvesse cliente na fila
		            if (cliente_atendimento != NULL) {
		                //printf("\n\nCliente saiu da fila"); imprimir_cliente(cliente_atendimento);              //Imprime cliente que saiu da fila
		                tempo_media += cliente_atendimento->tempo_espera;   //Acrescenta tempo de espera na fila ao tempo médio
		                
						//Verifica se o tempo de espera do cliente é o maior de todos
						if (cliente_atendimento->tempo_espera > tempo_max) tempo_max = cliente_atendimento->tempo_espera;
		                    
		                caixas[M] = cliente_atendimento->tempo_servico;     //Tempo que o caixa levará para ficar livre novamente
		                apagar_cliente(&cliente_atendimento);
		            }
		        }
		    }
	
			tempo_media /= qte_clientes;
		    printf("\n\t Tempo media: %lf", tempo_media);    //Imprime o tempo média de espera na fila
		    printf("\n\t Tempo maximo: %lf", tempo_max);     //Imprime o tempo máximo de espera na fila
		
		    for (i = 0; i < num_filas; i++) apagar_fila(&filas[i]);      //Apaga as filas de espera
		    
		    fit_loja[k] = fitness(num_caixas, tempo_media, tempo_max);
		    printf("\n\t Fitness = %f\n", fit_loja[k]);
		}//for população
		
		//Encontra The best e calcula média
		the_best = 0; media = fit_loja[0];
		for (i = 1; i < TAM_POP; i++) {
			if (fit_loja[i] > fit_loja[the_best]) the_best = i;
			media += fit_loja[i];
		}
		media /= TAM_POP;
		
		//Imprime The best e média
		printf("\n\n%da geracao:\n", geracao);
		printf("\tThe best : No caixas = %d, Fitness = %f\n", lojas[the_best], fit_loja[the_best]);
		printf("\tFitness medio: %f\n", media);
		printf("\tTaxa de mutacao = %lf\n", tax_mut);
		
		//Salva desempenho the_best
		#ifdef PLOTAR_DESEMPENHO
		fprintf(melhor_arq, "%d \t %f \r\n", geracao, fit_loja[the_best]);
		fprintf(media_arq, "%d \t %f \r\n", geracao, media);
		#endif
		
		//APlica AG
		//elitismo(the_best, lojas);
		torneio2(the_best, lojas, fit_loja);
		
		/* Varia taxa de mutação
			 Diminue taxa se fitness começar a cair
			 Aumenta taxa se fitness estiver estabilizado
		*/
		d_fit = fit_loja[the_best] - fit_anterior;
		if (d_fit < -0.5) tax_mut *= 0.9;
		else if (d_fit < 0.01 && d_fit > -0.01) tax_mut *= 1.05;
		fit_anterior = fit_loja[the_best];
		
		//Genocídio
		if (tax_mut >= 0.5) {
			for (k = 0; k < TAM_POP; k++)  {
				if (k != the_best) lojas[k] = rand() % (max_caixas + 1);
				if (lojas[k] == 0 ) lojas[k] = 1;
			}
			tax_mut = TAX_MUT;
		}
		
		//Predação a cada TEMPO_PRED gerações
		if(( geracao % TEMPO_PRED ) == 0) predacao(the_best, lojas, fit_loja);
		
		//Interromper evolução
		if(geracao % 10 == 0) {
		    
		    //Plotar desempenho the_best
			#ifdef PLOTAR_DESEMPENHO
			fflush(melhor_arq); fflush(media_arq);
			system("gnuplot -p -e \"plot \'melhor.txt\' with lines, \'media.txt\' with lines\"\n");
			#endif
		    
			printf("\n\nContinuar evolucao (s/n)?");
	    	fflush(stdin);
			ch = getchar();
		}
		
	} while (ch != 'n' && ch != 'N');
    
	#ifdef PLOTAR_DESEMPENHO
    fclose(melhor_arq); fclose(media_arq);
    #endif
	
	return 0;
}

//Verifica se as filas estão vazias
int filas_vazias(int num_filas, FILA** filas){
    int i;
    for (i = 0; i < num_filas; i++)                //Varre as filas
        if (!vazia_fila(filas[i])) return 0;       //Caso não estejam vazias, retorna falso
    return 1;                                      //Caso estejam vazias, retorna verdadeiro
}

//Avalia desempenho
double fitness (int num_caixas, double tma, double tmax) {
	return (-0.5 * tma) + (0.3/num_caixas) - (0.1 * tmax); 
}

/* ------------------------ Funções AG ------------------------ */
void elitismo(int the_best, int* pop) {
	
    for (int i = 0; i <= TAM_POP; i++) {
        if (i == the_best) continue;		// Protege o melhor individuo

        // Crossover
        pop[i] = (pop[i] + pop[the_best]) / 2.0;

        // Mutação
        pop[i] = pop[i] + ((rand() % 10) - 5) * tax_mut;
    }
}

// Torneio de Dois
void torneio2(int the_best, int* pop, float* fit) {
    int a, b, pai1, pai2;
    double pop_aux[TAM_POP];  // Vetor auxiliar para o torneio

    for (int i = 1; i <= TAM_POP; i++) {
        
		//Pula The Best
		if (i == the_best) {
            pop_aux[i] = pop[i];
             continue;
        }

        // Sorteia dois individuos para 1º torneio
        a = (rand() % TAM_POP);
        b = (rand() % TAM_POP);
        
		if (fit[a] > fit[b]) pai1 = a;
        else pai1 = b;

        // Sorteia mais dois individuos para 2º torneio
        a = (rand() % TAM_POP);
        b = (rand() % TAM_POP);
        
		if (fit[a] > fit[b]) pai2 = a;
        else pai2 = b;

        // Crossover
        pop_aux[i] = (pop[pai1] + pop[pai2]) / 2.0;

        // Mutação
        pop_aux[i] = pop_aux[i] + ((rand() % 10) - 5) * tax_mut;
    }
    
    //Copia o População auxiliar para população principal
    for (int i = 1; i <= TAM_POP; i++)
		pop[i] = pop_aux[i] > 1 ? pop_aux[i] : 1;
}

//Predação
void predacao(int the_best, int* pop, float* fit) {
    float min_fit = fit[0];
    int mini = 0;
    
    //Busca pior a ser predado
	for (int i = 1; i <= TAM_POP; i++) {
        if (fit[i] < min_fit) {
            min_fit = fit[i];
            mini = i;
        }
    }
    
    // Mata o Pior e troca por um Randômico
    pop[mini] = rand() % (max_caixas + 1);
    if (pop[mini] == 0 ) pop[mini] = 1;
}
