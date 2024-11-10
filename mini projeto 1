//declaração de variaveis 
int buttonPin = 4; //pino do botão
bool buttonState = true; 
bool lastButtonState = true; //último estado do botão

int delayLED = 500; 
int ledatual = 0; //led atualemnte aceso
bool ledaceso = false; //verifica o estado do led 
byte leds = 0; //armazena o estado dos leds 

long lastDebounceTime = 0; //última vez que o pino de saida foi alterado 
long debouceDelay = 50;
unsigned long tempo_inicial = 0; 
unsigned long pressionar_botao_tempo = 2000; //tempo minimo para pressionar o botão (2 segundos) 
unsigned long ultimo_millis = 0; //ultimo tempo em que o led foi aceso

bool jogoiniciado = false; //jogo começa desligado


void setup() {
  Serial.begin(9600);

  for (int i = 7; i <= 13; i++) {
    pinMode(i, OUTPUT); //definir os leds como outputs
    digitalWrite(i, LOW); //leds desligados inicialmente
  }

  pinMode(buttonPin, INPUT_PULLUP); //definir o botão como um input

}

void loop() {
    
  //se o botão foi pressionado por 2 segundos, então o jogo começa
  if (verificapressaobotao() && !jogoiniciado) {
    jogoiniciado = true; //jogo começa
    piscarled(3); //piscam 3 vezes todos os leds para dar a partida do jogo
  } 
  
  //executa o jogo continuamente, até a função 'jogoiniciado' parar de ser verdadeira
  while (jogoiniciado){
    verificapressaobotao() == false; 
    ojogo(); 
    verificavitoria(); 
    
    //se o botão for pressionado durante 2 segundos, enquanto o jogo está a decorrer, então reinicia 
    if (verificapressaobotao()) {
      reiniciarjogo(); 
      verificapressaobotao() == false; 

      //se o botão voltar a ser pressionado por 2 segundos, então o jogo volta outra vez
      if (verificapressaobotao()){
        ojogo();
    	  verificavitoria(); 
      }
    }
  }
}


//função para verificar se o botão foi pressionado por 2 segundos, se este for o caso, vai piscar 3 vezes
bool verificapressaobotao() {
  bool estado_atual_do_botao = digitalRead(buttonPin); //lê o estado atual do botão 
  bool botaopressionado = false; //variável que lê se o botão está pressionado ou não (inicialmente não está)

  if (estado_atual_do_botao != lastButtonState) { //verifica se o estado do botão foi alterado
    lastDebounceTime = millis(); //se a condição anterior se verifica, então faz se reset ao tempo atual
  }
  //verifica se desde que ocorreu o último reset, já passaram 50 milisegundos 
  if ((millis() - lastDebounceTime) >= debouceDelay) {
    if (estado_atual_do_botao != buttonState) { 
        buttonState = estado_atual_do_botao; //se já passaram os 50 milisegundos e o estado do botão ainda está alterado, então atualiza o estado
        
      //verifica se o botão foi pressionado
      if (buttonState == LOW) {
        tempo_inicial = millis(); //define o tempo inicial quando o botão é pressionado
      }       
      else if (buttonState == HIGH) { 
        tempo_inicial = 0; //se o botão foi solto faz reset do tempo inicial
      }
    }
  }
  lastButtonState = estado_atual_do_botao; 

  //verifica se o botão foi pressionado por 2 segundos 
  if (buttonState == LOW && (millis() - tempo_inicial) >= pressionar_botao_tempo) {
    botaopressionado = true; //logo, o botão foi pressionado por 2 segundos 
    tempo_inicial = millis(); //fazer reset do tempo inicial para evitar novas ativações
  }
  return botaopressionado; 
}


//função para fazer os LEDs piscarem o numero de vezes que desejar
void piscarled(int num) {
  for (int j = 0; j < num; j++) { //pisca o numero de vezes digitado em 'num' 
    for (int i = 7; i <= 13; i++){
      digitalWrite(i, HIGH);
    }
    delay(delayLED); 

    for (int i = 7; i <= 13; i++){
      digitalWrite(i, LOW);
    }
    delay(delayLED); 
  }
}


//função para obter um número aleatório entre 7 e 13 (pinos dos leds)  
int numeroaleatorio() {
  return random(7, 14); 
} 


//função do funcionamento do jogo
void ojogo() {

  if (jogoiniciado == true) {
    int k = numeroaleatorio(); //definir o numero aleatório numa variavel chamada 'k'

    //se o led não estiver aceso, acende-mos um aleatoriamente 
    if (!ledaceso) {
      ledatual = k; //define o novo led atualmente aceso
      digitalWrite(ledatual, HIGH); //liga esse led
      ultimo_millis = millis(); //reinicia o temporizador, pois o led foi agora aceso
      ledaceso = true;
    } else {
      //se tiver um led aceso, apago-o
      if (millis() - ultimo_millis >= delayLED) { //verifica se o tempo desde que o led foi aceso é maior do que o delayLED
        digitalWrite(ledatual, LOW); //apaga o led
        ledaceso = false;
      }
    }

    //verifica se o estado do botão foi alterado, ou seja, se carregar no botão
    if ((buttonState != lastButtonState) && (digitalRead(buttonPin) == HIGH)) {
      if (ledaceso) {
        leds = leds | (1 << (ledatual - 7)); //se o botão foi pressionado e tiver um led aceso, guardo o led aceso na variavel ´leds´
      }
      else { //se o led não estiver aceso
        ledatual = k; //define o novo led atualmente aceso
        digitalWrite(ledatual, HIGH); //liga esse led
        ultimo_millis = millis(); //reinicia o temporizador, pois o led foi agora aceso
        ledaceso = true;
      }
    }

    //percorre todos os pinos e atualiza o estado de cada led 
    for (int i = 7; i <= 13; i++) {
      digitalWrite(i, ((leds >> (i - 7)) & 1) | (ledatual == i)); //verifica se o led deve estar aceso de acordo com a variável leds ou se é o led atualmente aceso 
    }
  }
}

//função para verificar a vitoria/conclusão do jogo
void verificavitoria(){
  if (leds == 0b01111111) { //condição de vitoria: leds todos ligados
    piscarled(3); //os leds piscam 3 vezes para assinalar a vitoria no final do jogo
    jogoiniciado = false; //termina o jogo
    leds = 0; //o armazenamento de leds volta para 0  
  } 
}

//função para reuniciar o jogo quando o botão for pressionado por 2 segundos
void reiniciarjogo() {
  piscarled(2); //piscam os leds 2 vezes para marcar o reinicio do jogo
  jogoiniciado = false; //jogo desliga
  leds = 0; //o armazenamento de leds volta para 0 
}
  
