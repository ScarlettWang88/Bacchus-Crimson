import json


# 创建一个商品类
class Product:
    def __init__(self, user_id, product_id, time_in_stock, time_out_stock=None, stock_num=1):
        self.user_id = user_id
        self.product_id = product_id

        # 默认情况下，商品被注册时，即为入库
        self.time_in_stock = time_in_stock
        self.time_out_stock = time_out_stock
        self.stock_num = stock_num

    def set_in_stock(self, time_in_stock):
        self.time_in_stock = time_in_stock
        self.stock_num = self.stock_num + 1

    def set_out_stock(self, time_out_stock):
        self.time_out_stock = time_out_stock
        self.stock_num = self.stock_num - 1

    def get_stock_num(self):
        return self.stock_num

    def __str__(self):
        return (f"Product: {self.product_id}; User: {self.user_id}; In stock: {self.time_in_stock}; "
                f"Out stock: {self.time_out_stock}; Stock: {self.stock_num}")

    def ui_display(self, user_name):
        return (f"Product: {self.product_id}; Registrant: {user_name};\n—— Newest  In stock time: {self.time_in_stock}; \n"
                f"—— Newest Out stock time: {self.time_out_stock};\n—— Stock: {self.stock_num}")

    def json_display(self, user_id, status):
        display_dict = {
            "userid": user_id,
            "time": self.time_in_stock,
            "barcodeid": self.product_id,
            "status": status
        }
        return json.dumps(display_dict)

