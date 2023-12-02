import cv2
import numpy as np
from multiprocessing import Queue, Array, Lock
from utils.Communication import * 


# 任务一
# 扫码
def Task1_QRCode(cameraPath: str, 
                 queue: Queue, 
                 sequence: list,
                 qr_result: Array, 
                 lock: Lock):
    
    from pyzbar.pyzbar import decode
    # cap = cv2.VideoCapture(camera1_path)
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
            result_bytes = result.encode('utf-8')
            lock.acquire()
            for i in range(len(result_bytes)):
                qr_result[i] = result_bytes[i]
            lock.release()
            break
        queue.put(frame)
    cap.release()
    send_cmd("QROK") # 扫码完成，发送命令
    queue.put(np.ones((480, 640, 3), np.uint8) * 255)
    return result


if __name__ == "__main__":
    pass
