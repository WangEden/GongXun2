import cv2
import numpy as np
from xml.etree import ElementTree


def xmlReadLightCenter():
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = file.read()
        tag = int(color)
        paraDomTree = ElementTree.parse("../setting/lightCenter.xml")
        if tag == 0:
            item_node = paraDomTree.find(f'color[@tag="black"]')
        elif tag == 1:
            item_node = paraDomTree.find(f'color[@tag="white"]')
        x = int(item_node.find("x").text)
        y = int(item_node.find("y").text)
    return tuple([x, y])

stop = False
KeyCenter = (832, 240)
KeyLen = 60
LightCenterX, LightCenterY = xmlReadLightCenter()

def stopHandler(e, x, y, f, p):
    global stop, LightCenterX, LightCenterY
    if e == cv2.EVENT_LBUTTONDOWN:
        if 0 < x < 640 and 0 < y < 480:
            stop = True
        elif int(KeyCenter[0] - KeyLen * 3 / 2) < x < int(KeyCenter[0] - KeyLen / 2) and \
            int(KeyCenter[1] - KeyLen / 2) < y < int(KeyCenter[1] + KeyLen / 2):
            LightCenterX -= 1
        elif int(KeyCenter[0] + KeyLen / 2) < x < int(KeyCenter[0] + KeyLen * 3 / 2) and \
            int(KeyCenter[1] - KeyLen / 2) < y < int(KeyCenter[1] + KeyLen / 2):
            LightCenterX += 1
        elif int(KeyCenter[0] - KeyLen / 2) < x < int(KeyCenter[0] + KeyLen / 2) and \
            int(KeyCenter[1] - KeyLen * 3 / 2) < y < int(KeyCenter[1] - KeyLen / 2):
            LightCenterY -= 1
        elif int(KeyCenter[0] - KeyLen / 2) < x < int(KeyCenter[0] + KeyLen / 2) and \
           int(KeyCenter[1] + KeyLen / 2) < y < int(KeyCenter[1] + KeyLen * 3 / 2):
            LightCenterY += 1


cv2.namedWindow("screen", cv2.WINDOW_NORMAL)
cv2.setWindowProperty("screen", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
cv2.setMouseCallback("screen", stopHandler)


cap = cv2.VideoCapture("/dev/cameraMain")
a = cap.set(3, 640)
a = cap.set(4, 480)
a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面


# 左
cv2.rectangle(screen, (int(KeyCenter[0] - KeyLen * 3 / 2), int(KeyCenter[1] - KeyLen / 2)),
               (int(KeyCenter[0] - KeyLen / 2), int(KeyCenter[1] + KeyLen / 2)), (0, 0, 0), -1)
cv2.putText(screen, "left", (int(KeyCenter[0] - KeyLen * 3 / 2), int(KeyCenter[1] + KeyLen / 2)), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
# 右
cv2.rectangle(screen, (int(KeyCenter[0] + KeyLen / 2), int(KeyCenter[1] - KeyLen / 2)),
               (int(KeyCenter[0] + KeyLen * 3 / 2), int(KeyCenter[1] + KeyLen / 2)), (0, 0, 0), -1)
cv2.putText(screen, "right", (int(KeyCenter[0] + KeyLen / 2), int(KeyCenter[1] + KeyLen / 2)), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
# 上
cv2.rectangle(screen, (int(KeyCenter[0] - KeyLen / 2), int(KeyCenter[1] - KeyLen * 3 / 2)),
               (int(KeyCenter[0] + KeyLen / 2), int(KeyCenter[1] - KeyLen / 2)), (0, 0, 0), -1)
cv2.putText(screen, "up", (int(KeyCenter[0] - KeyLen / 2), int(KeyCenter[1] - KeyLen / 2)), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
# 下
cv2.rectangle(screen, (int(KeyCenter[0] - KeyLen / 2), int(KeyCenter[1] + KeyLen / 2)),
               (int(KeyCenter[0] + KeyLen / 2), int(KeyCenter[1] + KeyLen * 3 / 2)), (0, 0, 0), -1)
cv2.putText(screen, "down", (int(KeyCenter[0] - KeyLen / 2), int(KeyCenter[1] + KeyLen * 3 / 2)), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)

isflip = False
with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
    color = file.read()
    if int(color) == 0: # 黑车
        isflip = False
    elif int(color) == 1: # 白车
        isflip = True

while not stop:
    ret, frame = cap.read()
    if isflip:
        frame = cv2.flip(frame, -1)
    cv2.circle(frame, (LightCenterX, LightCenterY), 5, (255, 0, 0), 2)
    cv2.putText(frame, f"({LightCenterX}, {LightCenterY})", (LightCenterX, LightCenterY), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)
    cv2.line(frame, (0, LightCenterY), (640, LightCenterY), (255, 0, 0), 1)
    cv2.line(frame, (LightCenterX, 0), (LightCenterX, 480), (255, 0, 0), 1)
    screen[0:480, 0:640] = frame
    cv2.imshow("screen", screen)
    cv2.waitKey(5)

from xml.etree import ElementTree
with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
    color = file.read()
    tag = int(color)
    paraDomTree = ElementTree.parse("../setting/lightCenter.xml")
    if tag == 0:
        item_node = paraDomTree.find(f'color[@tag="black"]')
    elif tag == 1:
        item_node = paraDomTree.find(f'color[@tag="white"]')
    item_node.find("x").text = str(LightCenterX)
    item_node.find("y").text = str(LightCenterY)
    paraDomTree.write("../setting/lightCenter.xml")
