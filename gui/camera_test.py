import cv2
from PySide6.QtCore import QThread, Signal
from PySide6.QtGui import QImage


class CameraThread(QThread):
    frame_ready = Signal(QImage)

    def __init__(self, source):
        super().__init__()
        self.source = source
        self.running = True

    def run(self):
        cap = cv2.VideoCapture(self.source)
        while self.running:
            ok, frame = cap.read()
            if not ok:
                self.msleep(100)
                continue
            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            h, w, ch = rgb.shape
            image = QImage(rgb.data, w, h, ch * w, QImage.Format_RGB888).copy()
            self.frame_ready.emit(image)
        cap.release()

    def stop(self):
        self.running = False
        self.wait()