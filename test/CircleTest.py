import cv2
import numpy as np
from XmlProcess import *


# 获取色环的圆心像素坐标
def getCircleCenter(img:np.ndarray):
    result = []
    # img_calc = cv2.GaussianBlur(img, (3, 3), 0)
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    img_binary = cv2.adaptiveThreshold(~img_gray, 255,
                                cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 15, -5)
    # erode_kernel = np.ones((1, 1), dtype=np.uint8)
    # erosion_binary = cv2.erode(img_binary, kernel=erode_kernel, iterations=1)
    img_binary = cv2.medianBlur(img_binary, 3)
    cv2.imshow("video in deal", img_binary)
    circles = cv2.HoughCircles(img_binary, cv2.HOUGH_GRADIENT, 1, 100)
    
    # circles = cv2.HoughCircles(img_gray, cv2.HOUGH_GRADIENT, 1, 100)
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


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    a = cap.set(3, 640)
    a = cap.set(4, 480)
    a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
    # # 获取相机参数
    paraDomTree = ElementTree.parse("./setting/capSetting.xml")
    brightness = float(paraDomTree.find("brightness").text)
    contrast = float(paraDomTree.find("contrast").text)
    saturation = float(paraDomTree.find("saturation").text)
    hue = float(paraDomTree.find("hue").text)
    a = cap.set(cv2.CAP_PROP_BRIGHTNESS, brightness)
    a = cap.set(cv2.CAP_PROP_CONTRAST, contrast)
    a = cap.set(cv2.CAP_PROP_SATURATION, saturation)
    a = cap.set(cv2.CAP_PROP_HUE, hue)

    # 调整白平衡
    paraDomTree = ElementTree.parse("./setting/rateTuple.xml")
    rateb = float(paraDomTree.find("rateb").text)
    rateg = float(paraDomTree.find("rateg").text)
    rater = float(paraDomTree.find("rater").text)
    rateTuple = (rateb, rateg, rater)
    
    while True:
        ret, frame = cap.read()
        frame = useRateMWB(frame, rateTuple)
        circles = getCircleCenter(frame)
        print("circle num:", len(circles))
        for c in circles:
            x, y, r = c
            cv2.circle(frame, (x, y), r, (255, 0, 0), 2)
        cv2.imshow("img", frame)
        cv2.waitKey(5)
