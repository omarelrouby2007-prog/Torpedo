import sys
from PySide6.QtWidgets import QApplication, QWidget
from ui_main_window import Ui_Form
from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from camera_thread import CameraThread
from sensor_thread import SensorThread

SOURCE = 0

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.ui = Ui_Form()
        self.ui.setupUi(self)
        self.ui.verticalLayout.setStretch(0, 5)
        self.ui.verticalLayout.setStretch(7, 1)
        self.ui.statusLabel.setText("Status: connected")
        #threads:
        #camera
        self.camera = CameraThread(SOURCE)
        self.camera.frame_ready.connect(self.show_frame)
        self.camera.start()
        #sensors
        self.sensors = SensorThread()
        #distances
        self.sensors.sensors_ready.connect(self.show_sensors)
        #cubes
        self.sensors.cubes_ready.connect(self.show_cubes)
        self.sensors.start()
    #displays:
    #camera
    def show_frame(self, image):
        pixmap = QPixmap.fromImage(image)
        self.ui.videoLabel.setPixmap(pixmap.scaled(self.ui.videoLabel.size(), Qt.KeepAspectRatio))
    #distances
    def show_sensors(self, data):
        self.ui.frontSensor.setText(f"Front: {data['front']} cm")
        self.ui.rightSensor.setText(f"Right: {data['right']} cm")
        self.ui.leftSensor.setText(f"Left: {data['left']} cm")
        self.ui.batteryLevel.setText(f"Battery: {data['battery']} V")
    #cubes
    def show_cubes(self, cubes):
        self.ui.listWidget.clear()
        for cube in cubes:
            self.ui.listWidget.addItem(f"{cube['color']} (area {cube['area']})")

    def closeEvent(self, event):
        self.camera.stop()
        event.accept()
        
app = QApplication(sys.argv)
window = MainWindow()
window.show()
sys.exit(app.exec())