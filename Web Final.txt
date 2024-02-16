#include <WiFi.h>
#include <Wire.h> //I2C connection

// Replace with your network credentials
const char *ssid = "TIGO-70C3";
const char *password = "2D9557300820";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

int state = 0; 
int state2 = 0;
int state3 = 0; 
int state4 = 0;

int led1=15;
int buzz=4;

void setup() {
   pinMode(led1,OUTPUT);
   pinMode(buzz,OUTPUT);
   Wire.begin(21, 22);
   Serial.begin(115200);

     // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
   WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
 // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /led/on") >= 0)
            {
              state = 1;
              Serial.println("Led ON");
              LED();
                       
            }
            else if (header.indexOf("GET /led/off") >= 0)
            {
              state = 0;
              Serial.println("Led OFF");
              LED_Apagada();
                       }
            if (header.indexOf("GET /buzzer/on") >= 0)
            {
              state3 = 1;
              Serial.println("Buzzer on");
              BUZZER();
                       
            }
            else if (header.indexOf("GET /buzzer/off") >= 0)
            {
              state3 = 0;
              Serial.println("Buzzer off");
              BUZZER_APAGADO();
                       }

            //Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
           

            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #FD0D05; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #6A0A07;}</style></head>");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button3 { background-color: #117987; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button4 {background-color: #0B454D;}</style></head>");
        
            // Web Page Heading
            client.println("<body><h1>Web para encender un BUZZER y una LED</h1>");

            // Display current state, and ON/OFF buttons for GPIO 26
            // If the output26State is off, it displays the ON button
            if (state == 1)
            {
              client.println("<p>Led Encendida</p>");
              client.println("<p><a href=\"/led/off\"><button class=\"button button2\">OFF</button></a></p>");
              
              
            
            }
            else
            {
              client.println("<p>Led Apagada</p>");
              client.println("<p><a href=\"/led/on\"><button class=\"button\">ON</button></a></p>");
            
           
            
            }
            if (state3 == 1)
            {
              client.println("<p>Buzzer Encendido</p>");
              client.println("<p><a href=\"/buzzer/off\"><button class=\"button3 button4\">OFF</button></a></p>");
              
              
            
            }
            else
            {
              client.println("<p>Buzzer Apagado</p>");
              client.println("<p><a href=\"/buzzer/on\"><button class=\"button3\">ON</button></a></p>");
            
           
            
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}

void LED(){
 digitalWrite(led1,HIGH);
}

void LED_Apagada(){
  digitalWrite(led1,LOW);
}
void BUZZER(){
  digitalWrite(buzz,HIGH);
}
void BUZZER_APAGADO(){
  digitalWrite(buzz,LOW);
}
