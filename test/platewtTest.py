import cv2
import numpy as np


# 获取色环的圆心像素坐标
def getCircleCenter(img:np.ndarray):
    result = []
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_binary = cv2.adaptiveThreshold(~img_gray, 255,
                                cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 15, -5)
    img_binary = cv2.medianBlur(img_binary, 3)
    circles = cv2.HoughCircles(img_binary, cv2.HOUGH_GRADIENT, 1, 100)
    if circles is not None and len(circles) != 0:
        circles = np.round(circles[0, :]).astype('int')
        for (x, y, r) in circles:
            result.append(tuple([x, y, r]))
    return result


def useRateMWB(img: np.ndarray, rateTuple: tuple):
    rb, rg, rr = cv2.split(img.astype(np.int64))
    rb = rb * rateTuple[0]
    rg = rg * rateTuple[1]
    rr = rr * rateTuple[2]
    result = np.stack((rb, rg, rr), axis=2)
    result = np.clip(result, 0, 255).astype(np.uint8)
    return result

stop = False
cap = cv2.VideoCapture("/dev/cameraMain")
a = cap.set(3, 640)
a = cap.set(4, 480)
a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

def stopHandle(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True

while True:
    ret, frame = cap.read()
    img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # img_binary = cv2.adaptiveThreshold(~img_gray, 255,
    #                             cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 15, -5)
    t2, img_binary = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    # img_binary = cv2.adaptiveThreshold(img_gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 25, 10)
    cv2.imshow("bin", ~img_binary)
    cv2.waitKey(4)
    if stop:
        break

cap.release()