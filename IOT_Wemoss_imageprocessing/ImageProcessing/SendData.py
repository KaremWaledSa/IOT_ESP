import requests

# Replace with your ESP8266's actual IP address
url = "http://192.168.137.89/process"
data = {"result": "Object Detected"}  # Example data

try:
    response = requests.post(url, json=data)
    print("Response from ESP8266:", response.text)
except requests.exceptions.ConnectionError as e:
    print("Error connecting to ESP8266:", e)
