import asyncio
from bleak import BleakScanner


async def scan_ble_devices():
    devices = await BleakScanner.discover()
    for device in devices:
        print(f"Device {device.name}, Address {device.address}, RSSI {device.rssi}")


loop = asyncio.get_event_loop()
loop.run_until_complete(scan_ble_devices())
