import cv2
import numpy as np
from multiprocessing import Queue
from utils.Communication import * 
from utils.XmlProcess import xmlReadCommand

# 任务二
# 取物块
def Task2_GetFromPlate(cameraPath: str,
                       queue: Queue,
                       sequence: list, 
                       loop: int):
    
    while True: # 等待到达原料区
        response = recv_data()
        if response == xmlReadCommand("arrive", 0):
            print("等待到达圆盘response:", response, end='\r')
            break
    
    blank = np.ones((480, 640, 3), np.uint8) * 255



    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"去圆盘", (384, 200), (0, 0, 0), 45)
    queue.put(img)

    isflip = False
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = file.read()
        if int(color) == 0: # 黑车
            isflip = False
        elif int(color) == 1: # 白车
            isflip = True

    # 读取抓取顺序
    if loop == 1:
        rank = np.array(sequence[0:3])
    elif loop == 2:
        rank = np.array(sequence[3:6])
    rank = (rank - 1).tolist()
    
    # 读取阈值
    from utils.XmlProcess import xmlReadItemThreshold
    threshold = [[], [], []]  # -> [[min, max], [min, max], [min, max]]
    for i, c in enumerate(["red", "green", "blue"]):
        threshold[i] = xmlReadItemThreshold(c)
    
    # print(threshold)

    # 读取相机参数和白平衡参数
    from utils.XmlProcess import xmlReadCapSettings, xmlReadRateTuple
    CapSetting = xmlReadCapSettings()
    RateTuple = xmlReadRateTuple()
    
    cap = cv2.VideoCapture(cameraPath)
    a = cap.set(3, 640)
    a = cap.set(4, 480)
    a = cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
    a = cap.set(cv2.CAP_PROP_BRIGHTNESS, CapSetting[0])
    a = cap.set(cv2.CAP_PROP_CONTRAST, CapSetting[1])
    a = cap.set(cv2.CAP_PROP_SATURATION, CapSetting[2])
    a = cap.set(cv2.CAP_PROP_HUE, CapSetting[3])

    from utils.MWB import useRateMWB
    import time
    from utils.VisionUtils import moving_detect, mask_find_b_boxs, get_the_most_credible_box

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
                queue.put(current_frame)
                c = 0
            else: c += 1
            if c > 5:
                break

        # 开始处理
        ret, frame = cap.read()
        if isflip:
            frame = cv2.flip(frame, -1)
        img = useRateMWB(frame, RateTuple)
        img = cv2.GaussianBlur(img, (3, 3), 0)

        # 假设物块在画面左下方，区域暂定为(0, )
        ROI = [0, 85, 220 - 0, 460 - 85]
        roi_img = np.zeros((480, 640, 3), np.uint8)
        img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        roi_img[ROI[1]:ROI[1]+ROI[3], ROI[0]:ROI[0]+ROI[2]] = \
            img_hsv[ROI[1]:ROI[1]+ROI[3], ROI[0]:ROI[0]+ROI[2]]

        mask1 = cv2.inRange(roi_img, threshold[0][0], threshold[0][1])
        mask2 = cv2.inRange(roi_img, threshold[1][0], threshold[1][1])
        mask3 = cv2.inRange(roi_img, threshold[2][0], threshold[2][1])

        b_box1 = mask_find_b_boxs(mask1)
        b_box2 = mask_find_b_boxs(mask2)
        b_box3 = mask_find_b_boxs(mask3)
        
        if len(b_box1) == 0 and len(b_box2) == 0 and len(b_box3) == 0:
            cv2.putText(roi_img, "no color", (ROI[0], ROI[1] - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
            queue.put(roi_img) # 没有识别到任何一个颜色，一般不可能，除非OPS9坏了
            continue
        else:
            box1 = get_the_most_credible_box(b_box1)
            box2 = get_the_most_credible_box(b_box2)
            box3 = get_the_most_credible_box(b_box3)
            # 不存在的颜色, 区域面积为0, 不会被选出来框中
            boxs = [box1, box2, box3]
            size = [box1[4], box2[4], box3[4]]
            max_index = size.index(max(size)) # 获取面积最大的那个索引，就是所识别到的颜色
            COLOR = {0: "Red", 1: "Green", 2: "Blue"}
            COLOR2 = {0: (0, 0, 255), 1: (0, 255, 0), 2: (255, 0, 0)}

            box = boxs[max_index] # 选红绿蓝最大的画框框
            cv2.rectangle(img, (box[0], box[1]), (box[0] + box[2], box[1] + box[3]), COLOR2[max_index], 2)
            cv2.putText(img, COLOR[max_index], (box[0], box[1]), cv2.FONT_HERSHEY_SIMPLEX, 1, COLOR2[max_index], 2, cv2.LINE_AA)

            if box[4] < 500:
                cv2.putText(img, "low size", (box[0] + 80, box[1]), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 128), 2, cv2.LINE_AA)
                queue.put(img)
            else:
                if max_index == rank[0]: # 颜色和当前的匹配
                    send_cmd(xmlReadCommand(f"plate{COLOR[max_index]}", 0))
                    print("catch from plate: ", COLOR[max_index])
                    cv2.imwrite(f"/home/jetson/GongXun2/app/debug/plate{COLOR[max_index]}.jpg", img)
                    rank.remove(max_index)
                    queue.put(img)
                    # 等待抓放完成
                    time.sleep(6)
                else:
                    cv2.putText(img, "wrong target", (box[0] + 80, box[1]), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 128), 2, cv2.LINE_AA)
                    queue.put(img)
                if len(rank) == 0:
                    print("三个物块抓取完成")
                    queue.put(np.ones((480, 640, 3), np.uint8) * 255)
                    break

    # 等最后一个物块抓完
    time.sleep(2)

    cap.release()
    blank = np.ones((480, 640, 3), np.uint8) * 255
    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"去粗加工区", (384, 200), (0, 0, 0), 65)
    queue.put(img)

    send_dataDMA(xmlReadCommand("complete", 1), 0, 0)
    print("yp", xmlReadCommand("complete", 0))
            

if __name__ == "__main__":
    pass