//-------Bibliotecas---------//
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

//----------Definições----------//
#define DHTPIN 7 // pino que estamos conectados no arduino
#define DHTTYPE DHT11 // tipo e modelo do sensor: DHT 11

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress ip(192, 168, 21, 51);//---------------IP (um que não esteja sendo usado)  
  EthernetServer server = EthernetServer(8081); //--Porta onde estamos conectados no servidor
  //OBS 1: No campo de pesquisa colocar o IP e a porta juntas, exemplo: 192, 168, 21, 51:8081
  
  DHT dht(DHTPIN, DHTTYPE);//----Muda o formato DHT para dht no código,
 //------------------------------dessa forma o dht pega o pino e o tipo 
 //------------------------------do sensor já especificados no código.
void setup() 
{
  Serial.begin(9600); //---------Inicia o Monitor serial.
  Serial.println("Servidor Ativo!");
  Ethernet.begin(mac, ip); //----Inicia o despositivo Ethernet mandando o mac e o ip para ela.
  server.begin(); //-------------Inicia o servidor.
  dht.begin(); //----------------Inicia o sensor.
}
 
void loop() {
  
  //-------Define a variável -----//  
  float umid = dht.readHumidity(); 
  float temp = dht.readTemperature();

  EthernetClient client = server.available();

  if (client)
  {
      boolean currentLineIsBlank = true; // currentLineIsBlank = A linha atual está em branco
      while (client.connected())//----Enquanto o server estiver conectado.
      {
        if(client.available())//---se o cliente do servidor for aceito para o uso,
                             //---ou seja: se a porta estiver livre.
        {
          char c = client.read(); //---Troca client.read por um char = c.

          if (c=='\n' && currentLineIsBlank) { //----Se a linha atual estiver quebrada e em branco
                                              //----o cliente pode escrever a próxima linha.
                                              
               client.println("HTTP/1.1 200 OK");//------------------Inicia o HTTP verificado.
               client.println("Content-Type: application/json");//---Define o tipo do conteúdo, no caso: JSON
               client.println("Connection: close");
               client.println("Refresh: 2");//-----------------------Atualiza a página e o dados a cada 2 segundos.
               client.println();
               //OBS 2: client.print(ln) é necessário pois estabelece a linha do código e conecta ela ao cliente 
               //-------código-JSON---------// 
                client.println(); 
                client.print("{"); 
                client.print("\"Umidade");
                client.print("\": ");
                client.print(umid);
                client.println(",");
                client.print("\"Temperatura");
                client.print("\": ");
                client.print( temp);
                client.println("}");
                break;
           } 

           if (c == '\n')
           {
            currentLineIsBlank = true;
          }
          else if (c != '\r') 
                {
            currentLineIsBlank = false;
                } 
            } 
        } 
        
        delay(1);      
        client.stop(); 
        
    } 
}
