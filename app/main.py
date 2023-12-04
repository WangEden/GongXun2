import cv2
import numpy as np
from multiprocessing import Process, Queue, Array, Lock, Value


# 全局变量区  # # # # # # # # # #
camera1_path = "/dev/cameraQRCode"
camera2_path = "/dev/cameraMain"
screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面
screen_qrcode_area = np.ones((120, 1024, 3), np.uint8) * 255 # 信息显示区域
sequence = []
qr_result = Array('c', b"       ") # 创建进程共享变量，存储二维码识别结果
continue_ = Value('b', False) # 创建进程共享变量，存储二维码识别结果
# # # # # # # # # # # # # # # # #


# 状态区 # # # # # # # # # # # # #
stop = False
task_state = 0
response = ""
KeyCenter = (832, 240)
buttonLen = 50
# # # # # # # # # # # # # # # # #


# 显示图像 和 控制关闭程序 # # # #
def process_display(queue: Queue
                    ):
    global screen, screen_qrcode_area, qr_result

    screen[120:600, 0:640] = queue.get()
    cv2.rectangle(screen, (int(KeyCenter[0] + buttonLen / 2), int(KeyCenter[1] - buttonLen / 2)),
               (int(KeyCenter[0] + buttonLen * 7 / 2), int(KeyCenter[1] + buttonLen / 2)), (0, 0, 0), -1)
    cv2.putText(screen, "continue", (int(KeyCenter[0] + buttonLen / 2), int(KeyCenter[1] + buttonLen / 2)), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
    
    qrcode = qr_result.raw.decode('utf-8')
    cv2.putText(screen, qrcode, (512 - 7 * 25, 50 + 25), cv2.FONT_HERSHEY_SIMPLEX, 2.5, (0, 0, 0), 8, cv2.LINE_AA)
    cv2.imshow("screen", screen)
    cv2.waitKey(1)

def stopHandler(e, x, y, f, p):
    global stop, KeyCenter, buttonLen, continue_
    if e == cv2.EVENT_LBUTTONDOWN:
        if 0 < x < 640 and 120 < y < 600:
            stop = True
        elif int(KeyCenter[0] + buttonLen / 2) < x < int(KeyCenter[0] + buttonLen * 5 / 2) and \
            int(KeyCenter[1] - buttonLen / 2) < y < int(KeyCenter[1] + buttonLen / 2):
                continue_.value = True
        
# # # # # # # # # # # # # # # # #


# 任务逻辑进行进程 # # # # # # # #
def process_task(queue: Queue, lock: Lock, continue_: Value):
    global camera1_path, camera2_path, qr_result, \
        sequence

    # # 执行任务一 扫码
    # from tasks.Task1QRCode import Task1_QRCode
    # Task1_QRCode(camera1_path, image_queue, sequence, qr_result, lock)
    # del Task1_QRCode
    
    # # 执行任务二 从圆盘取物块
    # from tasks.Task2GetFromPlate import Task2_GetFromPlate
    # Task2_GetFromPlate(camera2_path, image_queue, sequence, 1)
    # del Task2_GetFromPlate

    # 执行任务三 在粗加工区放取物块
    from tasks.Task3PutOnRing3 import Task3_PutOnRing3
    Task3_PutOnRing3(camera2_path, image_queue, sequence, 1)
    del Task3_PutOnRing3
    
    # 执行任务四 在暂存区放物块
    # from tasks.Task4PutOnRing6 import Task4_PutOnRing6
    # Task4_PutOnRing6(camera2_path, image_queue, sequence)
    # del Task4_PutOnRing4

    # # 执行任务六 从圆盘取第二轮物块
    # from tasks.Task2GetFromPlate import Task2_GetFromPlate
    # Task2_GetFromPlate(camera2_path, image_queue, sequence)
    # del Task2_GetFromPlate

    # # 执行任务七 在粗加工区放取第二轮物块
    # from tasks.Task3PutOnRing3 import Task3_PutOnRing3
    # Task3_PutOnRing3(camera2_path, image_queue, sequence)
    # del Task3_PutOnRing3

    # 执行任务八 在暂存区码垛放置物块

    # 结束
    blank = np.ones((480, 640, 3), np.uint8) * 255
    from utils.VisionUtils import cv2AddChineseText
    img = cv2AddChineseText(blank, f"单击屏幕退出", (364, 200), (0, 0, 0), 45)
    while True:
        queue.put(img)


# # # # # # # # # # # # # # # # #


if __name__ == "__main__":
    cv2.namedWindow("screen", cv2.WINDOW_NORMAL)
    cv2.setWindowProperty("screen", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.setMouseCallback("screen", stopHandler)

    # 创建实时画面队列
    image_queue = Queue()
    # 创建锁
    lock = Lock()

    # 创建任务处理进程
    processTask = Process(target=process_task, args=(image_queue, lock, continue_))
    processTask.start() # 任务开启

    # 主进程
    while True:
        process_display(image_queue)
        if stop:
            processTask.terminate()
            break
    
    print("program end")
