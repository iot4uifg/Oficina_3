/**********
  Projeto IOT4U
  IFG Campus Goiânia
  Oficina 3 - Vídeo 2
  Autor:    Matheus Yosimura / Professor Carlos Silveira
  Objetivo: Aprimorar o Exemplo do Vídeo 1 da Oficina 3 inserindo um sensor de luminosidade e introduzindo as linguagens JavaScript e CSS na página web
*********/

// Biblioteca para funções de comunicação WiFi entre o ESP8266 e o Roteador
#include<ESP8266WiFi.h>

//Definição do Nome e Senha da rede que o ESP8266 irá acessar - O ESP8266 não conecta a redes 5G 
const char* ssid = "VIVOFIBRA-D921";
const char* password = "ctaDtZj67X";  

//Início do server wifi utilizando a porta 80 para comunicação com o navegador
WiFiServer server(80);

void setup() {
  //Declaração do pino analógico para o sensor de luminosidade
  pinMode(A0,INPUT);
  Serial.begin(115200);
  delay(10); 
  //Declaração do LED externo no pino GPIO-16 / D0
  pinMode(16, OUTPUT);
  //O LED externo acende em HIGH e apaga em LOW
  digitalWrite(16, HIGH);
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
  int value = HIGH;
  //Verifica a indexação da URL para ligar o LED
  if (request.indexOf("/LED=ON") != -1)  {
    //Liga o LED e altera o valor da variável value para HIGH
    digitalWrite(16, HIGH);
    value = HIGH;
  }
  //Verifica a indexação da URL para desligar o LED
  if (request.indexOf("/LED=OFF") != -1)  {
    //Desiga o LED e altera o valor da variável value para LOW
    digitalWrite(16, LOW);
    value = LOW;
  }
  //Página HTML que será enviada para o cliente
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("");
  client.println("");
  client.print("<!DOCTYPE html>"); 
  client.println("<html>");
  client.println(" <head>");
  client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">");
  //Estilização da página utilizando CSS
  client.println("<style>");
  client.println(".container { text-align: center; }");
  client.println(".bt1{background-color:#00FF00;cursor:pointer;font-size:20px;}");
  client.println(".bt2{background-color:#FF0000;cursor:pointer;font-size:20px;}");
  client.println(".size{font-size:18px;}");
  client.println("</style>");
  //Script para atualizar a página a cada 2,5 segundos utilizando o JavaScript
  client.println("<script type=\"text/javascript\">");
  client.println("function Reload (){");
  client.println("window.location.reload();}");
  client.println(" </script>");
  client.println("<title>ESP8266</title>");
  client.println("</head>");
  client.println("<body onload=\"setInterval('Reload()',2500)\">");
  client.println("<h1 class=\"container\">LED</h1>");
  client.println("<p class=\"container size\">O LED está");
  //Imprime o estado do LED de acordo com o valor armazenado na variável value
  if(value == HIGH){
    client.println("LIGADO</p>");
  }else{
    client.println("DESLIGADO</p>");
  }
  client.println("<p class=\"container size\">Clique nos botões para ligar ou desligar o LED</p>");
  client.println("<h1 class=\"container\">");
  //Insere botões para facilitar o processo de controle do estado do LED
  client.println("<a href=\"/LED=ON\"><button class=\"bt1\">LIGAR</button></a>");
  client.println("<a href=\"/LED=OFF\"><button class=\"bt2\">DESLIGAR</button></a>");
  client.println("</h1>");
  client.println("<br>");
  client.println("<h1 class=\"container\"> SENSOR DE LUMINOSIDADE</h1>");
  client.print("<p class=\"container size\">O sensor está medindo ");
  //Apresenta o valor enviado pelo sensor de luminosidade para o ESP8266
  client.print(analogRead(A0));
  client.println("</p>");
  client.println("</body>");
  client.println("</html>");
  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");   
  }
