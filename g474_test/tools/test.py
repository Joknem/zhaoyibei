import sys
from PyQt5.QtWidgets import QApplication, QGraphicsView, QGraphicsScene, QMainWindow, QVBoxLayout, QWidget, QPushButton
from PyQt5.QtCore import QTimer, QRectF
from PyQt5.QtGui import QTransform

class RotatingRectWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # 设置主窗口
        self.setGeometry(100, 100, 800, 600)
        self.setWindowTitle('Rotating Rectangle')

        # 设置场景和视图
        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene, self)
        self.view.setGeometry(50, 50, 700, 500)

        # 创建矩形
        self.rect = self.scene.addRect(QRectF(0, 0, 100, 50))
        self.rect.setTransformOriginPoint(50, 25)  # 设置旋转中心

        # 设置定时器用于旋转矩形
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.rotate_rect)
        self.angle = 0

        # 添加按钮
        self.button = QPushButton('Start/Stop Rotate', self)
        self.button.setGeometry(350, 560, 120, 40)
        self.button.clicked.connect(self.toggle_rotation)

        self.rotating = False

    def rotate_rect(self):
        """旋转矩形"""
        self.angle = (self.angle + 5) % 360  # 每次旋转5度
        transform = QTransform().rotate(self.angle)
        self.rect.setTransform(transform)

    def toggle_rotation(self):
        """控制旋转开始/停止"""
        if self.rotating:
            self.timer.stop()
        else:
            self.timer.start(100)  # 100毫秒更新一次
        self.rotating = not self.rotating

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = RotatingRectWindow()
    window.show()
    sys.exit(app.exec_())

