import cv2
import numpy as np
import multiprocessing
import serial, time


# # # # # # # # # # # # # # # # #
# 全局变量区
screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面
screen_realtime_picture = np.zeros((480, 640, 3), np.uint8) # 拍到的画面
screen_qrcode_area = np.ones((120, 1024, 3), np.uint8) * 255 # 信息显示区域
screen_blank_area = np.ones((480, 384, 3), np.uint8) * 255 # 空白区域
screen_blank_area2 = np.ones((480, 1024, 3), np.uint8) * 255 #空白区域 2
uart = serial.Serial(  # 声明串口
    port="/dev/ttyTHS1",
    baudrate=115200,
    # bytesize=8,
    # parity=serial.PARITY_NONE,
    stopbits=1,
    timeout=1,
    # dsrdtr=True,1
)
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 状态区
stop = False

# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 显示任务
def process_display(queue):
    global screen, screen_realtime_picture, screen_qrcode_area, screen_blank_area
    screen[0:120, 0:1024] = screen_qrcode_area
    screen[120:600, 0:640] = queue.get()
    cv2.imshow("screen", screen)
    cv2.waitKey(5)
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 串口接收
def process_uart():
    global response
    print("process_uart start")
    time.sleep(5)
    print("process_uart end")
    # response = uart.read(6)
    # while True:
    #     print("response: ")
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 图像处理
def process_task(queue):
    global screen_realtime_picture
    cap = cv2.VideoCapture(0)
    cap.set(3, 640)
    cap.set(4, 480)
    cap.set(6, cv2.VideoWriter.fourcc(*'MJPG')) 
    while True:
        ret, frame = cap.read()
        if ret:
            # 将图像数据放入队列
            queue.put(frame)
            print("img copy")
    # cv2.rectangle(screen_realtime_picture, (50, 50), (100, 100), (255, 0, 0), 2)
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 其他
def stopHandler(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


if __name__ == "__main__":
    cv2.namedWindow("screen", cv2.WINDOW_NORMAL)
    cv2.setWindowProperty("screen", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.setMouseCallback("screen", stopHandler)

    # 创建队列
    image_queue = multiprocessing.Queue()

    # 创建三个子进程
    processUart = multiprocessing.Process(target=process_uart, args=())
    processTask = multiprocessing.Process(target=process_task, args=(image_queue, ))

    processUart.start()
    processTask.start()

    # 主进程
    while True:
        process_display(image_queue)
        if stop:
            processUart.terminate()
            processTask.terminate()
            break
