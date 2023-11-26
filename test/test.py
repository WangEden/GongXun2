# 平均灰度自动白平衡
import cv2
import numpy as np


cap = cv2.VideoCapture("/dev/video0")


while True:
    ret, frame = cap.read()
    if not ret:
        continue
    cv2.imwrite("asd.jpg", frame)
    break
    # cv2.imshow("s", frame)
    # cv2.waitKey(24)
    # img_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

