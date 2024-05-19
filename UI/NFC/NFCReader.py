import time

from PyQt5.QtCore import QThread, pyqtSignal
from py_acr122u import nfc


class NFCReader(QThread):

    # 反馈信号
    user_signal = pyqtSignal(str)
    info_signal = pyqtSignal(str)

    def __init__(self):
        super().__init__()
        self.reader = nfc.Reader()
        self.current_uid = None

    def connect(self):
        self.reader.connect()

    def get_uid(self):
        return self.reader.get_uid()

    # def close(self):
    #     self.reader.close()

    def run(self):
        while not self.isInterruptionRequested():
            try:
                self.connect()
                # self.info_signal.emit("Card detected successfully.")
                uid = self.get_uid()
                if uid:
                    if uid != self.current_uid:
                        self.current_uid = uid
                        self.user_signal.emit(str(uid))
            except Exception as e:
                pass
                # self.info_signal.emit(f"An error occurred: {e}")
            time.sleep(1)


def read_nfc_data():
    # Initialize the NFC reader
    reader = nfc.Reader()

    # Attempt to connect to the reader
    try:
        reader.connect()
        print("Reader connected successfully.")

        # Retrieve and print the UID of the NFC tag
        uid = reader.get_uid()
        print(f"UID of the NFC tag: {uid}")

        # Optionally, you can add more commands here to read other types of data from the tag

    except Exception as e:
        print(f"An error occurred: {e}")

    finally:
        # Ensure to close the connection properly
        # Use the correct method to close or release the reader.
        # Replace 'disconnect' with the appropriate method like 'close' if available.
        if hasattr(reader, 'close'):
            reader.close()
        else:
            print("The reader does not support a direct close method. Please check the documentation.")


if __name__ == "__main__":
    read_nfc_data()
