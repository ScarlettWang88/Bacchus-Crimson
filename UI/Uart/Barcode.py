import time

import serial
from PyQt5.QtCore import QThread, pyqtSignal


# 创建Uart交互类
class BarcodeReader(QThread):

    # 信号
    product_signal = pyqtSignal(bytes)
    connect_signal = pyqtSignal(bool)

    def __init__(self, port="COM11", baudrate=9600):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.connected = False
        self.uart = None
        # self.build_connection()

    def build_connection(self):
        try:
            self.uart = serial.Serial(self.port, self.baudrate, timeout=0.5)
            self.connected = True
            print("Barcode扫描器连接成功")
            self.connect_signal.emit(True)
        except:
            print("无法与Barcode扫描器建立串口连接")
            self.connect_signal.emit(False)

    def run(self):
        while not self.isInterruptionRequested():
            if self.connected:
                # print("尝试读取")
                # 读取串口数据
                data = self.uart.read(1024)
                if data:
                    # print("成功读取到数据")
                    # 发射信号，可用于更新UI或其他处理
                    self.product_signal.emit(data)
                time.sleep(0.5)

    def close(self):
        if self.connected:
            # 关闭串口
            self.uart.close()
            self.connected = False

    def __del__(self):
        self.close()