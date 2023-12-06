import cv2
import numpy as np


stop = False
def stopHandle(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True

n = [0]
with open("./img.txt", "r") as file:
    s = file.read()
    n[0] = int(s)
    print(s)


from xml.etree import ElementTree
def xmlReadCapSettings() -> tuple:
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue"
    }
    result = []
    paraDomTree = ElementTree.parse("../../app/setting/capSetting.xml")
    for i in range(4):
        item_node = paraDomTree.find(para[i])
        result.append(float(item_node.text))
    return tuple(result)


def useRateMWB(img: np.ndarray, rateTuple: tuple):
    rb, rg, rr = cv2.split(img.astype(np.int64))
    rb = rb * rateTuple[0]
    rg = rg * rateTuple[1]
    rr = rr * rateTuple[2]
    result = np.stack((rb, rg, rr), axis=2)
    result = np.clip(result, 0, 255).astype(np.uint8)
    return result


capSetting = xmlReadCapSettings()
# 调整白平衡
paraDomTree = ElementTree.parse("../../app/setting/rateTuple.xml")
rateb = float(paraDomTree.find("rateb").text)
rateg = float(paraDomTree.find("rateg").text)
rater = float(paraDomTree.find("rater").text)
rateTuple = (rateb, rateg, rater)

# cap = cv2.VideoCapture(0)
cap = cv2.VideoCapture("/dev/cameraMain")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
cap.set(cv2.CAP_PROP_BRIGHTNESS, capSetting[0])
cap.set(cv2.CAP_PROP_CONTRAST, capSetting[1])
cap.set(cv2.CAP_PROP_SATURATION, capSetting[2])
cap.set(cv2.CAP_PROP_HUE, capSetting[3])

isflip = False
with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
    color = file.read()
    if int(color) == 0: # 黑车
        isflip = False
    elif int(color) == 1: # 白车
        isflip = True

brightness_ = cap.get(cv2.CAP_PROP_BRIGHTNESS)
contrast = cap.get(cv2.CAP_PROP_CONTRAST)
saturation = cap.get(cv2.CAP_PROP_SATURATION)
hue = cap.get(cv2.CAP_PROP_HUE)
print("亮度值:", brightness_, "对比度:", contrast, "饱和度:", saturation, "色调值:", hue)

while n[0] > 0:
    ret, frame = cap.read()
    if isflip:
        frame = cv2.flip(frame, -1)
    frame = useRateMWB(frame, rateTuple)
    frame = cv2.GaussianBlur(frame, (3, 3), 0)
    cv2.imshow("img", frame)
    cv2.setMouseCallback("img", stopHandle)
    if cv2.waitKey(10) & 0xFF == ord("q"):
        cv2.imwrite(f"./img{n[0]}.jpg", frame)
        with open("./img.txt", "w") as file:
            file.write(str(n[0] + 1))
        n[0] = 0
    
    if stop:
        cv2.imwrite(f"./take{n[0]}.jpg", frame)
        with open("./img.txt", "w") as file:
            file.write(str(n[0] + 1))
            break
