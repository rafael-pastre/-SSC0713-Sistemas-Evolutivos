# Filas Evolutivas
O projeto consiste em encontrar a melhor disposição de caixas para uma loja ou supermecado, mas também pode ser usado para catracas de metrô, estádios ou qualquer outro tipo de situação na qual diversos pontos de atendimento sejam utilizados massivamente. Para tanto, são aplicados algoritmos evolutivos.
[Vídeo demonstrativo](https://drive.google.com/file/d/1Ajq8PMBiOfxP_hV6EZXo_-gltucSy7rX/view?usp=sharing)

### Algoritmo evolutivo
É criada uma população de lojas com as quantidades de caixas definidas randomicamente. Essa  população é submetida a simulação de filas para obter seu desempenho (tempo médio de atendimento e tempo máximo). Cada loja é avaliada considerando o desempenho em 10 simulações e os custos de manutenção dos caixas. Então, aplica-se torneio de 2 para encontrar a melhor disposição através das gerações.

Foi aplicada predação para aumentar a diversidade a cada N gerações. A taxa de mutação é variável, diminui quando a avaliação começa a cair ou aumenta quando esta estabiliza. Quando a taxa de mutação ultrapassa certo valor, ocorre um genocídio, sendo que só a melhor loja é preservada.
As avaliações da melhor loja e da média das lojas podem ser impressas em um gráfico com o GNUPLOT.
O tamanho da população, a taxa de mutação inicial, o tempo para predação e a impressão de gráficos podem ser configurados no início do programa principal.

### Simulação das Filas
Para simulação, foram usadas uma estrutura CLIENTE para representar o cliente da loja e uma FILA, para representar as filas de espera. Para simular os caixas, usou-se um vetor, cujo valor de cada posição indica o tempo restante para liberar o caixa e chamar o próximo da fila.

A simulação ocorre enquanto houver clientes para serem atendidos. Cada loop da simulação tem um tempo transcorrido, o qual é o menor entre os tempos de cada caixa a ser liberarado e o do cliente a chegar(ainda não chegou à loja e está para entrar em uma fila quando chegar). De acordo com o menor tempo se toma uma ação: no caso do cliente a chegar, ele entra na fila que lhe parecer menor (considera incerteza na escolha da menor fila); no caso de um caixa, chama o próximo da fila. O tempo transcorrido em cada loop é somado ao tempo de espera de cada cliente na fila e decrementado dos tempos de caixa e do cliente a chegar.

O tipo CLIENTE possui três valores que representam tempo: chegada(usado como contador regressivo para o cliente entrar na fila), atendimento (valor que é passado ao caixa quando é atendido e representa quanto tempo demoraria até o caixa estar livre de novo) e espera (tempo que o cliente aguarda na fila).

Ao final da simulação, informa-se o tempo médio e o máximo de espera nas filas.
Parâmetros como quantidade de clientes e números máximo de caixas são inseridos pelo usuário antes do início da simulação. Outros parâmetros como quantidade máxima de clientes na fila, tempo de chegada, desvio do tempo de chegada, etc. podem ser alterados no código do arquivo *Def.h*, configurando a situação a ser otimizada. Abaixo segue o algoritmo para a simulação:
- Enquanto houver clientes a serem criados ou nas filas:
    - Se houver clientes a serem atendidos e não houver nenhum esperando para entrar na fila, então gera novo cliente;
    - Verifica menor tempo a ser transcorrido(caixas ou cliente a chegar);
    - Soma menor tempo ao tempo de espera dos clientes na fila;
    - Decrementa menor tempo dos caixas e do cliente a chegar;
    - Se o menor tempo for de um dos caixas, então:
        - Desenfileirar cliente;
        - Somar tempo de espera ao tempo médio;
        - Verificar se foi o maior tempo de espera;
    - Senão, o menor tempo é do cliente a chegar, então:
        - Enfileirar na menor fila.
- Mostrar tempo médio e máximo de espera.
