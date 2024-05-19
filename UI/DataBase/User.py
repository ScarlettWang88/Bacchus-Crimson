

# 创建用户类
class User:
    def __init__(self, user_id, user_name, user_order):
        self.user_id = user_id
        self.user_name = user_name
        self.user_order = user_order

    def get_user_id(self):
        return self.user_id

    def __str__(self):
        return f"Id: {self.user_id}; Name: {self.user_name}; Order: {self.user_order}"

