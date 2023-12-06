import cv2
import numpy as np
import copy


cap = cv2.VideoCapture("/dev/cameraMain")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

# 获取默认亮度值 0
brightness = cap.get(cv2.CAP_PROP_BRIGHTNESS)
print("默认亮度值:", brightness)
# 获取默认对比度值 35
contrast = cap.get(cv2.CAP_PROP_CONTRAST)
print("默认对比度:", contrast)
# 获取默认饱和度值 80
saturation = cap.get(cv2.CAP_PROP_SATURATION)
print("默认饱和度:", saturation)
# 获取默认色调值 0
hue = cap.get(cv2.CAP_PROP_HUE)
print("默认色调值:", hue)
def callback(event):
    pass


isflip = False
with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
    color = file.read()
    if int(color) == 0: # 黑车
        isflip = False
    elif int(color) == 1: # 白车
        isflip = True


stop = False
def stopHandle(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        if 0 < x < 50 and 0 < y < 50:
            stop = True

from xml.etree import ElementTree
def xmlReadCapSettings() -> tuple:
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue"
    }
    result = []
    paraDomTree = ElementTree.parse("../setting/capSetting.xml")
    for i in range(4):
        item_node = paraDomTree.find(para[i])
        result.append(float(item_node.text))
    return tuple(result)

capSetting = xmlReadCapSettings()

cv2.namedWindow("img")
cv2.createTrackbar("Brightness", "img", int(capSetting[0]) + 100, 200, callback)
cv2.createTrackbar("Contrast", "img", int(capSetting[1]), 100, callback)
cv2.createTrackbar("Saturation", "img", int(capSetting[2]), 100, callback)
cv2.createTrackbar("Hue", "img", int(capSetting[3]), 100, callback)

def useRateMWB(img: np.ndarray, rateTuple: tuple):
    rb, rg, rr = cv2.split(img.astype(np.int64))
    rb = rb * rateTuple[0]
    rg = rg * rateTuple[1]
    rr = rr * rateTuple[2]
    result = np.stack((rb, rg, rr), axis=2)
    result = np.clip(result, 0, 255).astype(np.uint8)
    return result

# 调整白平衡
paraDomTree = ElementTree.parse("../setting/rateTuple.xml")
rateb = float(paraDomTree.find("rateb").text)
rateg = float(paraDomTree.find("rateg").text)
rater = float(paraDomTree.find("rater").text)
rateTuple = (rateb, rateg, rater)
# rateTuple = (0.9953363416315475, 0.9660302745350318, 0.9843837281643676)

while True:
    brightness = cv2.getTrackbarPos("Brightness", "img", )
    contrast = cv2.getTrackbarPos("Contrast", "img", )
    saturation = cv2.getTrackbarPos("Saturation", "img", )
    hue = cv2.getTrackbarPos("Hue", "img", )

    cap.set(cv2.CAP_PROP_BRIGHTNESS, brightness - 100)
    cap.set(cv2.CAP_PROP_CONTRAST, contrast)
    cap.set(cv2.CAP_PROP_SATURATION, saturation)
    cap.set(cv2.CAP_PROP_HUE, hue)

    ret, frame = cap.read()
    if isflip:
        frame = cv2.flip(frame, -1)
    frame = useRateMWB(frame, rateTuple)
    frame = cv2.GaussianBlur(frame, (3, 3), 0)

    brightness_ = cap.get(cv2.CAP_PROP_BRIGHTNESS)
    contrast = cap.get(cv2.CAP_PROP_CONTRAST)
    saturation = cap.get(cv2.CAP_PROP_SATURATION)
    hue = cap.get(cv2.CAP_PROP_HUE)

    print("亮度值:", brightness_, "对比度:", contrast, "饱和度:", saturation, "色调值:", hue)

    if stop:
        # 保存相机参数
        paraDomTree = ElementTree.parse("../setting/capSetting.xml")
        paraDomTree.find("brightness").text = str(brightness_)
        paraDomTree.find("contrast").text = str(contrast - 0.0)
        paraDomTree.find("saturation").text = str(saturation - 0.0)
        paraDomTree.find("hue").text = str(hue - 0.0)
        paraDomTree.write("../setting/capSetting.xml")
        break

    frame2 = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    out = np.hstack([frame, frame2])
    out = cv2.resize(out, (int(out.shape[1] /  2), int(out.shape[0] /  2)))
    cv2.rectangle(out, (0, 0), (50, 50), (255, 0, 0), -1)
    cv2.imshow("img", out)
    cv2.setMouseCallback("img", stopHandle)
    cv2.waitKey(25)
cap.release()