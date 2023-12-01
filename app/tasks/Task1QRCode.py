import cv2
import numpy as np
import multiprocessing


# 任务一
# 扫码
def Task1_QRCode(cameraPath: str, 
                 screen_qrcode_area: np.ndarray, 
                 queue: multiprocessing.Queue, 
                 sequence: list ):
    
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
            cv2.putText(screen_qrcode_area, result, (600, 80), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 0), 2, cv2.LINE_AA)
            break
        queue.put(frame)
    cap.release()


if __name__ == "__main__":
    pass
