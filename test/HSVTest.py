from XmlProcess import *
import cv2
import numpy as np


stop = False


def mask_find_b_boxs(_mask):
    retval, labels, stats, centroids = cv2.connectedComponentsWithStats(
        _mask, connectivity=8
    )
    stats = stats[stats[:, 4].argsort()]
    return stats[:-1]


def get_the_most_credible_box(b_box):
    if len(b_box) == 0:
        return None
    if len(b_box) == 1:
        return b_box[0]
    b_box = sorted(b_box, key=lambda box: box[4], reverse=True)
    return b_box[0]


def stopHandle(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True


def useRateMWB(img: np.ndarray, rateTuple: tuple):
    rb, rg, rr = cv2.split(img.astype(np.int64))
    rb = rb * rateTuple[0]
    rg = rg * rateTuple[1]
    rr = rr * rateTuple[2]
    result = np.stack((rb, rg, rr), axis=2)
    result = np.clip(result, 0, 255).astype(np.uint8)
    return result


cap = cv2.VideoCapture("/dev/video0")
a = cap.set(3, 640)
a = cap.set(4, 480)
a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
print(cap, a)


if __name__ == "__main__":
    # # 获取色环和色块的阈值 # # # # # # # # # # # # # # # # # #
    thresholdItem = [[], [], []]  # -> [[min, max], [min, max], [min, max]]
    for i, c in enumerate(["red", "green", "blue"]):
        xmlReadThreshold("item", c, thresholdItem[i])

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
    print(a)

    # 调整白平衡
    paraDomTree = ElementTree.parse("./setting/rateTuple.xml")
    rateb = float(paraDomTree.find("rateb").text)
    rateg = float(paraDomTree.find("rateg").text)
    rater = float(paraDomTree.find("rater").text)
    rateTuple = (rateb, rateg, rater)

    while True:
        ret, frame = cap.read()
        if not ret:
            # print("no camera")
            continue

        mwbImg = useRateMWB(frame, rateTuple)
        mwbImg = cv2.GaussianBlur(mwbImg, (3, 3), 0)
        mwbImg_hsv = cv2.cvtColor(mwbImg, cv2.COLOR_BGR2HSV)
        
        mask1 = cv2.inRange(mwbImg_hsv, thresholdItem[0][0], thresholdItem[0][1])
        mask2 = cv2.inRange(mwbImg_hsv, thresholdItem[1][0], thresholdItem[1][1])
        mask3 = cv2.inRange(mwbImg_hsv, thresholdItem[2][0], thresholdItem[2][1])

        b_box1 = mask_find_b_boxs(mask1)
        b_box2 = mask_find_b_boxs(mask2)
        b_box3 = mask_find_b_boxs(mask3)

        box1 = get_the_most_credible_box(b_box1)
        box2 = get_the_most_credible_box(b_box2)
        box3 = get_the_most_credible_box(b_box3)

        print("box1:", box1)
        print("box2:", box2)
        print("box3:", box3)

        # if box1 is not None:
        #     cv2.rectangle(mwbImg, (box1[0], box1[1]), (box1[0] + box1[2], box1[1] + box1[3]), (0, 0, 255), 2)
        #     cv2.putText(mwbImg, "red", (box1[0], box1[1]), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2, cv2.LINE_AA)
        # if box2 is not None:
        #     cv2.rectangle(mwbImg, (box2[0], box2[1]), (box2[0] + box2[2], box2[1] + box2[3]), (0, 255, 0), 2)
        #     cv2.putText(mwbImg, "green", (box2[0], box2[1]), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
        # if box3 is not None:
        #     cv2.rectangle(mwbImg, (box3[0], box3[1]), (box3[0] + box3[2], box3[1] + box3[3]), (255, 0, 0), 2)
        #     cv2.putText(mwbImg, "blue", (box3[0], box3[1]), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)

        if stop:
            cv2.imwrite("HSVTestResult.jpg", mwbImg)
            break

        cv2.imshow("img", mwbImg)
        cv2.setMouseCallback("img", stopHandle)
        cv2.waitKey(25)
