/* declaração de variáveis */ 
int leds[4] = {8, 9, 10, 11}; /* array com os 4 pinos dos leds */
int botoes[3] = {4, 3, 2}; /* array com os 3 pinos dos botões */
int buttonState[3] = {HIGH, HIGH, HIGH}; /* array com o estado dos 3 botões */ 
int lastButtonState[3] = {HIGH, HIGH, HIGH}; /* array com o último estado dos botões */

unsigned long lastDebounceTime[3] = {0, 0, 0}; /* array com o último tempo de debounce de cada botão */ 
int debounceDelay = 100; 
long pressionar_botao_tempo = 2000; /* tempo que o botão tem de estar pressionado para reiniciar o jogo */
bool botaopressionado = false; /* váriavel que regista se um botão foi pressionado */

String leitura; /* string que guarda a leitura do número inserido no serial */

int target = 0; /* variável que guarda o valor do target */
int ponto_de_partida = 0; /* variável que guarda o valor do valor inicial */
int valor_inserido = 0;  /* variável que guarda o valor inserido pelo jogador */
int valor_calculado = 0; /* variável que guarda o valor do valor calculado em cada operação realizada */

long timeLimit = 120000; /* tempo de cada ronda: 2 minutos */ 
unsigned long tempo_led = 0; /* variável que guarda o tempo dos leds, desde que começou uma nova ronda, para acende-los conforme a passagem do tempo */
unsigned long tempo_botao = 0; /* variável que guarda o tempo do botão "OR" para saber quando o tempo de pressionamento é o suficiente para reiniciar o jogo */

bool jogoAtivo = false; /* variável que verifica se o jogo está ativo */
bool botao_and_disponivel = false; /* variável que verifica se o botão "AND" está disponivel para ser pressionado */
bool botao_xor_disponivel = false; /* variável que verifica se o botão "XOR" está disponivel para ser pressionado */


void setup() {

  Serial.begin(9600); 
  
  /* declara os leds como outputs e desligados inicialmente */
  for (int i=0; i<4; i++) {
    pinMode(leds[i], OUTPUT);  
    digitalWrite(leds[i], LOW); 
  } 

  /* declara os botões como inputs */
  for (int y=0; y<3; y++) {
    pinMode(botoes[y], INPUT_PULLUP); 
  } 

  /* inicializa o gerador de números aleatórios baseado no pino A0 */ 
  randomSeed(analogRead(A0)); 
}

  
void loop() {
  
  /* se o jogo não estiver ativo, então inicia uma nova ronda usando a função "nova_ronda()" e a variável jogoAtivo passa a ser verdadeira, isto é, o jogo começou  */
  if (jogoAtivo == false) {
    nova_ronda(); 
    jogoAtivo = true; 
  }

  /* enquanto que o jogoAtivo for verdade, ou seja, o jogo estiver a funcionar, executo a função "ojogo()" */ 
  while (jogoAtivo == true) {

    ojogo();

    /* caso o valor target seja igual ao valor que calculamos com as operações bitwise, mostramos ao jogador que alcançou a vitória e fazemos reset ao jogo */
    if (target == valor_calculado) {
      Serial.println("Vitória!!"); 
      reset_jogo();  
    }
  } 
}


/* função de inicio de ronda */
void nova_ronda() {

  /* estética */
  Serial.println(); 
  Serial.println(); 
  Serial.println("***NOVA RONDA***"); 

  tempo_led = millis(); /* começa a contar o tempo */

  /* define o meu target e o valor inicial */
  target = valor_target(); 
  ponto_de_partida = valor_base(); 

  Serial.print("Valor target: ");
  Serial.println(target, BIN); /* mostra ao jogador o target em binário */
  Serial.print("Valor base: ");
  Serial.println(ponto_de_partida, BIN); /* mostra ao jogador o valor inicial em binário */

  /* defino os leds como desligados no inicio de cada ronda */
  for (int i = 0; i<4; i++) {
    digitalWrite(leds[i], LOW); 
  }
}


/* função que gera o ponto de partida */ 
int valor_base() {

  ponto_de_partida = random(256); /* o máximo de números com 8 bits é igual a 2^8=256, não incluindo o 256 */  

  return ponto_de_partida; /* retorna o random do ponto de partida */
}


