import random
from PySide6.QtCore import QThread, Signal

"""todo: edit
FRONT_SENSOR 
RIGHT_SENSOR 
LEFT_SENSOR 
BATTERY_LEVEL """

class SensorThread(QThread):
    sensors_ready = Signal(dict)

    def __init__(self):
        super().__init__()
        self.running = True

    def run(self):
        count = 0
        while self.running:
            data = {
                "front": round(random.uniform(7.2, 8.4), 2),
                "right": round(random.uniform(7.2, 8.4), 2),
                "left": round(random.uniform(7.2, 8.4), 2),
                "battery": round(random.uniform(7.2, 8.4), 2),
            }
            self.sensors_ready.emit(data)
            self.msleep(200)

    def stop(self):
        self.running = False
        self.wait()