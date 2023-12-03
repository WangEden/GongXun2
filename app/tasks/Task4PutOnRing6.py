import cv2
import numpy as np
from multiprocessing import Queue
from utils.Communication import * 
from utils.XmlProcess import xmlReadCommand

# 任务二
# 取物块
def Task4_PutOnRing6(cameraPath: str,
                       queue: Queue,
                       sequence: list):

    # while True: # 等待到达三色环区
    #     response = recv_data()
    #     if response == xmlReadCommand("arrive", 0):
    #         break

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

    # 获取当前车的光心
    from utils.XmlProcess import xmlReadLightCenter
    XCenter, YCenter = xmlReadLightCenter()

    from utils.MWB import useRateMWB
    from utils.VisionUtils import getCircleCenter, cv2AddChineseText
    from math import pow

    # 底下那个靠近中间的是绿色色环，如果OPS9没出错
    while True:
        # 先锁定中间那个圆环
        ret, frame = cap.read()
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
            print("dy, dx:", dy, dx)
            send_data(xmlReadCommand("tweak", 0), dx, dy)
            queue.put(frame)
        # 待偏差归零，停止投送画面
            if abs(dy) < 5 and abs(dx) < 5:
                print("准了, 像素误差小于5", dy, dx)
                send_cmd(xmlReadCommand("calibrOk", 0))
                queue.put(np.ones((480, 640, 3), np.uint8) * 255)
                break
    cap.release()
    # 按顺序放置和拿起物块，画面显示正在做的事
    COLOR = {0: "red", 1: "green", 2: "blue"}
    COLOR2 = {0: "红", 1: "绿", 2: "蓝"}
    for c in rank:
        send_cmd(xmlReadCommand(f"catch{COLOR[c]}", 0))
        blank = np.ones((480, 640, 3), np.uint8) * 255
        img = cv2AddChineseText(blank, f"放{COLOR2[c]}物块", (384, 200), (0, 0, 0), 45)
        queue.put(img)
        print(f"放{COLOR2[c]}物块")
        while True:
            response = recv_data()
            if response == xmlReadCommand("mngOK", 0):
                break

    queue.put(np.ones((480, 640, 3), np.uint8) * 255)


if __name__ == "__main__":
    pass