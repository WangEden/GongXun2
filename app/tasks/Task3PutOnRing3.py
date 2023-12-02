import cv2
import numpy as np
from multiprocessing import Queue
from utils.Communication import * 
from utils.XmlProcess import xmlReadCommand

# 任务二
# 取物块
def Task3_PutOnRing3(cameraPath: str,
                       queue: Queue,
                       sequence: list):
    
    # while True: # 等待到达原料区
    #     response = recv_data()
    #     if response == xmlReadCommand("arriveYL", 0):
    #         break

    # 读取抓取顺序
    rank = np.array(sequence[0:3])
    rank = (rank - 1).tolist()
    
    # 读取阈值
    from utils.XmlProcess import xmlReadItemThreshold
    threshold = [[], [], []]  # -> [[min, max], [min, max], [min, max]]
    for i, c in enumerate(["red", "green", "blue"]):
        threshold[i] = xmlReadItemThreshold(c)
    
    print(threshold)

    # 读取相机参数和白平衡参数
    from utils.XmlProcess import xmlReadCapSettings, xmlReadRateTuple
    CapSetting = xmlReadCapSettings()
    RateTuple = xmlReadRateTuple()
    
    cap = cv2.VideoCapture(0)
    a = cap.set(cv2.CAP_PROP_BRIGHTNESS, CapSetting[0])
    a = cap.set(cv2.CAP_PROP_CONTRAST, CapSetting[1])
    a = cap.set(cv2.CAP_PROP_SATURATION, CapSetting[2])
    a = cap.set(cv2.CAP_PROP_HUE, CapSetting[3])

    from utils.MWB import useRateMWB
    import time
    from utils.VisionUtils import moving_detect, mask_find_b_boxs, get_the_most_credible_box

    while True:
        pass
            

if __name__ == "__main__":
    pass