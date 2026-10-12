import sys
from PySide6.QtWidgets import QApplication, QWidget
from ui_main_window import Ui_Form
from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from camera_test import CameraThread

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.ui = Ui_Form()
        self.ui.setupUi(self)
        self.ui.videoLabel.setText("i can do it")
        self.camera = CameraThread(0)
        self.camera.frame_ready.connect(self.show_frame)
        self.camera.start()
        
    def show_frame(self, image):
        pixmap = QPixmap.fromImage(image)
        self.ui.videoLabel.setPixmap(
        pixmap.scaled(self.ui.videoLabel.size(), Qt.KeepAspectRatio))

    def closeEvent(self, event):
        self.camera.stop()
        event.accept()
        
app = QApplication(sys.argv)
window = MainWindow()
window.show()
sys.exit(app.exec())