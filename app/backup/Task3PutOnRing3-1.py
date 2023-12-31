import cv2
import numpy as np
from multiprocessing import Queue
from utils.Communication import * 
from utils.XmlProcess import xmlReadCommand


# 任务三
# 放取物块
def Task3_PutOnRing3(cameraPath: str,
                       queue: Queue,
                       sequence: list):
    
    # send_dataDMA("QROK", 0, 0)
    # time.sleep(1)
    
    blank = np.ones((480, 640, 3), np.uint8) * 255
    
    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"去粗加工区", (384, 200), (0, 0, 0), 45)

    # while True: # 等待到达三色环区
    #     queue.put(img)
    #     response = recv_data()
    #     if response == xmlReadCommand("arrive", 0):
    #         break

    send_dataDMA(xmlReadCommand("prepareCatch", 1), 0, 0)
    import time
    # 等待机械臂运动
    time.sleep(3)

    # 读取抓取顺序
    rank = np.array(sequence[0:3])
    rank = (rank - 1).tolist()
    
    # 读取阈值
    from utils.XmlProcess import xmlReadRingThreshold
    threshold = [[], [], []]  # -> [[min, max], [min, max], [min, max]]
    for i, c in enumerate(["red", "green", "blue"]):
        threshold[i] = xmlReadRingThreshold(c)
    
    print(threshold)

    # 读取相机参数和白平衡参数
    from utils.XmlProcess import xmlReadCapSettings, xmlReadRateTuple
    CapSetting = xmlReadCapSettings()
    RateTuple = xmlReadRateTuple()
    
    # 设置相机参数
    cap = cv2.VideoCapture(0)
    a = cap.set(3, 640)
    a = cap.set(4, 480)
    a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
    a = cap.set(cv2.CAP_PROP_BRIGHTNESS, CapSetting[0])
    a = cap.set(cv2.CAP_PROP_CONTRAST, CapSetting[1])
    a = cap.set(cv2.CAP_PROP_SATURATION, CapSetting[2])
    a = cap.set(cv2.CAP_PROP_HUE, CapSetting[3])
    print(a)

    # 获取当前车的光心
    from utils.XmlProcess import xmlReadLightCenter
    XCenter, YCenter = xmlReadLightCenter()

    from utils.MWB import useRateMWB
    from utils.VisionUtils import getCircleCenter, cv2AddChineseText
    from math import pow

    # 底下那个靠近中间的是绿色色环，如果OPS9没出错
    f = True
    while True:
        # 先锁定中间那个圆环
        ret, frame = cap.read()
        frame = cv2.flip(frame, -1)
        frame = useRateMWB(frame, RateTuple)
        circles = getCircleCenter(frame) # 获取画面中的圆形
        if len(circles) != 0:
            centerCircle = sorted(circles, key=lambda circle: # 离光心最近的圆形
                             pow(circle[0] - XCenter, 2) + pow(circle[1] - YCenter, 2))[0]
        # 进行微调，不断发送偏差，不等回复，注意限制范围
            cv2.circle(frame, (XCenter, YCenter), 5, (255, 255, 255), 3)
            x, y, r = centerCircle
            cv2.circle(frame, (x, y), r, (255, 128, 64), 3)
            dy = x - XCenter
            dx = y - YCenter
            if -10 < dx < 10:
                dx = 0
            if -10 < dy < 10:
                dy = 0
            print("dy, dx:", dy, dx)
            cv2.putText(frame, f"(dy:{dy}, dx{dx})", (XCenter, YCenter), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
            if f:
                send_dataDMA(xmlReadCommand("tweak", 1), dx, dy)
                break
                f = False
            else:
                send_dataDMA("0000", dx, dy)
                queue.put(frame)
        # 待偏差归零，停止投送画面
            if abs(dy) < 5 and abs(dx) < 5:
                print("准了, 像素误差小于5:", dy, dx)
                # send_dataDMA(xmlReadCommand("calibrOk", 1), 0, 0)
                queue.put(np.ones((480, 640, 3), np.uint8) * 255)
                break
        else:
            queue.put(frame)
    cap.release()
    # 按顺序放置和拿起物块，画面显示正在做的事
    COLOR = {0: "red", 1: "green", 2: "blue"}
    COLOR2 = {0: "红", 1: "绿", 2: "蓝"}
    for c in rank:
        send_dataDMA(xmlReadCommand(f"catch{COLOR[c]}", 1), 0, 0)
        blank = np.ones((480, 640, 3), np.uint8) * 255
        img = cv2AddChineseText(blank, f"放{COLOR2[c]}物块", (384, 200), (0, 0, 0), 45)
        print(f"放{COLOR2[c]}物块")
        while True:
            response = recv_data()
            queue.put(img)
            if response == xmlReadCommand("mngOK", 0):
                break

    for c in rank:
        send_dataDMA(xmlReadCommand(f"catch{COLOR[c]}", 1), 0, 0)
        blank = np.ones((480, 640, 3), np.uint8) * 255
        img = cv2AddChineseText(blank, f"抓{COLOR2[c]}物块", (384, 200), (0, 0, 0), 45)
        print(f"抓{COLOR2[c]}物块")
        while True:
            response = recv_data()
            queue.put(img)
            if response == xmlReadCommand("mngOK", 0):
                break
        
    send_dataDMA(xmlReadCommand("calibrOk", 1), 0, 0)
    # send_dataDMA(xmlReadCommand("complete", 1), 0, 0)
    # print(xmlReadCommand("complete", 0))

    blank = np.ones((480, 640, 3), np.uint8) * 255
    
    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"去暂存区", (384, 200), (0, 0, 0), 45)
    queue.put(img)


if __name__ == "__main__":
    pass