/* função que gera o target */
int valor_target() {

  target = random(256); /* o máximo de números com 8 bits é igual a 2^8=256, não incluindo o 256 */ 

  return target; /* retorna o random do target */
}


/* função que lê o número inserido no terminal e transforma num inteiro */
void numero_inserido() { 

  leitura = Serial.readStringUntil('\n');  /* lê o terminal até o enter e guarda na string vazia "leitura" */ 

  valor_inserido = leitura.toInt(); /* transformar o número lido no terminal, armazenado numa string, num inteiro e armazena-lo numa variavel*/ 

  /* caso o valor inserido no terminal tiver mais do que 8 bits, volta a pedir para inserir um valor */ 
  if (valor_inserido > 255) {
    Serial.println("Número inválido, tente novamente.");
    ojogo(); 
  } 
  
  Serial.print("Valor inserido (BIN): ");  
  Serial.println(valor_inserido, BIN); /* mostra ao jogador o valor que inseriu em binário */
}


/* função que implementa a operação bitwise do botão "AND" */ 
void botao_and() {

  valor_calculado = ponto_de_partida & valor_inserido; /* executa a operação "AND" entre dois valores e armazena o resultado numa variavel */ 
  Serial.print("Valor calculado (AND): ");  
  Serial.println(valor_calculado, BIN); /* apresenta o resulta da operação em binário ao jogador */
  ponto_de_partida = valor_calculado; /* atualiza a variavel "ponto_de_partida" para futuros calculos com outros operadores */
}


/* função que implementa a operação bitwise do botão "OR" */ 
void botao_or() {

  valor_calculado = ponto_de_partida | valor_inserido; /* executa a operação "OR" entre dois valores e armazena o resultado numa variavel */ 
  Serial.print("Valor calculado (OR): ");  
  Serial.println(valor_calculado, BIN); /* apresenta o resulta da operação em binário ao jogador */
  ponto_de_partida = valor_calculado; /* atualiza a variavel "ponto_de_partida" para futuros calculos com outros operadores */
}


/* função que implementa a operação bitwise do botão "XOR" */ 
void botao_xor() {
  
  valor_calculado = ponto_de_partida ^ valor_inserido; /* executa a operação "XOR" entre dois valores e armazena o resultado numa variavel */ 
  Serial.print("Valor calculado (XOR): ");  
  Serial.println(valor_calculado, BIN); /* apresenta o resulta da operação em binário ao jogador */
  ponto_de_partida = valor_calculado; /* atualiza a variavel "ponto_de_partida" para futuros calculos com outros operadores */ 
}


/* função que verifica se o bit 1 está ativo */
bool bit1() {

  /* usando o operador "AND" verifica se o bit 1 está ativo, retornando true caso esse seja o caso e false se não estiver ativo */ 
  return (target & 2) != 0; 
} 


/* função que conta o tempo dos leds quando uma ronda começa */ 
bool contagem_tempo() {

  long contagem_led_tempo = millis() - tempo_led; /* variavel que conta o tempo desde o inicio de cada ronda */

  /* desde que o tempo começou a contar, caso seja superior a 1/4 do timeLimit, então acende o led no pino 11 */
  if (contagem_led_tempo >= (timeLimit * 0.25)) {
    digitalWrite(leds[3], HIGH); 
  }
  
  /* desde que o tempo começou a contar, caso seja superior a 1/2 do timeLimit, então acende o led no pino 10 */ 
  if (contagem_led_tempo >= (timeLimit * 0.5)) {
    digitalWrite(leds[2], HIGH); 
  }

  /* desde que o tempo começou a contar, caso seja superior a 3/4 do timeLimit, então acende o led no pino 9 */
  if (contagem_led_tempo >= (timeLimit * 0.75)) { 
    digitalWrite(leds[1], HIGH); 
  }

  /* desde que o tempo começou a contar, caso seja superior ao timeLimit, então acende o led no pino 8, e se o jogo estiver ativo então faço reset ao jogo */ 
  if (contagem_led_tempo >= timeLimit) {
    digitalWrite(leds[0], HIGH); 

    if (jogoAtivo == true) {
      Serial.println("Acabou o tempo!"); 
      reset_jogo(); /* função que faz reset ao jogo */ 
      return true; /* retorna true caso o tempo termine, para sair dos loops while */
    }
  }

  return false; /* retorna false caso o tempo não termine */
}


