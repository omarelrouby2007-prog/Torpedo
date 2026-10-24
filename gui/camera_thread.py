import cv2
import numpy as np
from PySide6.QtCore import QThread, Signal
from PySide6.QtGui import QImage

# ---------- NUMBERS YOU MAY TUNE AT THE COMPETITION ----------
MIN_AREA = 500                                # Step 8: smaller shapes are ignored
COLORS = {                                    # Steps 4-5: (lower H,S,V), (upper H,S,V)
    "red":    [((0, 80, 40), (10, 255, 255)), ((170, 80, 50), (179, 255, 255))],
    "yellow": [((15, 80, 40), (35, 255, 255))],
    "green":  [((40, 80, 40), (80, 255, 255))],
    "blue":   [((95, 80, 40), (130, 255, 255))],
}
KERNEL = np.ones((5, 5), np.uint8)            # Step 6: noise-removal size
# --------------------------------------------------------------

DRAW_COLORS = {                               # Step 10: marker colors in (B, G, R)
    "red": (0, 0, 255), "yellow": (0, 255, 255),
    "green": (0, 255, 0), "blue": (255, 0, 0),
}

def make_mask(hsv, name):                     # Steps 4, 5, 6
    mask = np.zeros(hsv.shape[:2], dtype=np.uint8)           # blank black mask
    for lo, hi in COLORS[name]:                              # red has two ranges
        part = cv2.inRange(hsv, np.array(lo), np.array(hi))  # Step 4: stencil for one range
        mask = cv2.bitwise_or(mask, part)                    # Step 5: merge the ranges
    return cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)    # Step 6: erase specks

def detect_cubes(frame):                      # Step 11: frame in, list of cubes out
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)             # Step 3
    cubes = []
    for name in COLORS:                                      # all four colors
        mask = make_mask(hsv, name)
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)   # Step 7
        for c in contours:
            area = cv2.contourArea(c)                        # Step 8
            if area >= MIN_AREA:
                x, y, w, h = cv2.boundingRect(c)             # Step 9
                cubes.append({"color": name, "x": x, "y": y, "w": w, "h": h,
                              "cx": x + w // 2, "cy": y + h // 2, "area": int(area)})
    return cubes

class CameraThread(QThread):                  # Step 11
    frame_ready = Signal(QImage)              # Step 12: envelope carrying the picture
    cubes_ready = Signal(list)                # Step 12: envelope carrying the cube list
    status_changed = Signal(str)              # Carries a status text to the GUI

    def __init__(self, source):
        super().__init__()
        self.source = source
        self.running = True

    def run(self):
        while self.running:                   # OUTER loop: keep trying to connect
            cap = cv2.VideoCapture(self.source)
            if not cap.isOpened():            # did the connection fail?
                cap.release()                 # clean up the failed attempt
                self.status_changed.emit("Status: camera not found, retrying...")
                self.cubes_ready.emit([])     # clear the list in the GUI
                self.msleep(2000)             # wait 2 seconds before the next try
                continue                      # jump back to the top of the outer loop

            self.status_changed.emit("Status: connected")
            fails = 0                         # counts bad reads in a row
            while self.running:               # INNER loop: read frames while connected
                ok, frame = cap.read()
                if not ok:
                    fails += 1
                    if fails >= 10:           # 10 failures in a row = stream is dead
                        break                 # leave the inner loop only
                    self.msleep(100)
                    continue
                fails = 0                     # a good frame resets the counter
                self.process(frame)
            cap.release()                     # dead stream: hang up, outer loop redials
            self.cubes_ready.emit([])

    def process(self, frame):                 # your old detect + draw + emit code
        cubes = detect_cubes(frame)
        for c in cubes:
            col = DRAW_COLORS[c["color"]]
            cv2.rectangle(frame, (c["x"], c["y"]), (c["x"] + c["w"], c["y"] + c["h"]), col, 2)
            cv2.circle(frame, (c["cx"], c["cy"]), 5, col, -1)
            cv2.putText(frame, c["color"], (c["x"], c["y"] - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, col, 2)
        self.cubes_ready.emit(cubes)
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb.shape
        image = QImage(rgb.data, w, h, ch * w, QImage.Format_RGB888).copy()
        self.frame_ready.emit(image)

    def stop(self):                           # Step 11
        self.running = False
        self.wait()