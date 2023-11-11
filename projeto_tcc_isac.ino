/* Universidade de Brasília
  Autor: Isac Jeferson Ferreira de Sousa
  Programa feito para uso com o sensor de umidade so solo
  proposto por Isac Sousa em seu projeto de TCC em 2019.
  Criado em 04/2019
  Modificado em 06/2019
  IDE versão 1.8.9
*/ 

#include <SPI.h> //biblioteca datalogger
#include <SD.h> //biblioteca datalogger
#include <Wire.h> //biblioteca DHT
#include <SimpleTimer.h> // biblioteca de agendamento de tarefas
#include <Adafruit_ADS1015.h> // biblioteca do conversor AD
//######### variáveis ##########
Adafruit_ADS1115 ads; // cria um objeto do tipo ads que sera utilizado para receberas conversões AD
const int chipSelect = 10; // define o CS como pino 10, com base na pinagem SPI do arduino UNO.
const int T1 = 6;
const int T2 = 5;
const int T3 = 4;
const int T4 = 3;
//Cria um objeto do tipo SimpleTimer, esse objeto tem sua execução programável
SimpleTimer aquecimento;
//Esse void e executado a cada duas horas e define a sequencia dos eventos.
void processo() {
//Primeira parte: informa o início do processo e cria os títulos das colunas de dados.
Serial.println("Inicio do processo");
Serial.print("AD1");
Serial.print('\t');
Serial.print("AD2");
Serial.print('\t');
Serial.print("AD3");
Serial.print('\t');
Serial.println("AD4");
/*
Segunda parte: Cria um laço de repetição de 60 ciclos com 1 segundo de duração,
a cada ciclo todos os dados são gravados
*/
for (int i = 0; i < 60; i++) {
grava(); //chama à execução o void grava.
delay(1000);
}
/*
Terceira parte: Inicia um laço de repetição com 10 ciclos, onde o aquecimento
e ligado (HIGH), posteriormente ao fim desse laço o aquecimento e desligado (LOW).
*/
Serial.println("Inicio do aquecimento");
for (int e = 0; e < 10; e++) {
digitalWrite(T4, HIGH);//define a saída como 5V
digitalWrite(T3, HIGH);
digitalWrite(T2, HIGH);
digitalWrite(T1, HIGH);
grava();
delay(1000);
}
Serial.println("Fim do aquecimento");
digitalWrite(T4, LOW);//define a saída como 0V
digitalWrite(T3, LOW);
digitalWrite(T2, LOW);
digitalWrite(T1, LOW);
/*
Quarta parte: Um laço com 600 ciclos para registrar o resfriamento
*/
for (int u = 0; u < 600; u++) {
grava();
delay(1000);
}
Serial.println("Fim do processo");
}//fim do void processo
void grava() {
//define as variáveis que vão receber os valores digitais
int16_t adc0, adc1, adc2, adc3;
//localiza e distribui os valores obtidos pelo ADS1115
adc0 = ads.readADC_SingleEnded(0);
adc1 = ads.readADC_SingleEnded(1);
adc2 = ads.readADC_SingleEnded(2);
adc3 = ads.readADC_SingleEnded(3);
//Cria uma string onde serão adicionados os valores que serão gravados.
String dataString = "";
dataString += ('\t');
dataString += adc0;
dataString += ('\t');
dataString += adc1;
dataString += ('\t');
dataString += adc2;
dataString += ('\t');
dataString += adc3;
File dataFile = SD.open("datalog.txt", FILE_WRITE);
// Se o arquivo estiver disponível:
if (dataFile) {
dataFile.println(dataString);
dataFile.close();
// imprime as informações no serial:
Serial.println(dataString);
}
// Se o arquivo não abrir, aparece mensagem de erro:
else {
Serial.println("error opening datalog.txt");
}
}// fim do void grava
void setup() {
Serial.begin(57600);
//define os pinos que controlam os transistores como saída
pinMode(T1, OUTPUT);
pinMode(T2, OUTPUT);
pinMode(T3, OUTPUT);
pinMode(T4, OUTPUT);
//Inicia os pinos que controlam a saida como 0V
digitalWrite(T1, LOW);
digitalWrite(T2, LOW);
digitalWrite(T3, LOW);
digitalWrite(T4, LOW);
ads.begin(); // dá início ao conversor AD
//Define qual processo deve ser realizado pelo SimpleTimer assim como o intervalo de tempo
aquecimento.setInterval(7200000, processo);
}// fim do void setup
void loop() {
aquecimento.run(); //aciona a contagem do objeto SimpleTimer
grava();
delay(600000);
} // fim do void loop