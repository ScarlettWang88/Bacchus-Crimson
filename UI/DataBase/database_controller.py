import os
import time

from UI.DataBase.Product import Product
from UI.DataBase.User import User


# 从同目录下users.txt加载用户信息
def load_users():
    if not os.path.exists("DataBase/users.txt"):
        print("No user file found")
        return {}
    with open("DataBase/users.txt", "r") as f:
        user_id_dict = {}
        for line in f:
            if line.startswith("#") or line == "\n":
                continue
            uid, name, order = line.split(";")
            uid = uid.split(":")[1].strip()
            name = name.split(":")[1].strip()
            order = order.split(":")[1].strip()
            user = User(uid, name, order)
            user_id_dict[uid] = user
    return user_id_dict


def save_users(user_id_dict):
    # TODO 这里简单粗暴的进行了彻底的覆写，如果user_id_dict很大，可能会有性能问题
    with open("DataBase/users.txt", "w") as f:
        for user in user_id_dict.values():
            f.write(str(user) + "\n")


def load_product(user_id_dict):
    if not os.path.exists("DataBase/products.txt"):
        print("No product file found")
        return {}
    with open("DataBase/products.txt", "r") as f:
        product_dict = {}
        for line in f:
            if line.startswith("#") or line == "\n":
                continue
            product_id, user_id, time_in_stock, time_out_stock, stock_num = line.split(";")
            user_id = user_id.split(":")[1].strip()
            user = user_id_dict[user_id]
            product_id = product_id.split(":")[1].strip()
            # 这里修改为只split一次，然后取第二个元素，避免出现多个冒号的情况
            time_in_stock = time_in_stock.split(":", 1)[1].strip()
            time_out_stock = time_out_stock.split(":", 1)[1].strip()
            stock_num = int(stock_num.split(":")[1].strip())
            product = Product(user, product_id, time_in_stock, time_out_stock, stock_num)
            product_dict[product_id] = product
    return product_dict


def save_product(product_dict):
    # TODO 这里简单粗暴的进行了彻底的覆写，如果product_dict很大，可能会有性能问题
    with open("DataBase/products.txt", "w") as f:
        for product in product_dict.values():
            f.write(str(product) + "\n")


# 返回当前时间 yy/dd/mm hh:mm:ss
def get_current_time():
    return time.strftime("%Y/%m/%d %H:%M:%S", time.localtime())


