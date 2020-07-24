# Filas Evolutivas
O projeto consiste em encontrar a melhor disposi��o de caixas para uma loja ou supermecado, mas tamb�m pode ser usado para catracas de metr�, est�dios ou qualquer outro tipo de situa��o na qual diversos pontos de atendimento sejam utilizados massivamente. Para tanto, s�o aplicados algoritmos evolutivos.
[V�deo demonstrativo](https://www.google.com)

### Algoritmo evolutivo
� criada uma popula��o de lojas com as quantidades de caixas definidas randomicamente. Essa  popula��o � submetida a simula��o de filas para obter seu desempenho (tempo m�dio de atendimento e tempo m�ximo). Cada loja � avaliada considerando o desempenho na simula��o e os custos de manuten��o dos caixas. Ent�o, aplica-se torneio de 2 para encontrar a melhor disposi��o atrav�s das gera��es.
Foi aplicada preda��o para aumentar a diversidade a cada N gera��es. A taxa de muta��o � vari�vel, diminui quando a avalia��o come�a a cair ou aumenta quando esta estabiliza. Quando a taxa de muta��o ultrapassa certo valor, ocorre um genoc�dio, sendo que s� a melhor loja � preservada.
As avalia��es da melhor loja e da m�dia das lojas podem ser impressas em um gr�fico com o GNUPLOT.
O tamanho da popula��o, a taxa de muta��o inicial, o tempo para preda��o e a impress�o de gr�ficos podem ser configurados no in�cio do programa principal.

### Simula��o das Filas
Para simula��o, foram usadas uma estrutura CLIENTE para representar o cliente da loja e uma FILA, para representar as filas de espera. Para simular os caixas, usou-se um vetor, cujo valor de cada posi��o indica o tempo restante para liberar o caixa e chamar o pr�ximo da fila.
A simula��o ocorre enquanto houver clientes para serem atendidos. Cada loop da simula��o tem um tempo transcorrido, o qual � o menor entre os tempos de cada caixa a ser liberarado e o do cliente a chegar(ainda n�o chegou � loja e est� para entrar em uma fila quando chegar). De acordo com o menor tempo se toma uma a��o: no caso do cliente a chegar, ele entra na fila que lhe parecer menor (considera incerteza na escolha da menor fila); no caso de um caixa, chama o pr�ximo da fila. O tempo transcorrido em cada loop � somado ao tempo de espera de cada cliente na fila e decrementado dos tempos de caixa e do cliente a chegar.
O tipo CLIENTE possui tr�s valores que representam tempo: chegada(usado como contador regressivo para o cliente entrar na fila), atendimento (valor que � passado ao caixa quando � atendido e representa quanto tempo demoraria at� o caixa estar livre de novo) e espera (tempo que o cliente aguarda na fila).
Ao final da simula��o, informa-se o tempo m�dio e o m�ximo de espera nas filas.
Par�metros como quantidade de clientes e n�meros m�ximo de caixas s�o inseridos pelo usu�rio antes do in�cio da simula��o. Outros par�metros como quantidade m�xima de clientes na fila, tempo de chegada, desvio do tempo de chegada, etc. podem ser alterados no c�digo do arquivo *Def.h*, configurando a situa��o a ser otimizada. Abaixo segue o algoritmo para a simula��o:
- Enquanto houver clientes a serem criados ou nas filas:
    - Se houver clientes a serem atendidos e n�o houver nenhum esperando para entrar na fila, ent�o gera novo cliente;
    - Verifica menor tempo a ser transcorrido(caixas ou cliente a chegar);
    - Soma menor tempo ao tempo de espera dos clientes na fila;
    - Decrementa menor tempo dos caixas e do cliente a chegar;
    - Se o menor tempo for de um dos caixas, ent�o:
        - Desenfileirar cliente;
        - Somar tempo de espera ao tempo m�dio;
        - Verificar se foi o maior tempo de espera;
    - Sen�o, o menor tempo � do cliente a chegar, ent�o:
        - Enfileirar na menor fila.
- Mostrar tempo m�dio e m�ximo de espera.