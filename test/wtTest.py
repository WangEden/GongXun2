import cv2
import numpy as np


def wt():
    sequence = [2, 3, 1, 3, 1, 2]
    loop = 1

    # 读取抓取顺序
    if loop == 1:
        rank = np.array(sequence[0:3])
    elif loop == 2:
        rank = np.array(sequence[3:6])
    rank = (rank - 1).tolist()

    # 读取阈值
    from XmlProcess2 import xmlReadItemThreshold
    threshold = [[], [], []]  # -> [[min, max], [min, max], [min, max]]
    for i, c in enumerate(["red", "green", "blue"]):
        threshold[i] = xmlReadItemThreshold(c)

    # print(threshold)

    # 读取相机参数和白平衡参数
    from XmlProcess import xmlReadCapSettings, xmlReadRateTuple
    CapSetting = xmlReadCapSettings()
    RateTuple = xmlReadRateTuple()

    isflip = False
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = file.read()
        if int(color) == 0: # 黑车
            isflip = False
        elif int(color) == 1: # 白车
            isflip = True
    
    cap = cv2.VideoCapture("/dev/cameraMain")
    a = cap.set(3, 640)
    a = cap.set(4, 480)
    a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
    a = cap.set(cv2.CAP_PROP_BRIGHTNESS, CapSetting[0])
    a = cap.set(cv2.CAP_PROP_CONTRAST, CapSetting[1])
    a = cap.set(cv2.CAP_PROP_SATURATION, CapSetting[2])
    a = cap.set(cv2.CAP_PROP_HUE, CapSetting[3])

    from MWB import useRateMWB
    import time
    from VisionUtils import moving_detect, mask_find_b_boxs, get_the_most_credible_box

    while True:
        # 判断圆盘是否在运动
        ret, last_frame = cap.read()
        if isflip:
            last_frame = cv2.flip(last_frame, -1)
        start_time = time.time()
        is_plate_move = True
        c = 0
        while is_plate_move:
            end_time = time.time()
            if end_time - start_time > 0.3:
                start_time = time.time()
                ret, last_frame = cap.read()
                if isflip:
                    last_frame = cv2.flip(last_frame, -1)
            
            ret, current_frame = cap.read()
            if isflip:
                current_frame = cv2.flip(current_frame, -1)
            
            # 圆盘没在移动时退出
            is_plate_move = moving_detect(last_frame, current_frame)
            if is_plate_move:
                cv2.putText(current_frame, "move", (320 - 7 * 20, 50 + 20), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255, 0, 0), 4, cv2.LINE_AA)
                # queue.put(current_frame)
                cv2.imshow()
                c = 0
            else: c += 1
            if c > 5:
                break

    

    cap.release()
    
wt()