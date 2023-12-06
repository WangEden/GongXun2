import cv2
import numpy as np
from multiprocessing import Queue
from utils.Communication import * 
from utils.XmlProcess import xmlReadCommand

# 任务二
# 取物块
def Task4_PutOnRing6(cameraPath: str,
                       queue: Queue,
                       sequence: list,
                       loop: int):

    isflip = False
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = file.read()
        if int(color) == 0: # 黑车 摄像头不反
            isflip = False
        elif int(color) == 1: # 白车
            isflip = True

    blank = np.ones((480, 640, 3), np.uint8) * 255
    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"去暂存区", (384, 200), (0, 0, 0), 45)

    while True: # 等待到达三色环区
        response = recv_data()
        queue.put(img)
        if response == xmlReadCommand("arrive", 0):
            break

    # 读取抓取顺序
    if loop == 1:
        rank = np.array(sequence[0:3])
    else:
        rank = np.array(sequence[3:6])
    rank = (rank - 1).tolist()
    
    # 读取阈值
    from utils.XmlProcess import xmlReadRingThreshold
    threshold = [[], [], []]  # -> [[min, max], [min, max], [min, max]]
    for i, c in enumerate(["red", "green", "blue"]):
        threshold[i] = xmlReadRingThreshold(c)
    
    # print(threshold)

    # 读取相机参数和白平衡参数
    from utils.XmlProcess import xmlReadCapSettings, xmlReadRateTuple
    CapSetting = xmlReadCapSettings()
    RateTuple = xmlReadRateTuple()
    
    # 设置相机参数
    cap = cv2.VideoCapture(cameraPath)
    a = cap.set(3, 640)
    a = cap.set(4, 480)
    a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
    a = cap.set(cv2.CAP_PROP_BRIGHTNESS, CapSetting[0])
    a = cap.set(cv2.CAP_PROP_CONTRAST, CapSetting[1])
    a = cap.set(cv2.CAP_PROP_SATURATION, CapSetting[2])
    a = cap.set(cv2.CAP_PROP_HUE, CapSetting[3])

    # 获取当前车的光心
    from utils.XmlProcess import xmlReadLightCenter
    XCenter, YCenter = xmlReadLightCenter()

    from utils.MWB import useRateMWB
    from utils.VisionUtils import getCircleCenter, cv2AddChineseText
    from math import pow
    import time

    RealDistance = 150 # mm
    PixelDistance = 320 # picel
    distanceRate = RealDistance / PixelDistance

    f = 2
    # 底下那个靠近中间的是绿色色环，如果OPS9没出错
    while True:
        n = 10
        circlesList = []
        circles = []
        while n > 0:
            ret, frame = cap.read()
            if isflip:
                frame = cv2.flip(frame, -1)
            circles = getCircleCenter(frame) # 获取画面中的圆形
            # centerCircle = ()
            if len(circles) != 0:
                centerCircle = sorted(circles, key=lambda circle: # 离光心最近的圆形
                             pow(circle[0] - XCenter, 2) + pow(circle[1] - YCenter, 2))[0]
                circlesList.append(tuple([centerCircle[0], centerCircle[1]]))
            n -= 1
        
        from utils.VisionUtils import getKmeansCenter
        circless = getKmeansCenter(1, circlesList)
        x, y = circless[0]   

        # 进行微调，发送偏差，不等回复，注意限制范围
        cv2.circle(frame, (XCenter, YCenter), 5, (255, 255, 255), 3)
        x, y, r = centerCircle
        cv2.circle(frame, (x, y), 5, (255, 128, 64), 3)
        cv2.circle(frame, (x, y), r, (255, 128, 64), 3)
        cv2.line(frame, (x, y), (XCenter, YCenter), (255, 255, 255), 2)

        dx = -(x - XCenter)
        dy = y - YCenter
        
        if f > 0:
            dxr = int(distanceRate * dx)
            dyr = int(distanceRate * dy)
            print("dy, dx:", dy, dx)
            cv2.putText(frame, f"(dy:{dy}, dx{dx})", (XCenter, YCenter), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
            send_dataDMA(xmlReadCommand("tweak", 1), dxr, dyr)
            queue.put(frame)
            cv2.imwrite(f"/home/jetson/GongXun2/app/debug/ring{f}_.jpg", frame)
            f -= 1
            time.sleep(3)
        if f == 0:
            break

    # 微调完等3s
    ret, frame_ = cap.read()
    cv2.circle(frame_, (XCenter, YCenter), 5, (0, 0, 0), 3)
    queue.put(frame_)
    time.sleep(3)
    cv2.imwrite("/home/jetson/GongXun2/app/debug/ring46.jpg", frame_)
    cap.release()

    # 按顺序放置和拿起物块，画面显示正在做的事
    COLOR = {0: "Red", 1: "Green", 2: "Blue"}
    COLOR2 = {0: "红", 1: "绿", 2: "蓝"}
    if loop == 1:
        for c in rank:
            send_dataDMA(xmlReadCommand(f"ring{COLOR[c]}", 1), 0, 0)
            blank = np.ones((480, 640, 3), np.uint8) * 255
            img = cv2AddChineseText(blank, f"在色环放{COLOR2[c]}物块", (384, 200), (0, 0, 0), 45)
            print(f"在色环放{COLOR2[c]}物块")
            # 等放完
            time.sleep(12)
    elif loop == 2:
        for c in rank:
            send_dataDMA(xmlReadCommand(f"md{COLOR[c]}", 1), 0, 0)
            blank = np.ones((480, 640, 3), np.uint8) * 255
            img = cv2AddChineseText(blank, f"在色环放{COLOR2[c]}物块", (384, 200), (0, 0, 0), 45)
            print(f"在色环放{COLOR2[c]}物块")
            # 等放完
            time.sleep(11)

    blank = np.ones((480, 640, 3), np.uint8) * 255

    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"回圆盘", (384, 200), (0, 0, 0), 45)
    queue.put(img)

    if loop == 1:
        send_dataDMA(xmlReadCommand("huiyuanpan", 1), 0, 0)
        print("zcq", xmlReadCommand("huiyuanpan", 0))
    elif loop == 2:
        send_dataDMA(xmlReadCommand("complete", 1), 0, 0)
        print("zcq", xmlReadCommand("complete", 0))


if __name__ == "__main__":
    pass