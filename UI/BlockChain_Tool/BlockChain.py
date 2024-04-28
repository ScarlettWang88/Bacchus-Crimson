from BlockChain_Tool.Block import Block
import json


class Blockchain:
    def __init__(self):
        self.chain = [self.create_genesis_block()]
        self.difficulty = 4

    def create_genesis_block(self):
        return Block("0", ["Genesis Block"])

    def get_latest_block(self):
        return self.chain[-1]

    def add_block(self, new_block):
        new_block.previous_hash = self.get_latest_block().hash
        new_block.mine_block(self.difficulty)
        self.chain.append(new_block)

    def is_chain_valid(self):
        for i in range(1, len(self.chain)):
            current_block = self.chain[i]
            previous_block = self.chain[i - 1]

            if current_block.hash != current_block.calculate_hash():
                print("Current block hashes not consistent")
                return False

            if current_block.previous_hash != previous_block.hash:
                print("Previous block's hash doesn't match with the current block")
                return False

        return True


# 测试
blockchain = Blockchain()
transaction_data = json.loads('{"timestamp": "2021-01-01T12:00:00Z", "user_id": "123456", "product_barcode": "78910"}')
new_block = Block(blockchain.get_latest_block().hash, [transaction_data])
blockchain.add_block(new_block)

# 检查区块链是否有效
print("Blockchain valid:", blockchain.is_chain_valid())
