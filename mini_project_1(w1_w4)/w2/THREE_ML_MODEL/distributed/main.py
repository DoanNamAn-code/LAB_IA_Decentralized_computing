from node import Node
from network import Network
from ml_predictor import MLPredictor


# 1. Initialize Machine Learning models
predictor = MLPredictor()

# 2. Create 3 nodes
node1 = Node("Node_1")
node2 = Node("Node_2")
node3 = Node("Node_3")

# 3. Create network
network = Network()

network.add_node(node1)
network.add_node(node2)
network.add_node(node3)


# 4. Sensor data
temperature = 39
humidity = 82

print("\n========== SENSOR DATA ==========")
print(f"Temperature: {temperature}")
print(f"Humidity: {humidity}")


# 5. Machine Learning prediction
predictions = predictor.predict(temperature, humidity)

print("\n========== ML PREDICTION ==========")

for model_name, status in predictions.items():
    print(f"{model_name}: {status}")


# 6. Create message
message = {
    "temperature": temperature,
    "humidity": humidity,
    "predictions": predictions
}


# 7. Send message to Node 2 and Node 3
print("\n========== NORMAL COMMUNICATION ==========")

network.send_message(
    "Node_1", "Node_2", message
)

network.send_message(
    "Node_1", "Node_3", message
)

node2.process_messages()
node3.process_messages()


# 8. Simulate Node 2 failure
print("\n========== NODE FAILURE ==========")

node2.fail()


# 9. Send another sensor message after failure
print("\n========== COMMUNICATION AFTER FAILURE ==========")

temperature = 42
humidity = 90

predictions = predictor.predict(temperature, humidity)

message = {
    "temperature": temperature,
    "humidity": humidity,
    "predictions": predictions
}

network.send_message(
    "Node_1", "Node_2", message
)

network.send_message(
    "Node_1", "Node_3", message
)

node3.process_messages()

# ========== NODE RECOVERY ==========
print("\n========== NODE RECOVERY ==========")

node2.recover()

print("\nNode_1 -> Node_2")
network.send_message(
    "Node_1",
    "Node_2",
    {
        "temperature": 25,
        "humidity": 50,
        "message": "Node_2 has recovered!"
    }
)

node2.process_messages()

# 10. Final node status
print("\n========== FINAL NODE STATUS ==========")

for node in [node1, node2, node3]:
    status = "ACTIVE" if node.is_active else "FAILED"
    print(f"{node.node_id}: {status}")