#include <AFMotor.h> // Bilbioteca para acionamento dos motores
#include <Ultrasonic.h>

AF_DCMotor motor_esq(1); //Seleciona o motor 1
AF_DCMotor motor_dir(3); //Seleciona o motor 4

int velocidadeE = 250; // Velocidade do Motor
int velocidadeD = 250; // Velocidade do Motor

int emMovimento = 0; // Variavel de controle de movimento
int direcaoAT = 0; // Variavel de controle de direção
int ultimaDirecao = 0; // Variavel de controle de ultima direção
int colisao = 0; // Variavel de controle de colisão

int speakerPin = 26; // Define o pino que vai ser utilizado para o Buzzer



int trigPin = 24; // Define a porta que será utilizada pelo sensor de proximidade
int echoPin = 25; // Define a porta que será utilizada pelo sensor de proximidade


Ultrasonic ultrasonic(trigPin,echoPin);

void setup() {
  Serial.begin(9600); // Inicia a Serial
  pinMode(speakerPin, OUTPUT); // Determina que o pino do Buzzer é de Saida
  //pinMode(trigPin, OUTPUT); // Determina que o pino de Trigger é de Saida para o Sensor HC-SR04
  //pinMode(echoPin, INPUT); // Determia que o pino de Echo é de Entrada para o Sensor HC-SR04
}

void loop()
{
   
  if (emMovimento == 1) { // Verifica se o protótipo esta em movimento
    if ((colisao == 0) || ((colisao == 1) && (direcaoAT != ultimaDirecao))) {
      if (VerificaColisao() < 15) { // Verifica se a distancia lida pelo HC-SR04 é menor que 15cm
        colisao = 1; // Modifica o valor de colisão para 1, que significa que pode haver colisão
        pararMotores(); // para todos os motores
        sirene(); // emite sinal sonoro
        moverDirecaoInvertida(); // move o protótipo na direção contraria
      } else { // senão
        colisao = 0; // não há colisão e o protótipo continua em movimento
      }
    }
    if ((colisao == 1) && (direcaoAT == ultimaDirecao)) { // Se houver colisão e a direção atual for igual a ultima direção
      sirene(); // emite sinal sonoro
      pararMotores(); // para todos os motores
    }
  }
   
  if (Serial.available() > 0) //verifica se a porta Serial esta disponível
  {

    int entrada = Serial.read();  //faz leitura da Porta Serial

    direcaoAT = entrada; // atribui a direcaoAT (direção atual) com o valor lido
    switch (entrada) { //verificação
      case '8':  //se entrada for igual a 8
        pararMotores(); // para todos os motores
        moverParaFrente(); // move o protótipo para frente
        break;  //termina condição

      case '2':  // se entrada for igual a 2
        pararMotores();  //para todos os motores
        moverParaTras();  //move o protótipo para trás
        break;  //termina condição

      case '6':  //se entrada for igual a 6
        pararMotores();  //para todos os motores
        virarParaDireita();  //vira o protótipo para a direita
        break;  //termina condição

      case '4':  // se entrada formigual a 4
        pararMotores();  //para todos os motores
        virarParaEsquerda();  //vira o ptotótipo para a esquerda
        break;  //termina condição

      case '5':  //se entrada for igual a 5
        pararMotores();  //para todos os motores
        break;    //termina condição

      default:  //se entrada for diferente de 2, 4, 5, 6, 8
        pararMotores();//para todos os motores
        break;  //termina condição
    }
  }
}

void moverParaFrente() {//função de movimento - FRENTE
  //Para frente

  motor_esq.setSpeed(velocidadeE); //Define a velocidade maxima
  motor_dir.setSpeed(velocidadeD); //Define a velocidade maxima
  motor_esq.run(BACKWARD); //Gira o motor sentido anti-horario
  motor_dir.run(BACKWARD); //Gira o motor sentido anti-horario
  emMovimento = 1; // Modifica a variave emMovimento para 1 - que significa que o protótipo esta em movimento
  if (colisao == 0) { // verifica se não há colisão
    ultimaDirecao = 8; // determina que ultimaDirecao é igual a 8 - FRENTE
  }
  Serial.println("Movendo para frente....");// imprime na Serial

}

void moverParaTras() {//função de movimento - TRAS
  //Para tras
  motor_esq.setSpeed(velocidadeE); //Define a velocidade maxima
  motor_dir.setSpeed(velocidadeD); //Define a velocidade maxima
  motor_esq.run(FORWARD); //Gira o motor sentido horario
  motor_dir.run(FORWARD); //Gira o motor sentido horario
  emMovimento = 1;// Modifica a variave emMovimento para 1 - que significa que o protótipo esta em movimento
  if (colisao == 0) { // verifica se não há colisão
    ultimaDirecao = 2;// determina que ultimaDirecao é igual a 2 - TRAS
  }
  Serial.println("Movendo para tras....");
}

void virarParaEsquerda() {//função de movimento - Esquereda
  //Girar para Esquerda
  motor_esq.setSpeed(velocidadeE);//Define a velocidade maxima
  motor_esq.run(BACKWARD);//Gira o motor sentido anti-horario
  emMovimento = 1; // Modifica a variave emMovimento para 1 - que significa que o protótipo esta em movimento
  if (colisao == 0) { // verifica se não há colisão
    ultimaDirecao = 4; //determina que ultimaDirecao é igual a 4 - Esquerda
  }
  Serial.println("Virando para a esquerda....");
}

void virarParaDireita() {//função de movimento - Esquereda
  //Girar para Direita
  motor_dir.setSpeed(velocidadeD);//Define a velocidade maxima
  motor_dir.run(BACKWARD);//Gira o motor sentido anti-horario
  emMovimento = 1;
  emMovimento = 1; // Modifica a variave emMovimento para 1 - que significa que o protótipo esta em movimento
  if (colisao == 0) { // verifica se não há colisão
    ultimaDirecao = 6; //determina que ultimaDirecao é igual a 6 Direita
  }
  Serial.println("Virando para a direita....");
}

void pararMotores() {//função de movimento - PARAR
  //Parar motores
  motor_esq.setSpeed(0); //Define a velocidade maxima
  motor_dir.setSpeed(0);//Define a velocidade maxima
  motor_esq.run(RELEASE); //Desliga o motor
  motor_dir.run(RELEASE); //Desliga o motor
  emMovimento = 0; // Modifica variávem em Movimento para 0 - que significa sem movimento
  Serial.println("Parando os motores....");
}

void sirene() { // Função de Sinal Sonoro
  for (int x = 0; x < 3 ; x ++) { // repete por 3 vezes
    digitalWrite(speakerPin, HIGH); // emite o som
    delay(300); // aguarda 300 milissegundos
    digitalWrite(speakerPin, LOW); // para de emitir o som
    delay(300); // aguarda 300 milissegundos
  }
}

float VerificaColisao() { // função de Verificação de Colisão
  float cmMsec;

  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec,Ultrasonic::CM);
  
  Serial.println(cmMsec);
  return cmMsec;
}

void moverDirecaoInvertida() {
  if (direcaoAT == '8') {
    moverParaTras();
    delay(300);
    pararMotores();
  }
  if (direcaoAT == '4') {
    virarParaDireita();
    delay(300);
    pararMotores();
  }
  if (direcaoAT == '6') {
    virarParaEsquerda();
    delay(300);
    pararMotores();
  }
}