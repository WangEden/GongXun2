import numpy as np
import cv2


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


cap = cv2.VideoCapture(0)
a = cap.set(3, 640)
a = cap.set(4, 480)
a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

import math
while True:
    ret, frame = cap.read()
    circles = getCircleCenter(frame)
    print("circle num:", len(circles))
    if len(circles) == 2:
        x1, y1, r1 = circles[0]
        x2, y2, r2 = circles[1]
        distance = math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)
        print("distance:", distance)
        cv2.line(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)
        cv2.putText(frame, f"distance:{distance}", (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
        cv2.imshow("img", frame)
        cv2.waitKey(5)