/* função que lê qual botão foi pressionado para executar a sua funcionalidade */  
void botao_pressionado() {

  bool botaopressionado = false; /* variavel que verifica se um botão foi pressionado */
  bool contar = false; /* variavel que verifica o tempo dos leds, para caso este termine sair do loop */
  bool reiniciar = false; /* variavel que verifica se o jogo vai reinicar, para caso o jogo reinicie saia do loop */

  /* enquanto nenhum botão for pressionado, o tempo dos leds não terminar e o jogo não reiniciar, espera que um dos três aconteça */
  while (botaopressionado == false && contar == false && reiniciar == false) {

    /* variaveis para lerem o estados dos 3 botões */
    int estado_botao_OR = digitalRead(botoes[1]); 
    int estado_botao_AND = digitalRead(botoes[0]); 
    int estado_botao_XOR = digitalRead(botoes[2]); 

    /* debounce dos 3 botões para verificar, caso o estado de um dos botões altere, que foi realmente pressionado */
    if (estado_botao_OR != lastButtonState[1]) {
      lastDebounceTime[1] = millis(); 
    }

    if (estado_botao_AND != lastButtonState[0]) {
      lastDebounceTime[0] = millis(); 
    }

    if (estado_botao_XOR != lastButtonState[2]) {
      lastDebounceTime[2] = millis(); 
    }

    /* caso o botão "OR" seja realmente pressionado, então executo a função relativa a esse botão */ 
    if ((millis() - lastDebounceTime[1]) > debounceDelay && estado_botao_OR == LOW) {
      if (botaopressionado == false) {
        botaopressionado = true; /* atualizo a varivel de "botaopressionado" para true, pois este botão foi pressionado */
        botao_or(); /* executo a função do botão pressionado, neste caso o "OR" */
      } 
    }

    /* caso o botão "AND" seja realmente pressionado e caso esteja disponivel, então executo a função relativa a esse botão */ 
    if ((millis() - lastDebounceTime[0]) > debounceDelay && estado_botao_AND == LOW) {
      if (botao_and_disponivel == true && botaopressionado == false) {
        botaopressionado = true; /* atualizo a varivel de "botaopressionado" para true, pois este botão foi pressionado */
        botao_and(); /* executo a função do botão pressionado, neste caso o "AND" */
      }
      /* caso o botão não esteja disponivel, mas no entanto foi pressionado, informo o jogador que é impossivel realizar a operação e mostro o valor calculado da ultima operação valida e realizada */
      if (botao_and_disponivel == false && botaopressionado == false) {
        botaopressionado = true; /* atualizo a varivel de "botaopressionado" para true, pois este botão foi pressionado mesmo estando indisponivel */
        Serial.println("Operador indisponível"); 
        Serial.print("Valor calculado: ");  
        Serial.println(ponto_de_partida, BIN);
      }
    }

    /* caso o botão "XOR" seja realmente pressionado e caso esteja disponivel, então executo a função relativa a esse botão */ 
    if ((millis() - lastDebounceTime[2]) > debounceDelay && estado_botao_XOR == LOW) {
      if (botao_xor_disponivel == true && botaopressionado == false) {
        botaopressionado = true; /* atualizo a varivel de "botaopressionado" para true, pois este botão foi pressionado */
        botao_xor(); /* executo a função do botão pressionado, neste caso o "XOR" */
      } 
      /* caso o botão não esteja disponivel, mas no entanto foi pressionado, informo o jogador que é impossivel realizar a operação e mostro o valor calculado da ultima operação valida e realizada */
      if (botao_xor_disponivel == false && botaopressionado == false) {
        botaopressionado = true; /* atualizo a varivel de "botaopressionado" para true, pois este botão foi pressionado mesmo estando indisponivel */
        Serial.println("Operador indisponível");
        Serial.print("Valor calculado: ");  
        Serial.println(ponto_de_partida, BIN);
      }
    }
     
    /* atualizo o último estado dos botões */ 
    lastButtonState[1] = estado_botao_OR; 
    lastButtonState[0] = estado_botao_AND;
    lastButtonState[2] = estado_botao_XOR;

    /* caso o tempo dos leds termine, ou seja, termine o tempo da rodada, atualiza o estado da variavel "contar", saindo deste loop, e assim atualizando a variavel "jogoAtivo", do loop principal, para false */
    contar = contagem_tempo(); 
    /* caso o jogador decida reiniciar o jogo, atualiza a variavel reiniciar, saindo do loop e assim atualizando a variavel "jogoAtivo", do loop principal, para false */
    reiniciar = reiniciar_jogo();
  }
}


