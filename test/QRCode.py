from pyzbar.pyzbar import decode
import numpy as np
import cv2

save_path = '../static/imgs/qrcode_result/'
camera_path = '/dev/cameraTop'

cap = cv2.VideoCapture("/dev/video0")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

try:
    while True:
        # 读取摄像头传回的图片
        ret, frame = cap.read()
        img_gray = None
        if ret:
            img_gray = frame.copy()
            img_gray = cv2.cvtColor(img_gray, cv2.COLOR_BGR2GRAY)
        else:
            print("cannot read image")
            break

        cv2.imshow("img", frame)
        cv2.waitKey(1)

        result = decode(img_gray)
        result_string_list = []
        if len(result) == 0:
            print("qrcode not found")
            continue
        else:
            # print(len(result))
            # print(result)
            for item in result:
                result_string_list.append(item.data.decode("utf-8"))
            print("success read qrcode")
            string = result_string_list[0]
            print("qrcode result: "+string)
            break 
except:
    print("error")
