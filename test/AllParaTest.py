import cv2
import numpy as np


def handler(event, x, y, flags, param):
    pass


# 创建画面
cv2.namedWindow("screen", cv2.WINDOW_NORMAL)
cv2.setWindowProperty("screen", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
cv2.setMouseCallback("screen", handler)


# 画面中位置
position = {
    "qrcode": [0, 0, 1024, 120],
    "panel": [0, 120, 384, 480],
    "img": [384, 120, 640, 480],
}

# screen
screen_path = "./data/screen.jpg"


if __name__ == "__main__":
    screen = cv2.imread(screen_path)
    
