import cv2
import numpy as np


cap = cv2.VideoCapture("/dev/video")

try:
    while True:
        ret, img = cap.read()
        cv2.line(img, (0, 280), (640, 280), (255, 0, 0), 2)
        cv2.line(img, (275, 0), (275, 480), (255, 0, 0), 2)
        cv2.imshow("look", img)
        cv2.waitKey(24)

except:
    cap.terminate()
finally:
    cap.terminate()