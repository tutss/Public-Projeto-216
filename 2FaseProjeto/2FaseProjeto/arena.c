#include<stdio.h>
#include<stdlib.h>
#include "arena.h"
#include "maq.h"
#include<time.h>
#include "util.h"

// Variaveis do jogo em si (Condicoes iniciais do jogo)

	static int TotTimes = 2;      // Total de times na partida
	static int TotRobTime = 5;    // Total de robos por partida
	static Celula arena[20][20];  // A arena em sí (o campo de batalha)
	static Maquina robos[2][5];   // Vetor que contem os robos ([times][robos])

// Variáveis da arena
	static int cristaisRestantes;   // Cristais restantes na arena

// Variaveis do sistema

	// Variavies de execucao (indices para as matrizes)
	static int timeAtual = 0;   // Qual time o robo que esta sendo executado pertence
	static int roboAtual = 0;   // Do time, qual robo esta sendo executado

	// Variaveis de controle
	static int pontosTotais [2];    // Pontos totais de cada time,
	static int RobosAtivos[2][5];   // Vetor que guarda se o robo esta ativo ou nao
	static int fimDoJogo = 0;       // Guarda a informacao se o jogo ja acabou

	// Variavel de tempo
	clock_t begin; // "relogio" do começo
  clock_t end; // "relogio" do fim
  double currentTime = 0.0; // tempo total de jogo

// Caso queiramos mudar a contagem de tempo para chamadas de sistema:
// int TempoDeCadaRobo[2][5];

/*
// Funcao apenas para esta faze, a qual atribui a mauina a matriz robos
void assignRobo(Maquina *m){
  robos[0][0] = *m;

}
*/
// Funcao main, a qual inicializa as variaveis e o jogo
// Implementa, mas não para esta fase
/*
int main () {


  // Cria a arena, com os argumentos:
  //	-Tamanho da arena,
  //	-Total de times,
  // 	-Total de cristais,
  // 	-Total de robos por time
  CriaArena(20, TotTimes, 10, TotRobTime);



  // Chama o atualiza
  Atualiza();
}
*/
/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Sistema                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/

