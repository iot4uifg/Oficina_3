/**********
  Projeto IOT4U
  IFG Campus Goiânia
  Oficina 3 - Vídeo 1
  Autor:    Matheus Yosimura / Professor Calos Silveira
  Objetivo: Conectar o ESP8266 na rede Wifi. Utilizar funções da biblioteca EDP8266WiFi. Ligar e desligar o LED pelo WebSite e apresentar seu estado (ligado ou desligado).
*********/


// Biblioteca para funções de comunicação WiFi entre o ESP8266 e o Roteador
#include<ESP8266WiFi.h>

//Definição do Nome e Senha da rede que o ESP8266 irá acessar - O ESP8266 não conecta a redes 5G 
const char* ssid = "nome da rede";
const char* password = "senha da rede";  

//Início do server wifi utilizando a porta 80 para comunicação com o navegador
WiFiServer server(80);


void setup() {
  Serial.begin(115200);  
  delay(10); 
  //Declaração do LED imbutido no ESP8266
  pinMode(LED_BUILTIN, OUTPUT);
  //O LED BUILTIN acende em LOW e apaga em HIGH
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  Serial.println();
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid); 
  //Função para conectar o ESP8266 na rede WiFi
  WiFi.begin(ssid, password);
  //Imprime ponto até que o ESP8266 se conecte ao WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  //Inicia o WebServer
  server.begin();
  Serial.println("Server iniciado");
  Serial.print("Use esta URL para se conectar: ");
  //Imrpime o enredeço para conectar no site do WebServer
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 
}

void loop() {
  //Cria o objeto cliente do tipo WiFiClient
  WiFiClient client = server.available();
  //Verifica se o server está disponível para a comunicação
  if (!client) {
    return;
  }
  Serial.println("Novo Cliente");
  //Espera o cliente estar disponível para a comunicação
  while(!client.available()){
    delay(1);
  } 
  //Armazena a requisição do cliente na string request 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush(); 
  //Cria variável value
  int value = LOW;
  //Verifica a indexação da URL para ligar o LED
  if (request.indexOf("/LED=ON") != -1)  {
    //Liga o LED e altera o valor da variável value para LOW
    digitalWrite(LED_BUILTIN, LOW);
    value = LOW;
  }
  //Verifica a indexação da URL para desligar o LED
  if (request.indexOf("/LED=OFF") != -1)  {
    //Desliga o LED e altera o valor da variável value para HIGH
    digitalWrite(LED_BUILTIN, HIGH);
    value = HIGH;
  }
  //Página HTML que será enviada para o cliente
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("");
  client.println("");
  client.print("<!DOCTYPE html>"); 
  client.println("<HTML>");
  client.println("<HEAD>");
  client.println("<TITLE>ESP8266</TITLE>");
  client.println("</HEAD>");
  client.println("<BODY>");
  client.println("<p>O LED esta : ");  
  //Imprime o estado do LED de acordo com o valor armazenado na variável value
  if(value == HIGH) {
    client.print("apagado</p>");
  } 
  else {
    client.print("aceso</p>");
  }
  client.println("<p>Digite na URL /LED=ON para acender o led e /LED=OFF para apagar o led</p>");
  client.println("</BODY>");
  client.println("</HTML>");
  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");   
}
