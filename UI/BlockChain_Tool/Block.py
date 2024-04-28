import hashlib
import time
import json


class Block:
    def __init__(self, previous_hash, transactions):
        self.timestamp = time.time()
        self.transactions = transactions
        self.previous_hash = previous_hash
        self.nonce = 0
        self.hash = self.calculate_hash()

    def calculate_hash(self):
        block_contents = f"{self.timestamp}{self.transactions}{self.previous_hash}{self.nonce}"
        return hashlib.sha256(block_contents.encode()).hexdigest()

    def mine_block(self, difficulty):
        print("Mining block...")
        pattern = '0' * difficulty
        while not self.hash.startswith(pattern):
            self.nonce += 1
            self.hash = self.calculate_hash()
        print(f"Block mined with nonce {self.nonce} and hash: {self.hash}")

    def add_transaction(self, transaction):
        self.transactions.append(transaction)


