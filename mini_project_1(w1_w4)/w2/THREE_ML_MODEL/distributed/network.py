class Network:
    def __init__(self):
        self.nodes = {}

    def add_node(self, node):
        self.nodes[node.node_id] = node

    def send_message(self, sender_id, receiver_id, message):
        sender = self.nodes.get(sender_id)
        receiver = self.nodes.get(receiver_id)

        # Kiểm tra Node gửi
        if sender is None or not sender.is_active:
            print(f"Cannot send: {sender_id} is unavailable!")
            return False

        # Kiểm tra Node nhận
        if receiver is None or not receiver.is_active:
            print(f"Cannot deliver: {receiver_id} is unavailable!")
            return False

        print(f"\n{sender_id} -> {receiver_id}")
        receiver.receive_message(message)

        return True
