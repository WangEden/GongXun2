# 手动白平衡
import cv2
import numpy as np
import copy


cap = cv2.VideoCapture("/dev/cameraMain")
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

clickCnt = 2
mwbRect = []
stop = False

isflip = False
color = 0
COLOR = {0: "black", 1: "white"}
with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
    color = int(file.read())
    if int(color) == 0: # 黑车
        isflip = False
    elif int(color) == 1: # 白车
        isflip = True

def perfectReflection(img: np.ndarray): # 谨记要使用numpy优化
    use1 = copy.deepcopy(img).astype(np.int64)
    result = copy.deepcopy(img).astype(np.int64)
    sum_matrix = np.sum(use1, axis=2)  # 按z轴方向求和
    MaxVal = np.max(use1[0:120, 0:160, :])
    Threshold = np.max(sum_matrix[0:120, 0:160])
    temp_img = sum_matrix - Threshold
    temp_mask = np.clip(temp_img, 0, 1)
    b, g, r = cv2.split(use1)
    b = b * temp_mask
    size = img.shape[0] * img.shape[1]
    bn0 = np.count_nonzero(b == 0)
    AvgB = np.sum(b) / (size - bn0)
    g = g * temp_mask
    gn0 = np.count_nonzero(g == 0)
    AvgG = np.sum(g) / (size - gn0)
    r = r * temp_mask
    rn0 = np.count_nonzero(r == 0)
    AvgR = np.sum(r) / (size - rn0)
    rb, rg, rr = cv2.split(result)
    rb = rb * MaxVal / AvgB
    rg = rg * MaxVal / AvgG
    rr = rr * MaxVal / AvgR
    result = np.stack((rb, rg, rr), axis=2).astype(np.uint8)
    print("RateB:", MaxVal / AvgB, "RateG:", MaxVal / AvgG, "RateR:", MaxVal / AvgR)
    return result


def setPoinit(e, x, y, f, p):
    global clickCnt, mwbRect
    if e == cv2.EVENT_LBUTTONDOWN:
        clickCnt -= 1
        print((x, y))
        mwbRect.append((x, y))


def getRateBGR():
    global clickCnt, cap, mwbRect
    while True:
        ret, frame = cap.read()
        cv2.imshow("getRate", frame)
        cv2.setMouseCallback("getRate", setPoinit)
        cv2.waitKey(25)

        if clickCnt == 0:
            use1 = copy.deepcopy(frame).astype(np.int64)
            sum_matrix = np.sum(use1, axis=2)  # 按z轴方向求和
            print(mwbRect)
            MaxVal = np.max(use1[mwbRect[0][1]:mwbRect[1][1], mwbRect[0][0]:mwbRect[1][0], :])
            print("MaxVal", MaxVal)
            Threshold = np.max(sum_matrix[mwbRect[0][1]:mwbRect[1][1], mwbRect[0][0]:mwbRect[1][0]])
            temp_img = sum_matrix - Threshold
            temp_mask = np.clip(temp_img, 0, 1)
            b, g, r = cv2.split(use1)
            b = b * temp_mask
            size = frame.shape[0] * frame.shape[1]
            bn0 = np.count_nonzero(b == 0)
            AvgB = np.sum(b) / (size - bn0)
            g = g * temp_mask
            gn0 = np.count_nonzero(g == 0)
            AvgG = np.sum(g) / (size - gn0)
            r = r * temp_mask
            rn0 = np.count_nonzero(r == 0)
            AvgR = np.sum(r) / (size - rn0)
            rateB = MaxVal / AvgB
            rateG = MaxVal / AvgG
            rateR = MaxVal / AvgR
            if 0.6 < rateB < 1.4 and 0.6 < rateG < 1.4 and 0.6 < rateR < 1.4:
                cv2.destroyAllWindows()
                return (rateB, rateG, rateR)
            else:
                mwbRect.clear()
                clickCnt = 2
            print(rateB, rateG, rateR)


def useRateMWB(img: np.ndarray, rateTuple: tuple):
    rb, rg, rr = cv2.split(img.astype(np.int64))
    rb = rb * rateTuple[0]
    rg = rg * rateTuple[1]
    rr = rr * rateTuple[2]
    result = np.stack((rb, rg, rr), axis=2)
    result = np.clip(result, 0, 255).astype(np.uint8)
    return result


def stopHandle(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True


from xml.etree import ElementTree
def xmlReadCapSettings() -> tuple:
    global color
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue"
    }
    result = []
    paraDomTree = ElementTree.parse("../setting/capSetting.xml")
    item_node = None
    if color == 0:
        item_node = paraDomTree.find(f'color[@tag="black"]')
    elif color == 1:
        item_node = paraDomTree.find(f'color[@tag="white"]')
    for i in range(4):
        item = item_node.find(para[i])
        result.append(float(item.text))
    return tuple(result)


if __name__ == "__main__":
    # cap = cv2.VideoCapture(0)
    capSetting = xmlReadCapSettings()
    cap.set(cv2.CAP_PROP_BRIGHTNESS, capSetting[0])
    cap.set(cv2.CAP_PROP_CONTRAST, capSetting[1])
    cap.set(cv2.CAP_PROP_SATURATION, capSetting[2])
    cap.set(cv2.CAP_PROP_HUE, capSetting[3])

    rateTuple = getRateBGR()

    paraDomTree = ElementTree.parse("../setting/rateTuple.xml")
    item_node = None
    if color == 0:
        item_node = paraDomTree.find(f'color[@tag="black"]')
    elif color == 1:
        item_node = paraDomTree.find(f'color[@tag="white"]')
    rateb_node = item_node.find("rateb")
    rateg_node = item_node.find("rateg")
    rater_node = item_node.find("rater")
    rateb_node.text = str(rateTuple[0])
    rateg_node.text = str(rateTuple[1])
    rater_node.text = str(rateTuple[2])
    paraDomTree.write("../setting/rateTuple.xml")
    
    print(rateTuple)

    while True:
        ret, frame = cap.read()
        if isflip:
            frame = cv2.flip(frame, -1)
        gain_img = useRateMWB(frame, rateTuple)
        out = np.hstack([frame, gain_img])
        out = cv2.resize(out, (int(out.shape[1] / 3 * 2), int(out.shape[0] / 3 * 2)))
        cv2.imshow("show", out)
        cv2.setMouseCallback("show", stopHandle)
        if stop:
            # cv2.imwrite("afterMWB.jpg", gain_img)
            break
        if cv2.waitKey(24) & 0XFF == ord('q'):
            break
    cap.release()
