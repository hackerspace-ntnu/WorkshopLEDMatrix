import socket
import sys

def send_to_esp32(text):
    host = "ESP_IP_ADDRESS"
    port = 3333

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(str.encode(text))
        print(f"Sent {text} to LED matrix")
        response = s.recv(1024)
        print(f"Received {response.decode('utf-8')} back from LED matrix")

if __name__ == "__main__":
    # example usage: python3 send_text.py "GRATIS KAFFE"
   send_to_esp32(sys.argv[1]) # sys.argv[1] is now GRATIS KAFFE