/* função de reiniciar o jogo */ 
bool reiniciar_jogo() {

  int estado_botao = digitalRead(botoes[1]); /* variavel que lê o estado do botão "OR" */

  /* se o estado do botão "OR" foi alterado, isto é, o botão foi pressionado, começa a contar o tempo de pressionamento */
  if (estado_botao == LOW) {
    if (tempo_botao == 0) {
      tempo_botao = millis();
    }

    /* se o botão ainda estiver a ser pressionado e desde que o tempo começou a contar passaram 2 segundos, então o jogo é reiniciado */
    if ((millis() - tempo_botao) >= pressionar_botao_tempo) {
      Serial.println("A reiniciar jogo...");
      reset_jogo(); /* reset do jogo */
      return true; /* retorna true para sair dos loops */ 
    }
  }
  /* se o botão deixou de ser pressionado antes de passar os 2 segundos, então reinicia a variavel de contagem do tempo do botão */
  else {
    tempo_botao = 0; 
    return false; /* retorna false, pois o jogador não reiniciou o jogo */
  }

  lastButtonState[1] = estado_botao; /* atualiza o último estado do botão "OR" */
}


/* função que roda o jogo */
void ojogo() { 

  Serial.println("Introduza um valor: "); 

  bool contar = false; /* variavel que verifica o tempo dos leds, para caso este termine sair do loop */
  bool reiniciar = false; /* variavel que verifica se o jogo vai reinicar, para caso o jogo reinicie saia do loop */

  /* enquanto o jogador não inserir nada no serial monitor, o tempo continua a contar, e caso termine então o loop quebra e atualiza a variavel "jogoAtivo", do loop principal, para false.
  por outro lado, enquanto o jogador não inserir um valor e quiser reiniciar o jogo, a variavel relativa a esse propósito é atualizada, saindo do loop
  e atualizando a variavel "jogoAtivo", do loop principal, para false */
  while (Serial.available() == 0 && contar == false && reiniciar == false) {
    reiniciar = reiniciar_jogo();
    contar = contagem_tempo();
  }

  /* se o que estiver no terminal for maior do que 0, ou seja, se for alguma coisa, lemos o que foi inserido usando a função "numero_inserido()" */
  if (Serial.available() > 0) { 

    numero_inserido(); 

    /* usando a função que lê o bit 1 decidimos quais botões estão disponiveis */
    if (bit1() == false) { 
      /* bit 1 desativado, botões disponiveis: "OR" e "XOR" */
      Serial.println("Opções permitidas: OR - vermelho  XOR - amarelo");  
      botao_and_disponivel = false; 
      botao_xor_disponivel = true;
      botao_pressionado(); /* depois de defenirmos os botões disponiveis, usamos esta função para ver qual botão foi pressionado e assim executar a operação relativa a esse botão */
    }
    else {
      /* bit 1 ativado, botões disponiveis: "OR" e "AND" */
      Serial.println("Opções permitidas: OR - vermelho  AND - branco");  
      botao_xor_disponivel = false; 
      botao_and_disponivel = true;
      botao_pressionado(); /* depois de defenirmos os botões disponiveis, usamos esta função para ver qual botão foi pressionado e assim executar a operação relativa a esse botão */
    }
  }
}


/* fazer reset às variaveis do jogo, e apagar os leds, quando o jogador reiniciar o jogo, perder, ou ganhar */
void reset_jogo() {

  for (int i = 0; i<4; i++) {
    digitalWrite(leds[i], LOW); 
  }

  valor_calculado = 0; 
  tempo_botao = 0;
  tempo_led = 0;
  jogoAtivo = false; /* atualiza a variavel "jogoAtivo" para false, pois o jogo terminou */
}










