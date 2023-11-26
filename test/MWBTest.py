# 手动白平衡
import cv2
import numpy as np

# cap = cv2.VideoCapture("/dev/video0")


"""
notes:
1. 提高图像的色温:
    黑体在受热后, 发出的辐射光颜色应该是由黑-->红-->黄-->白-->蓝的变化规律,
    基于此, 对于RGB图像, 若要提高图像的色温, 应对R和G通道进行增强，B通道进行削弱，反之同理。


"""
def limit(num):
    if num > 255: return 255
    elif num < 0: return 0
    else: return num


def imageColorTemperatureGain(img: np.ndarray, gain: int):
    img_result = img.copy()
    rows, cols, channel = img.shape
    level = np.round(gain / 2)
    for i in range(rows):
        for j in range(cols): # img's color module is BGR
            img_result[i, j, 2] = limit(img[i, j, 2] + level)
            img_result[i, j, 1] = limit(img[i, j, 1] + level)
            img_result[i, j, 0] = limit(img[i, j, 0] + level)
    return img_result
        
    
def fineTuneColorTemperature():
    # 将图像分块

    # 
    pass


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        gain = imageColorTemperatureGain(img=frame, gain=100)
        out = np.hstack([frame, gain])
        cv2.imshow("show", out)
        if cv2.waitKey(24) & 0XFF == ord('q'):
            break