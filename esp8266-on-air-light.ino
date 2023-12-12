/*********
  Original - Rui Santos
  Project details at http://randomnerdtutorials.com  

  On-Air light version - Bill Brooks
  Project details at https://github.com/wcbdata/onAirLight
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <../sketch/arduino_secrets.h>

// Configure network credentials in arduino_secrets.h
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output2State = "off";
String output3State = "off";

//Initialize variable to hold SVG image
String signString = "Image Not Initialized...";

// Assign output variables to GPIO pins
const int output2 = 2;
const int output3 = 3;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  //Set RX for use as a GPIO
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);
  //Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output2, HIGH);
  digitalWrite(output3, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
           if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("LED off");
              output2State = "on";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("LED on");
              output2State = "off";
              digitalWrite(output2, LOW);
            } else if (header.indexOf("GET /3/single") >= 0) {
              Serial.println("GPIO 3 on");
              digitalWrite(output3, HIGH);
              delay(100); 
              digitalWrite(output3, LOW);
              Serial.println("GPIO 3 off");
            } else if (header.indexOf("GET /3/double") >= 0) {
              Serial.println("GPIO 3 blinking");
              digitalWrite(output3, HIGH);
              delay(100); 
              digitalWrite(output3, LOW);
              delay(100); 
              digitalWrite(output3, HIGH);
              delay(100); 
              digitalWrite(output3, LOW);
              Serial.println("GPIO 3 off");
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266</h1>");
            
            //Image
             //Read image file to show it in the browser
  
            Serial.println("Reading image file");

            if(!LittleFS.begin()){
              Serial.println("An Error has occurred while mounting LittleFS");
              client.println("<p>Image failed to load</p>");
            } else {
              File file = LittleFS.open("OnAirSign.svg", "r");
              uint8_t buf[256];
              int readSize;
              while(file.available()){
                  readSize = file.read(buf, 256);
                  client.write(buf, readSize);
                }
              file.close();
            }


            // Display current state, and ON/OFF buttons for LED (GPIO2)  
            
            // If the output2State is off, it displays the ON button       
            if (output2State=="off") {
              client.println("<p>LED - State " + output2State + " (low/LED ON)</p>");
              client.println("<p><a href=\"/2/on\"><button class=\"button\">HIGH</button></a></p>");
            } else {
              client.println("<p>LED - State " + output2State + " (high/LED OFF)</p>");
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">LOW</button></a></p>");
            }

            // Display current state, and Single-tap button for GPIO 3
            client.println("<p>GPIO 3 - State " + output3State + "</p>");
            // If the output0State is off, it displays the button       
            if (output3State=="off") {
              client.println("<p><a href=\"/3/single\"><button class=\"button\">SINGLE</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">ERR</button></a></p>");
            }

            // Display current state, and Double-tap button for GPIO 3
            client.println("<p>GPIO 3 - State " + output3State + "</p>");
            // If the output0State is off, it displays the button       
            if (output3State=="off") {
              client.println("<p><a href=\"/3/double\"><button class=\"button\">DOUBLE</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">ERR</button></a></p>");
            }

            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
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