/*************************************************
* As op's vão ser implementadas da seguinte maneira,
* MOV = valor 0 (movimenta)
* ATK = valor 1 (ataca)
* INF = valor 2 (informação);
* CLT = valor 3 (coleta)
* DEP = valor 4 (deposita)
*
*************************************************/
// Recebe a operacao (acao) que ira executar, a direcao e de qual maquina esta solicitando
void Sistema(int op, int dir, Maquina *m) {


  /*
  Legenda da direcao:
     0 - Norte
     1 - Nordeste
     2 - Sudeste
     3 - Sul
     4 - Sudoeste
     5 - Noroeste
	 6 - Atual
  */
   //Imprime a arena
   /*
  for(int i = 0; i < 20; i++){
    for(int j = 0; j < 20; j++){
        printf("%d ", arena[j][i].terreno);
    }
    printf("\n");
  }
  */
  // Ajusta o movimento da matriz hexagonal para a matriz quadrada
  int movX = 0; // Celula na direcao X a avancar
  int movY = 0; // Celula na direcao Y a avancar

  if(dir == 1){
    movX = 1;
    movY = -1;
  }
  else if(dir == 2){
    movX = 1;
  }
  else if(dir == 4){
    movX = -1;
  }
  else if(dir == 5){
    movX = -1;
    movY = -1;
  }
  //Para a proxima fase: robos[timeAtual][roboAtual] ao invez de m
  if(m->posx%2){
    movY++;
  }
  if(dir == 0){
    movY = -1;
  }
  else if(dir == 3){
    movY = +1;
  }
  else if (dir == 6){
    movX = 0;
    movY = 0;
  }

  // Futura possivel posicao do robo
  int posTmpX = m->posx + movX;
  int posTmpY = m->posy + movY;
  //Fora do mapa?
  if(posTmpX < 0 || posTmpY < 0 || posTmpX > 19 || posTmpY > 19) {
    empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
  }
  else{
    if(op == 2) { // Se pedir infos
      OPERANDO o;
      o.t = CELL;
      o.Valor.c = arena[posTmpX][posTmpY];
      empilha(&m->pil, o); // Empilha, no robo, a Celula
    }
    else if(op == 0) { // Move
		if(move(posTmpX, posTmpY, m) == 0)
			empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
		else
			empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
	}
    else if(op == 1) { // Ataque
		if(ataque(posTmpX, posTmpY, m))
			empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
		else
			empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false

    }
    else if(op == 3) { // Coletar
		if(coleta(posTmpX, posTmpY, m)) {
			m->crist++;
			empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
		}
      else
        empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
    }
    else if(op == 4) { // Depositar
      // Se o robo tiver cristais
      if(m->crist) {
        // Se tiver uma base a celula que ele quer depositar
        if(arena[posTmpX][posTmpY].base){
          // O time da base que foi depositado um cristal ganha um ponto
          pontosTotais[arena[posTmpX][posTmpY].base -1]++;
          cristaisRestantes--;
          // Caso nao tenha sobrado nenhum cristal mais no jogo
          if(cristaisRestantes == 0)
            Fim();
        }
        // Se nao for base
        else {
           arena[posTmpX][posTmpY].nCristais++;
        }
        // Remove um cristal do robo
        m->crist--;
		    empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
      }
      // Caso ele nao tenha cristais
      else
				empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
    }
  }
  // Caso queiramos mudar a contagem de tempo para chamadas de sistema:
  //TempoDeCadaRobo[timeAtual][roboAtual]++;
  //Atualiza();
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Fim                                          //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Fim () {
  // "Para o relogio"
  end = clock();
  currentTime = (double) (end - begin)/ CLOCKS_PER_SEC;

  // Caso nao foram coletados todos os cristais, eh avisado
  // que o jogo nao foi ate o fim (ou seja, foi interrompido)
  if(cristaisRestantes != 0){
	printf("Esse jogo foi longe demais...\n");
  }

  // Procura o ganhado (quem fez mais pontos)
  int ganhador = 0;
  // calcular o tamanho do array pontosTotais
  int len = sizeof(pontosTotais) / sizeof(pontosTotais[0]);

  for(int i = 1; i < len; i++) {
	if(pontosTotais[i-1] < pontosTotais[i]) {
		ganhador = i;
	}
  }
  // Verifica se houve empate, se houve, avisa que empatou e quem ganhou
  int empate = 0;
  for(int i = 1; i < len; i++) {
	if(pontosTotais[i] == pontosTotais[ganhador] && empate == 0 && ganhador != i) {
		printf("Empate! Times vencedores:\n");
		printf("Time %d;", (i+1));
		printf("Time %d;", ganhador);
	}
	else if(empate == 1  && ganhador != i){
		printf("Time %d;", (i+1));
	}
  }
  // Caso nao houve empate, avisa o ganhador
  if(empate != 0) {
	printf("Time ganhador: Time %d\n Parabéns!\n", ++ganhador);
  }
  // Da parabens a todos e diz quanto tempo levou essa partida
  else {
	printf("Parabéns à todos!\n");
	printf("Tempo total de jogo: %f\n", currentTime);
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Atualiza                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Atualiza (){

  // Caso queiramos mudar a contagem de tempo para chamadas de sistema:
  /*
  // Verifica se o jogo não foi muito longe
  if(RodadaAtual < 500) {


  	// Verifica se o robo não chamou mais do que 5 vezes o sistema nessa rodada
  	if(TempoDeCadaRobo[timeAtual][roboAtual] > RodadaAtual*5){
  		// Se chamou, passa a vez
  		timeAtual++;
  		if(timeAtual == TotTimes -1) {
  			timeAtual = 0;
  			roboAtual++;
  			// Caso tenha acabado a rodada:
  			if(roboAtual == TotRobTime -1) {
  				roboAtual = 0;
  				RodadaAtual++;
  			}
  		}
  	}

    // Verifica se o robo morreu
    if(RobosAtivos[timeAtual][roboAtual]) {
      Atualiza();
    }
    // Caso ainda esteja vivo
    else{
      exec_maquina(robos[timeAtual][roboAtual].m, 50);
    }
  }
  else {
	   Fim();
  }
  */

  int RodadaAtual = 0; // Qual rodada estamos
  // Enquanto estiver abaixo da rodada 500:
  while(RodadaAtual < 500) {
	// A cada robo dentro do time
	for(int i = 0; i < TotRobTime; i++){
		// A cada time
		for(int j = 0; j < TotTimes; j++){
			// Se acabou o jogo, pare com os 3 lacos
			if(fimDoJogo){
				break;
			}
			// Muda o Time atual que está executando
			timeAtual = i;
			// Se o robo esta vivo:
			if(RobosAtivos[timeAtual][roboAtual] == 1) {
				// Executa 50 instrucoes
        // cria ponteiro apontando para a maquina atual
        Maquina* mp = &robos[timeAtual][roboAtual];
				exec_maquina(mp, 50);
			}
      // Muda qual robo dentro do time está executando
      roboAtual = j;
      // "Proxima rodada"
      RodadaAtual++;
    }

	}
	// Caso seja a ultima rodada, acaba com o jogo
	if(RodadaAtual == 499){
		Fim();
	}
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Coleta                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int coleta(int posTmpX, int posTmpY, Maquina *m){
  // Caso tenha cristais na celula desejada
  if(arena[posTmpX][posTmpY].nCristais){
    // Retira um cristal da arena
    arena[posTmpX][posTmpY].nCristais--;
	// Adiciona um cristal ao robo
	m->crist++;
	// Retorna sucesso
    return 1;
  }
  // Caso nao tenha cristais na celula desejada
  else{
	// Retorna fracasso
    return 0;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Ataque                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int ataque(int posTmpX, int posTmpY, Maquina *m){
  // Caso tenha robo para atacar na posicao desejada:
  if(arena[posTmpX][posTmpY].vazia){
    // Procura o robo nessa posicao
    int temRobo = 0;
    int time = 0;
    int qual = 0;
    for(int i = 0; i < TotTimes; i++){
      for(int j = 0; i < TotRobTime; j++){
        if(robos[time][qual].posx == posTmpX && robos[time][qual].posy == posTmpY) {
          time = i;
          qual = j;
		  temRobo = 1;
          break;
        }
      }
    }
    // Como, por enquanto so tem um tipo de ataque, deixemos essa parte comentada
    // if(tipo == 1)
    // Retira 30 pontos de vida do robo atacado
    robos[time][qual].vida -= 30;
    //}
    // Caso acabe a vida desse robo, ele sera destruido
    if(robos[time][qual].vida <= 0){
    RemoveExercito(posTmpX, posTmpY, time, qual);
    }
    // Retorna sucesso
    return 1;
  }
  // Caso esteja vazia:
  else{
    // Retorna fracasso
    return 0;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Move                                         //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int move(int posTmpX, int posTmpY, Maquina *m){

  // Tem alguem ai? - Verifica se ja tem um robo na celula desejada
  if(arena[posTmpX][posTmpY].vazia != 0){

    // Retorna fracasso
    return 0;
  }
  // Nao tem monstro aqui - Nao tem robo na celula desejada
  else {
    // Muda estado da arena
    // Marca a celula que o robo está deixando para vazia
    arena[m->posx][m->posy].vazia = 0;
    // Marca a celula que o robo esta indo para nao vazia, indicando o seu time
    arena[posTmpX][posTmpY].vazia = timeAtual + 1;
    // Muda estado do robo
    // Muda sua posicao
    m->posx = posTmpX;
    m->posy = posTmpY;
    // Caso queiramos mudar a contagem de tempo para chamadas de sistema:
    //TempoDeCadaRobo[timeAtual][roboAtual] += arena[posTmpX][posTmpY].terreno;
    // Retorna sucesso
    return 1;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Cria Robo                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static void InsereExercito (int time, int posX, int posY, int qual) {
  // Cria o robo com a função cria_maquina
  //Implementar para a proxima fase
  //robos[time-1][qual] = *cria_maquina(getProg(), posX, posY, 100, 0, time);
  // Marca como "ativo" esse robo no vetor de robos ativos
  RobosAtivos[time-1][qual] = 1;
  // Caso queiramos mudar a contagem de tempo para chamadas de sistema:
  //TempoDeCadaRobo[time-1][qual] = 0;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                    Destroi Robo                                     //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void RemoveExercito(int posX, int posY, int time, int qual) {

  //Caso não foi informado de qual time o robo pertence
  if(time == -1){
    // Procura o robo a destruir, pela sua posição
    time = 0;
    qual = 0;
    for(int i = 0; i < TotTimes; i++){
      for(int j = 0; i < TotRobTime; j++){
        if(robos[time][qual].posx == posX && robos[time][qual].posy == posY) {
          time = i;
          qual = j;
          break;
        }
      }
    }
  }

  // "Destroi" o robo no vetor
  robos[time][qual].posx = -1;
  robos[time][qual].posy = -1;
  robos[time][qual].vida = 0;

  // Caso ele tenha cristais, ele os derruba (para uma posicao aleatoria, mas perto dele)
  for(int i = 0; i < robos[time][qual].crist; i++){
    int localX = rand() % 3 -1;
    int localY = rand() % 3 -1;
    int cairX = posX + localX;
    int cairY = posY + localY;

    //Verifica se nao caiu dentro da arena, caso nao tenha caido, bota na extremidade da arena
         if (cairX < 0)  cairX = 0;
    else if (cairX > 19) cairX = 19;
         if (cairY < 0)  cairY = 0;
    else if (cairY > 19) cairY = 19;
    arena[posX][posY].nCristais++;
  }
  robos[time][qual].crist = 0;

  // Destroi a Maquina pela funcao destroi_maquina
  destroi_maquina(&robos[time][qual]);
  // Marca como "inativo" esse robo no vetor de robos ativos
  RobosAtivos[time-1][qual] = 0;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                     Cria Arena                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int CriaArena(int tamanho, int times, int cristais, int robosT){

  // Inicialização dos vetores de cristais e dos pontos de cada time
  int cristaisRestantes [cristais];
  int pontosTotais [times];

  fimDoJogo = 0; // O jogo nao terminou
  arena[tamanho][tamanho];



  // alocar memória para arena
  /*for (int i = 0; i < tamanho; i++)
  {
    for (int j = 0; j < tamanho; j++)
      arena[i][j] = malloc ((sizeof(Celula));
  }*/
  //printf("tamanho de Celula: %d", sizeof(Celula));
   timeAtual = 0;
	 roboAtual = 0;
  // Gera a Base do terreno da arena, ou seja, a arena toda fica com o terreno do tipo 1
  for(int i = 0; i < 20; i++){
    for(int j = 0; j < 20; j++){
      arena[i][j].terreno = 0;
      arena[i][j].vazia = 0;
      arena[i][j].nCristais = 0;
      arena[i][j].base = 0;
    }
  }

  time_t t;
  srand((unsigned) time(&t));
  // Gera de modo aleatorio o terreno da arena, com irregularidades
  for(int a = 0; a < 50; a++){
    // Sorteia uma posicao, e gera um retangulo de largura tamanhoX e altura
    // de tamanhoY de um tipo aleatorio de terreno na arena
    int tipoterr = rand() % 5;
    int localX = rand() % 20;
    int localY = rand() % 20;
    int tamanhoX = rand() % 5;
    int tamanhoY = rand() % 5;

    int localXlMin = Maximo(0, localX - tamanhoX);
    int localYlMin = Maximo(0, localY - tamanhoY);
    //arena[0][0].terreno = tipoterr;
    // Caso nao couber na arena, ajustamos o seu tamanho
    for(int i = localXlMin; i < Minimo(20, localX + tamanhoX); i++) {
      for(int j = localYlMin; j < Minimo(20, localY + tamanhoY); j++) {
        arena[i][j].terreno = tipoterr;
      }
    }

  }
  //Bota um cristal no meio da arena, sendo no meio terreno mais dificil (o do tipo 5)
  for(int i = (tamanho/2) - 3; i <= (tamanho/2) + 3; i++){
    for(int j = (tamanho/2) - 3; j <= (tamanho/2) + 3; j++){
      arena[i][j].terreno = 4;
    }
  }
  arena[tamanho/2][tamanho/2].nCristais++;

  // Gera as posições das bases aleatoriamente
  for(int i = 1; i <= times; i++){
    int localX = rand() % 20;
    int localY = rand() % 20;
    // Caso tenha base ou cristais no local sorteado,
    // decrementa o i para refazer esse loop
    if(arena[localX][localY].base || arena[localX][localY].nCristais){
      i--;
    }
    // Caso não tenha nem base nem cristais
    else{
      arena[localX][localY].base = i;
    }
  }

  // Bota os cristais aleatoriamente no mapa
  for(int i = 1; i < cristais; i++){
    int localX = rand() % 20;
    int localY = rand() % 20;
    // Caso ja tenha uma base no local sorteado, decrementa o i para refazer esse loop
    if(arena[localX][localY].base){
      i--;
    }
    // Caso nao tenha base no local
    else{
      arena[localX][localY].nCristais++;
    }
  }
/* Implementa, mas nao para esta fase
  // Bota os robos aleatoriamente no mapa
  for(int j = 1; j <= times; j++){
    for(int i = 0; i < robosT; i++){
      int localX = rand() % 20;
      int localY = rand() % 20;
      // Caso ja tenha um robo no local sorteado, decrementa o i para refazer esse loop
      if(arena[localX][localY].vazia){
        i--;
      }
      // Se estiver vazia:
      else{
        arena[localX][localY].vazia++;
        InsereExercito(times, localX, localY, i);
      }
    }
  }
  */

  // "Inicializa o relogio"
  begin = clock();
}


/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Maximo                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
// Retorna o maior valor entre dois numeros
int Maximo (int a, int b){
  if(a > b) return a;
  else return b;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Minimo                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
// Retorna o menor valor entre dois numeros
int Minimo (int a, int b){
  if(a < b) return a;
  else return b;
}
