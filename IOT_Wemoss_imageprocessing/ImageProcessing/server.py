from flask import Flask, jsonify, request

app = Flask(__name__)

@app.route('/upload', methods=['POST'])
def upload_json():
    data = request.json
    with open("server_data.json", "w") as json_file:
        json.dump(data, json_file, indent=4)
    return jsonify({"message": "Data received and saved on server!"})

@app.route('/data', methods=['GET'])
def get_json():
    with open("server_data.json", "r") as json_file:
        data = json.load(json_file)
    return jsonify(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
