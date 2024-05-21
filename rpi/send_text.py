import socket
import sys
from pixelFont import Text, Font

def send_to_esp32():
    if len(sys.argv) < 2:
        print("No text given to the program! (It's required)\nExample usage: python3 send_text.py \"Gratis kaffe\"")
        print("Exiting...")
        return
    
    # set up pixelmaps from image for the text
    text = Text(sys.argv[1], Font.readFromImage('./font3.png')) 
   
    host = "ESP_IP_ADDRESS"
    port = 3333

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall((text.get_pixelmap()))
        print(f"Sent {text} to LED matrix")
        response = s.recv(1024)
        print(f"Received {response.decode('utf-8')} back from LED matrix")

if __name__ == "__main__":
   send_to_esp32() 
    
 