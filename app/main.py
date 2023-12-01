import cv2
import numpy as np
from multiprocessing import Process, Queue


# 全局变量区  # # # # # # # # # #
camera1_path = "/dev/cameraQRCode"
camera2_path = "/dev/cameraMain"
screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面
screen_qrcode_area = np.ones((120, 1024, 3), np.uint8) * 255 # 信息显示区域
sequence = []
# # # # # # # # # # # # # # # # #


# 状态区 # # # # # # # # # # # # #
stop = False
task_state = 0
response = ""
# # # # # # # # # # # # # # # # #


# 显示图像 和 控制关闭程序 # # # #
def process_display(queue: Queue
                    ):
    global screen, screen_qrcode_area
    screen[0:120, 0:1024] = screen_qrcode_area
    screen[120:600, 0:640] = queue.get()
    cv2.imshow("screen", screen)
    cv2.waitKey(5)

def stopHandler(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True
# # # # # # # # # # # # # # # # #


# 任务逻辑进行进程 # # # # # # # #
def process_task(queue: Queue):
    global camera1_path, camera2_path, \
        sequence
    
    from tasks.Task1QRCode import Task1_QRCode
    Task1_QRCode(camera1_path, screen_qrcode_area, image_queue, sequence)

    task_state = 0
    
# # # # # # # # # # # # # # # # #



if __name__ == "__main__":
    cv2.namedWindow("screen", cv2.WINDOW_NORMAL)
    cv2.setWindowProperty("screen", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.setMouseCallback("screen", stopHandler)

    # 创建实时画面队列
    image_queue = Queue()

    # 创建串口命令处理进程
    processTask = Process(target=process_task, args=(image_queue, ))
    processTask.start() # 任务开启

    # 主进程
    while True:
        process_display(image_queue)
        if stop:
            processTask.terminate()
            break

    print("program end")
