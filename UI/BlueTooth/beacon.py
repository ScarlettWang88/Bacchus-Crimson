from PyQt5.QtCore import QThread, pyqtSignal
import asyncio
from bleak import BleakScanner


class BluetoothScanner(QThread):
    device_signal = pyqtSignal(str)

    def __init__(self, uuid=b'\x00\x00\x00\x00'):
        super().__init__()
        self.target_uuid = uuid

    def run(self):
        loop = asyncio.new_event_loop()

        asyncio.set_event_loop(loop)
        loop.run_until_complete(self.scan_loop())
        # asyncio.run(self.scan_loop())

    async def scan_loop(self):
        print("开始扫描循环")
        while not self.isInterruptionRequested():
            # 发现附近的所有BLE设备
            devices = await BleakScanner.discover()
            # 根据RSSI值排序，并选取信号最强的三个设备
            top_devices = sorted(devices, key=lambda d: d.rssi, reverse=True)[:3]
            print("找到了设备")
            for device in top_devices:
                device_info = f"Device {device.name}, Address {device.address}, RSSI {device.rssi}"
                print(device_info)
                # 发射信号，可用于更新UI或其他处理
                self.device_signal.emit(device_info)
                # 这里添加解析广播数据的代码
                self.parse_advertisement_data(device)
            # 线程睡眠5秒
            await asyncio.sleep(5)  # 每5秒扫描一次

    def parse_advertisement_data(self, device):
        # 假设广播数据在广告数据的 'manufacturer_data' 中
        manufacturer_data = device.metadata.get('manufacturer_data', {})
        if manufacturer_data:
            for key, value in manufacturer_data.items():
                # 检查UUID是否匹配
                if value[:4] == self.target_uuid:
                    # 解析Barcode data
                    try:
                        barcode_data = value[4:].decode('utf-8')  # 假设barcode data紧随UUID之后开始
                        self.device_signal.emit(f"Barcode Data: {barcode_data}")
                    except UnicodeDecodeError:
                        self.device_signal.emit("Decoding error")
                else:
                    self.device_signal.emit("UUID does not match")
        else:
            self.device_signal.emit("No manufacturer data found.")


class BluetoothBroadcast(QThread):
    """
    由于windows直接实现蓝牙广播比较复杂，这里采用向A板发送Uart数据并由A板实现广播的方式
    """
    def __init__(self, uuid=b'\x00\x00\x00\x00', data=b''):
        super().__init__()
        self.uuid = uuid
        self.data = data

    def run(self):
        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)
        loop.run_until_complete(self.broadcast_loop())

    async def broadcast_loop(self):
        while not self.isInterruptionRequested():
            await asyncio.sleep(5)
            await self.broadcast_data()