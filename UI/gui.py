import time
import os

from UI.BlueTooth.beacon import BluetoothScanner

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMainWindow, QApplication, QGraphicsScene, QGraphicsEllipseItem, QGraphicsView, \
    QGraphicsPathItem
from PyQt5.QtCore import Qt, QPointF, QMutex
from PyQt5.QtGui import QPainter, QPen, QColor, QBrush, QPainterPath
import math
import sys
from Panel.MainPanel import Ui_MainWindow
from UI.NFC.NFCReader import NFCReader
from UI.DataBase.database_controller import load_users, save_users, load_product, save_product, get_current_time
from UI.DataBase.Product import Product
from UI.DataBase.User import User
from UI.DataBase.const import IN_STOCK, OUT_STOCK, IN_STOCK_CODE, OUT_STOCK_CODE
from UI.Uart.Base_node import BaseNode
from UI.Uart.const import connect_id_list, baud_rate, COM_port, board_name
from UI.Uart.Barcode import BarcodeReader

# 添加锁
mutex = QMutex()


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # 参数定义
        self.current_user = User("Unknown", "Unknown", "Unknown")
        self.temp_user = None
        self.user_id_dict = load_users()

        # 终端
        self.texte_shell.installEventFilter(self)  # 为texte_shell安装事件过滤器
        # 将texte_shell的光标移动到最后
        self.texte_shell.moveCursor(QtGui.QTextCursor.End)

        # nfc 相关
        self.cb_activ_nfc.stateChanged.connect(self.nfc_switch)
        self.cb_activ_nfc.setChecked(True)
        self.btn_user_signin.clicked.connect(self.user_signin)
        self.btn_user_logout.clicked.connect(self.user_logout)

        # base 相关
        self.bases = BaseNode(board_name, COM_port, baud_rate)
        self.cb_activ_base_cont.stateChanged.connect(self.base_switch)
        # self.cb_activ_base_cont.setChecked(True)

        # barcode 相关
        self.barcode = BarcodeReader()
        self.barcode.product_signal.connect(self.register_product)
        self.barcode.connect_signal.connect(self.barcode_start)
        self.cb_activ_ps.stateChanged.connect(self.barcode_switch)
        self.product_dict = load_product()
        # self.update_display_product()
        self.btn_save_product.clicked.connect(self.save_all_product)
        # 出入库设定
        self.in_out_stock_state = IN_STOCK
        self.btn_flip_stock_state.clicked.connect(self.flip_stock_state)
        self.display_stock_state()

        # search
        self.btn_search.clicked.connect(self.search_product)

    def flip_stock_state(self):
        if self.in_out_stock_state == IN_STOCK:
            self.in_out_stock_state = OUT_STOCK
        else:
            self.in_out_stock_state = IN_STOCK
        self.display_stock_state()

    def display_stock_state(self):
        if self.in_out_stock_state == IN_STOCK:
            self.tbx_stock_state.setText("In Stock")
            # 修改tbx_stock_state字体
            self.tbx_stock_state.setStyleSheet("font: 16pt \"MS Shell Dlg 2\";")
        else:
            self.tbx_stock_state.setText("Out Stock")
            # 修改tbx_stock_state字体
            self.tbx_stock_state.setStyleSheet("font: 16pt \"MS Shell Dlg 2\";")

    # 开启nfc扫描
    def nfc_switch(self):
        if self.cb_activ_nfc.isChecked():
            if not hasattr(self, 'nfc_reader'):
                self.nfc_reader = NFCReader()
                self.nfc_reader.user_signal.connect(self.display_user)
                self.nfc_reader.info_signal.connect(self.print_shell)
                self.nfc_reader.start()
                self.print_shell("NFC reader started")
        else:
            if hasattr(self, 'nfc_reader'):
                mutex.lock()
                self.nfc_reader.requestInterruption()
                self.nfc_reader.wait()
                mutex.unlock()
                del self.nfc_reader
                self.print_shell("NFC reader stopped")

    def add_user(self, user_id, user_name):
        new_user_order = len(self.user_id_dict) + 1
        new_user = User(user_id, user_name, new_user_order)
        return new_user

    def user_signin(self):
        if self.temp_user is None:
            self.print_shell("No user detected.")
            return
        if self.temp_user.get_user_id() not in self.user_id_dict.keys():
            user_new_name = self.lined_user_new_name.text()
            if user_new_name == "":
                self.print_shell("Please enter a name.")
                return
            if user_new_name in self.user_id_dict.values():
                self.print_shell("Name already exists.")
                return
            new_user = self.add_user(self.temp_user.get_user_id(), user_new_name)
            self.user_id_dict[new_user.get_user_id()] = new_user
            self.current_user = new_user
            save_users(self.user_id_dict)
            self.temp_user = None
        self.display_user(self.current_user.user_id)

    def display_user(self, user_id):
        new_user = self.user_id_dict.get(user_id, None)
        # 记录uid
        if new_user is not None:
            if self.current_user.get_user_id() != new_user.get_user_id():     # 如果不同，则User换人了
                self.current_user = new_user
                # self.tbx_product.clear()
                # 不再更新文件指针。
                # 更新文件指针（关闭旧文件，打开新文件，如果文件不存在，则创建，如果存在，则追加写入）
                # if self.fp:
                #     self.fp.close()
                # self.fp = open(self.database_address + user_id + ".txt", "a")
            user_name = f'{self.current_user.user_name} [{self.current_user.user_order}]'
        elif new_user is None:
            self.temp_user = User(user_id, "Unknown", "Unknown")
            # 说明此人暂时没有注册，视为临时用户，需要注册后才能添加到数据库中
            user_name = "Unknown [n]"
            self.print_shell(f"Unknown user with ID: {user_id}")
        else:   # new user 没有换人，但是重新调用了该函数，说明是用户注册，需要更新名字。
            user_name = f'{self.current_user.user_name} [{self.current_user.user_order}]'

        self.tbx_user.setText(user_name)
        # 修改tbx_user字体
        self.tbx_user.setStyleSheet("font: 30pt \"MS Shell Dlg 2\";")

    def user_logout(self):
        self.current_user = None
        self.tbx_user.setText("No User")
        self.tbx_user.setStyleSheet("font: 30pt \"MS Shell Dlg 2\";")

    def base_switch(self):
        # 首先检测基站节点的串口连接是否完成
        if not self.bases.connected and self.cb_activ_base_cont.isChecked():
            for i in range(3):
                self.print_shell(f"Trying to connect to the base station with <{i + 1}> time.")
                self.bases.build_connection()
                if self.bases.connected:
                    self.print_shell("Connection to the base station succeeded.")
                    return True
                time.sleep(1)
            self.print_shell("Connection to the base station failed.")
            self.cb_activ_base_cont.setChecked(False)
            return False
        elif not self.cb_activ_base_cont.isChecked() and self.bases.connected:
            self.bases.close_connection()
            self.print_shell("Connection to the base station closed.")

    def barcode_start(self, connect_result):
        if connect_result:
            self.print_shell("Barcode reader connected.")
            self.barcode.start()
        else:
            self.print_shell("Barcode reader connection failed.")
            self.cb_activ_ps.setChecked(False)

    def barcode_switch(self):
        if self.cb_activ_ps.isChecked():
            self.barcode.build_connection()
            self.update_display_product()
        else:
            self.barcode.close()

    def barcode_judge(self, barcode):
        # 检测输入的barcode是商品还是修改码
        if barcode == IN_STOCK_CODE:
            self.in_out_stock_state = IN_STOCK
            self.print_shell("In stock mode.")
            return 0
        elif barcode == OUT_STOCK_CODE:
            self.in_out_stock_state = OUT_STOCK
            self.print_shell("Out stock mode.")
            return 1
        # 如果是商品码
        return 2

    def register_product(self, product_id):
        # 将bytes转换为字符串
        product_id = product_id.decode().strip()
        if self.barcode_judge(product_id) == 2:
            # 首先检索产品是否已经添加
            if product_id in self.product_dict.keys():
                # 如果已经存在，则增加库存
                if self.in_out_stock_state == IN_STOCK:
                    self.product_dict[product_id].set_in_stock(get_current_time())
                    self.print_shell(f"Product {product_id} in stock.")
                    self.update_display_product()

                # 如果此时是出库状态，则降低库存
                if self.in_out_stock_state == OUT_STOCK:
                    if self.product_dict[product_id].get_stock_num() > 0:
                        self.product_dict[product_id].set_out_stock(get_current_time())
                        self.print_shell(f"Product {product_id} out stock.")
                        self.update_display_product()
                    else:
                        self.print_shell(f"[ERROR] Product {product_id} out of stock.")

                if self.bases.connected:
                    status = "0" if self.in_out_stock_state == IN_STOCK else "1"
                    self.bases.send(self.product_dict[product_id].json_display(self.current_user.user_order, status))
                    self.print_shell(f"Product {product_id} sent to base station.")
                    self.print_shell(str(self.bases.receive_all_in_buffer()))

            else:  # 如果不存在，则添加新的产品
                # 如果此时是出库状态，提示状态错误，更改为入库状态
                if self.in_out_stock_state == OUT_STOCK:
                    self.print_shell("[ERROR] Auto switched to in stock mode.")
                    self.in_out_stock_state = IN_STOCK
                # 如果不存在，则添加新的产品
                new_product = Product(self.current_user.get_user_id(), product_id, get_current_time())
                self.product_dict[product_id] = new_product
                self.print_shell(f"New product {product_id} added.")
                self.update_display_product()

                if self.bases.connected:
                    status = "0" if self.in_out_stock_state == IN_STOCK else "1"
                    self.bases.send(new_product.json_display(self.current_user.user_order, status))
                    self.print_shell(f"Product {product_id} sent to base station.")
                    self.print_shell(str(self.bases.receive_all_in_buffer()))

    def update_display_product(self):
        # 清空显示
        self.tbx_product.clear()
        # 遍历整个product_dict，将所有产品信息显示在tbx_product中
        for product in self.product_dict.values():
            self.tbx_product.append(product.ui_display(self.current_user.user_name))

    def save_all_product(self):
        save_product(self.product_dict)

    # shell相关函数
    def eventFilter(self, source, event):
        if event.type() == QtCore.QEvent.KeyPress and source is self.texte_shell:
            if event.key() == QtCore.Qt.Key_Return and self.texte_shell.hasFocus():
                self.get_command()
                return True
        return super().eventFilter(source, event)

    def feedback_shell(self, message):
        self.texte_shell.append("<< " + message)  # 将结果输出到文本框
        self.texte_shell.append(">> ")  # 添加一个空行

    def print_shell(self, message):
        self.texte_shell.append(message)

    # 清空shell
    def clear_shell(self):
        self.texte_shell.clear()

    def get_command(self):
        text = self.texte_shell.toPlainText()
        last_line = text.split('\n')[-1]  # 假设命令是最后一行输入
        command = last_line.strip('>> ').strip()  # 删除提示符和额外的空格
        self.execute_command(command)

    def execute_command(self, command):
        parts = command.split()
        if len(parts) == 0:
            # 如果没有命令，只有回车，那么直接返回
            self.print_shell(">> ")
            return
        head = parts[0]

        if head == 'clid':
            self.feedback_shell(f'User has been cleared')

        elif head == 'clear':
            self.clear_shell()
            self.print_shell(">> ")
        else:
            result = "Unknown command: " + command
            self.feedback_shell(result)

    # search 相关
    def search_product(self):
        input_product_id = self.lined_search.text()
        if input_product_id in self.product_dict.keys():
            self.tbx_search.setText(self.product_dict[input_product_id].ui_display(self.current_user.user_name))
        else:
            self.tbx_search.setText("Product not found.")


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    mainWindow = MainWindow()
    mainWindow.show()
    sys.exit(app.exec_())
