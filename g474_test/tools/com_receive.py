import sys
from numpy import float_
import serial
import threading
from PyQt5.QtGui import QTransform
from PyQt5.QtWidgets import (
    QApplication,
    QGraphicsView,
    QGraphicsScene,
    QMainWindow,
    QVBoxLayout,
    QWidget,
    QPushButton,
    QLabel,
)
from PyQt5.QtCore import *

BAUD_RATE = 115200


class receive_com(QThread):
    my_signal = pyqtSignal(str)

    def __init__(self):
        super().__init__()
        self.ser = serial.Serial("/dev/tty.usbmodem11203", BAUD_RATE)

    def run(self):
        while True:
            self.data = self.ser.readline().decode("utf-8")
            self.my_signal.emit(self.data)


class device_display(QWidget):
    def __init__(self):
        super().__init__()
        self.init_UI()

    def init_UI(self):
        self.setGeometry(100, 100, 1000, 1000)
        self.setWindowTitle("Rotating Rectangle")
        self.receive_com = receive_com()
        self.my_thread = receive_com()
        self.my_thread.my_signal.connect(self.set_label_func)
        self.button1 = QPushButton("Count", self)
        self.button1.clicked.connect(self.count_func)
        self.button2 = QPushButton("Start/Stop Rotate", self)
        self.button2.clicked.connect(self.toggle_rotation)
        self.button3 = QPushButton("Reset", self)
        self.button3.clicked.connect(self.reset_func)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.rotate_rect)
        self.label = QLabel("0", self)
        self.label.setAlignment(Qt.AlignCenter)
        self.v_layout = QVBoxLayout()
        self.v_layout.addStretch(1)
        self.v_layout.addWidget(self.label)
        self.v_layout.addWidget(self.button1)
        self.v_layout.addWidget(self.button2)
        self.v_layout.addWidget(self.button3)
        self.setLayout(self.v_layout)

        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene, self)
        self.view.setGeometry(50, 50, 800, 800)
        # 创建矩形
        self.rect = self.scene.addRect(QRectF(0, 0, 100, 50))
        self.rect.setTransformOriginPoint(50, 25)  # 设置旋转中心

        self.rotating = False

    def set_label_func(self, data):
        self.label.setText(data)

    def count_func(self):
        self.my_thread.start()

    def reset_func(self):
        self.receive_com.ser.write(b'r\n')

    def rotate_rect(self):
        self.angle = (float_(self.my_thread.data)) % 360  
        transform = QTransform().rotate(self.angle)
        self.rect.setTransform(transform)

    def toggle_rotation(self):
        if self.rotating:
            self.timer.stop()
        else:
            self.timer.start(100)  # 100毫秒更新一次
        self.rotating = not self.rotating


if __name__ == "__main__":
    app = QApplication(sys.argv)
    ui = device_display()
    ui.show()
    sys.exit(app.exec_())
