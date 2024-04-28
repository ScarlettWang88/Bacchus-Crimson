from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMainWindow, QApplication, QGraphicsScene, QGraphicsEllipseItem, QGraphicsView, \
    QGraphicsPathItem
from PyQt5.QtCore import Qt, QPointF, QMutex
from PyQt5.QtGui import QPainter, QPen, QColor, QBrush, QPainterPath
import math
import sys
from Panel.MainPanel import Ui_MainWindow


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.texte_shell.installEventFilter(self)  # 为texte_shell安装事件过滤器
        # 将texte_shell的光标移动到最后
        self.texte_shell.moveCursor(QtGui.QTextCursor.End)

        # self.pushButton.clicked.connect(self.on_click)
        # self.pushButton_2.clicked.connect(self.on_click)
        # self.pushButton_3.clicked.connect(self.on_click)
        # self.pushButton_4.clicked.connect(self.on_click)

    def on_click(self):
        print("Button clicked")

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


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    mainWindow = MainWindow()
    mainWindow.show()
    sys.exit(app.exec_())
