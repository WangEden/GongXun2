import cv2
import numpy as np
import copy


cap = cv2.VideoCapture("/dev/video0")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

# 获取默认亮度值
brightness = cap.get(cv2.CAP_PROP_BRIGHTNESS)
print("默认亮度值:", brightness)
# 获取默认对比度值
contrast = cap.get(cv2.CAP_PROP_CONTRAST)
print("默认对比度:", contrast)
# 获取默认饱和度值
saturation = cap.get(cv2.CAP_PROP_SATURATION)
print("默认饱和度:", saturation)
# 获取默认色调值
hue = cap.get(cv2.CAP_PROP_HUE)
print("默认色调值:", hue)
def callback(event):
    pass


cv2.namedWindow("img")
cv2.createTrackbar("Brightness", "img", 0, 200, callback)
cv2.createTrackbar("Contrast", "img", 0, 100, callback)
cv2.createTrackbar("Saturation", "img", 0, 100, callback)
cv2.createTrackbar("Hue", "img", 0, 100, callback)

while True:
    ret, frame = cap.read()
    brightness = cv2.getTrackbarPos("Brightness", "img", )
    contrast = cv2.getTrackbarPos("Contrast", "img", )
    saturation = cv2.getTrackbarPos("Saturation", "img", )
    hue = cv2.getTrackbarPos("Hue", "img", )

    cap.set(cv2.CAP_PROP_BRIGHTNESS, brightness - 100)
    cap.set(cv2.CAP_PROP_CONTRAST, contrast)
    cap.set(cv2.CAP_PROP_SATURATION, saturation)
    cap.set(cv2.CAP_PROP_HUE, hue)

    brightness = cap.get(cv2.CAP_PROP_BRIGHTNESS)
    contrast = cap.get(cv2.CAP_PROP_CONTRAST)
    saturation = cap.get(cv2.CAP_PROP_SATURATION)
    hue = cap.get(cv2.CAP_PROP_HUE)
    print("亮度值:", brightness, "对比度:", contrast, "饱和度:", saturation, "色调值:", hue, end="\r")
    frame = cv2.resize(frame, (int(frame.shape[1] / 3 * 2), int(frame.shape[0] / 3 * 2)))
    cv2.imshow("img", frame)
    cv2.waitKey(25)
