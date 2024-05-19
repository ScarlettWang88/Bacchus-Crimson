import time

import serial
import serial.tools.list_ports

from UI.Uart.const import connect_id_list


def find_device_port(device_identifier):
    """
    查找与给定标识符匹配的设备端口。

    参数:
    - device_identifier: 设备的标识符（可以是设备描述的一部分，或者是硬件ID的一部分）

    返回:
    - 如果找到匹配的设备，则返回设备的串口名称（例如'COM3'）。否则，返回None。
    """
    ports = serial.tools.list_ports.comports()
    for port in ports:
        print(port.description)
        # 这里的检查基于设备的描述，也可以基于硬件ID或其他属性
        for connect_id in device_identifier:
            if connect_id in port.description:
                return port.device
    return None


# Base 节点类，本次实验中指代板子nRF 52840 DK
class BaseNode:

    def __init__(self, name, port, baudrate=115200):
        self.name = name
        self.port = port
        self.baudrate = baudrate

        self.connected = False
        # self.build_connection()

    def build_connection(self):
        try:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=1)
            self.connected = True
            print("串口连接成功")
        except serial.SerialException:
            device_port = find_device_port(connect_id_list)

            if device_port:
                print(f"找到设备，端口为：{device_port}")
                self.port = device_port
                self.ser = serial.Serial(device_port, self.baudrate, timeout=1)
                self.connected = True
            else:
                print("未找到设备")
                # raise RuntimeError("无法建立串口连接")
        except:
            print("无法建立串口连接")
            return False

    # 关闭连接
    def close_connection(self):
        if self.ser:
            self.ser.close()
            self.connected = False

    def send_old(self, data):
        if self.ser:
            data_with_terminator = data + '\n'  # 假设使用换行符作为命令终止符
            data_with_terminator = data_with_terminator.encode()
            self.ser.write(data_with_terminator)
        else:
            print("串口未连接，无法发送数据")

    def receive_old(self):
        if self.ser:
            return self.ser.readline().decode()
        else:
            print("串口未连接，无法接收数据")
            return ''

    def send(self, data):
        if not self.connected or not self.ser:
            print("串口未连接，尝试重连...")
            self.build_connection()
            if not self.connected:
                print("重新连接失败，无法发送数据")
                return
        try:
            data_with_terminator = data + '\n'
            data_with_terminator = data_with_terminator.encode()
            self.ser.write(data_with_terminator)
        except serial.SerialException:
            print("发送数据时连接中断，尝试重新连接...")
            self.build_connection()

    def receive(self):
        if not self.connected or not self.ser:
            print("串口未连接，尝试重连...")
            self.build_connection()
            if not self.connected:
                print("重新连接失败，无法接收数据")
                return ''
        try:
            return self.ser.readline().decode()
        except serial.SerialException:
            print("接收数据时连接中断，尝试重新连接...")
            self.build_connection()
            return ''

    def receive_all_in_buffer(self):
        if not self.connected or not self.ser:
            print("串口未连接，尝试重连...")
            self.build_connection()
            if not self.connected:
                print("重新连接失败，无法接收数据")
                return ''

        try:
            lines = []
            while self.ser.in_waiting > 0:
                line = self.ser.readline().decode().strip()
                if line:
                    lines.append(line)
            return lines
        except serial.SerialException:
            print("接收数据时连接中断，尝试重新连接...")
            self.build_connection()
            return ''

    def __str__(self):
        return "Base [name=" + self.name + ", port=" + self.port + ", baudrate=" + str(self.baudrate) + "]"


# 测试代码
if __name__ == "__main__":
    base = BaseNode("nRF52840 DK", "COM4", 115200)
    base.receive()

    # 建立一个while true循环，检测用户输入的命令，然后发送到串口，然后读取串口的返回值。
    while True:
        command = input("请输入要发送到串口的命令：")
        base.send(command)
        # 延时一段时间，等待串口返回值
        time.sleep(0.1)
        print(base.receive_all_in_buffer())

