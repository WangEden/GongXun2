import cv2
import numpy as np


screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面
screen_blank_area = np.ones((480, 384, 3), np.uint8) * 255 # 空白区域
screen_qrcode_area = np.ones((120, 1024, 3), np.uint8) * 255 # 信息显示区域


def display(frame: np.ndarray):
    global screen, screen_qrcode_area, screen_blank_area

    temp = np.hstack([frame, screen_blank_area])
    screen = np.vstack([screen_qrcode_area, temp])
    cv2.imshow("screen", screen)


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        if not ret:
            continue
        display(frame)
        cv2.waitKey(25)



