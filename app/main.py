import cv2
import numpy as np
from multiprocessing import Process, Queue, Array, Lock


# 全局变量区  # # # # # # # # # #
camera1_path = "/dev/cameraQRCode"
camera2_path = "/dev/cameraMain"
screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面
screen_qrcode_area = np.ones((120, 1024, 3), np.uint8) * 255 # 信息显示区域
sequence = []
qr_result = Array('c', b"       ") # 创建进程共享变量，存储二维码识别结果
# # # # # # # # # # # # # # # # #


# 状态区 # # # # # # # # # # # # #
stop = False
task_state = 0
response = ""
# # # # # # # # # # # # # # # # #


# 显示图像 和 控制关闭程序 # # # #
def process_display(queue: Queue
                    ):
    global screen, screen_qrcode_area, qr_result

    screen[120:600, 0:640] = queue.get()
    qrcode = qr_result.raw.decode('utf-8')
    cv2.putText(screen, qrcode, (512 - 7 * 25, 50 + 25), cv2.FONT_HERSHEY_SIMPLEX, 2.5, (0, 0, 0), 8, cv2.LINE_AA)
    cv2.imshow("screen", screen)
    cv2.waitKey(5)

def stopHandler(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True
# # # # # # # # # # # # # # # # #


# 任务逻辑进行进程 # # # # # # # #
def process_task(queue: Queue, lock: Lock):
    global camera1_path, camera2_path, qr_result, \
        sequence
     
    # 执行任务一 扫码
    from tasks.Task1QRCode import Task1_QRCode
    Task1_QRCode(camera1_path, image_queue, sequence, qr_result, lock)
    
    # 执行任务二 从圆盘取物块
    from tasks.Task2GetFromPlate import Task2_GetFromPlate
    Task2_GetFromPlate(camera2_path, image_queue, sequence)

    # 执行任务三 在粗加工区放取物块


    # 执行任务四 在暂存区放物块


    # 执行任务六 从圆盘取第二轮物块


    # 执行任务七 在粗加工区放取第二轮物块


    # 执行任务八 在暂存区码垛放置物块


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
    processTask = Process(target=process_task, args=(image_queue, lock))
    processTask.start() # 任务开启

    # 主进程
    while True:
        process_display(image_queue)
        if stop:
            processTask.terminate()
            break
    
    print("program end")
