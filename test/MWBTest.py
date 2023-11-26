# 手动白平衡
import cv2
import numpy as np

cap = cv2.VideoCapture("/dev/video0")

"""
notes:
1. 提高图像的色温:
    黑体在受热后, 发出的辐射光颜色应该是由黑-->红-->黄-->白-->蓝的变化规律,
    基于此, 对于RGB图像, 若要提高图像的色温, 应对R和G通道进行增强，B通道进行削弱，反之同理。


"""

def imageColorTemperatureGain(img: np.ndarray):
    
    pass


def fineTuneColorTemperature():
    # 将图像分块

    # 
    pass


while True:
    ret, frame = cap.read()
    if not ret:
        continue
    cv2.imwrite("asd.jpg", frame)
    break
    # cv2.imshow("s", frame)
    # cv2.waitKey(24)
    # img_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

