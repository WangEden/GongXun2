import cv2
import numpy as np


# # # # # # # # # # # # # # # # #
# 常量区
camera1_path = "/dev/cameraQRCode"
camera2_path = "/dev/cameraMain"
screen = np.ones((600, 1024, 3), np.uint8) * 255 # 全屏显示的画面
screen_realtime_picture = np.zeros((480, 640, 3), np.uint8) # 拍到的画面
screen_qrcode_area = np.ones((120, 1024, 3), np.uint8) * 255 # 信息显示区域
screen_blank_area = np.ones((480, 384, 3), np.uint8) * 255 # 空白区域
sequence = []

# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 状态区
stop = False
def stopHandler(e, x, y, f, p):
    global stop
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True

# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 显示图像
def display(frame: np.ndarray):
    global screen, screen_realtime_picture, screen_qrcode_area, screen_blank_area
    
    temp = np.hstack([frame, screen_blank_area])
    screen = np.vstack([screen_qrcode_area, temp])
    cv2.imshow("screen", screen)
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 扫码 1
def Task1_QRCode():
    global camera1_path, screen_qrcode_area, sequence
    
    from pyzbar.pyzbar import decode
    cap = cv2.VideoCapture(camera1_path)
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        if not ret:
            print("no qrcode camera.")
            return False
        img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        qrcode_result = decode(img_gray)
        qrcode_result_list = []
        if len(qrcode_result) > 0:
            for item in qrcode_result:
                qrcode_result_list.append(item.data.decode("utf-8"))
            print("success read qrcode")
            result = qrcode_result_list[0]
            print("qrcode result: ", result)
            sequence.clear()
            number = result.split("+")
            for i in number:  # number: ['123', '321']
                l = list(i)  # l : ['1', '2', '3']
                for j in l:
                    sequence.append(int(j))  # queue: [1, 2, 3, 3, 2, 1]
            cv2.putText(screen_qrcode_area, result, (600, 80), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 0), 2, cv2.LINE_AA)
            break
# # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # #
# 取物块


if __name__ == "__main__":
    cv2.namedWindow("screen", cv2.WINDOW_NORMAL)
    cv2.setWindowProperty("screen", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.setMouseCallback("screen", stopHandler)

    Task1_QRCode()
