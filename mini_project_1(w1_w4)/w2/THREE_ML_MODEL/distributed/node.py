class Node:
    def __init__(self, node_id):
        self.node_id = node_id
        self.is_active = True
        self.inbox = []

    def receive_message(self, message):
        if not self.is_active:
            print(f"{self.node_id} is offline!")
            return

        self.inbox.append(message)
        print(f"{self.node_id} received: {message}")

    def process_messages(self):
        if not self.is_active:
            print(f"{self.node_id} cannot process messages!")
            return

        print(f"\n{self.node_id} processing messages:")

        while self.inbox:
            message = self.inbox.pop(0)
            print(f"{self.node_id} processed: {message}")

    def fail(self):
        self.is_active = False
        print(f"\n{self.node_id} has FAILED!")

    def recover(self):
        self.is_active = True
        print(f"\n{self.node_id} has RECOVERED!")
