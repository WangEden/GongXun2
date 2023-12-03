import numpy as np
import cv2


cap = cv2.VideoCapture("/dev/video0")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))

# 获取默认曝光时间 0
exposure_time = cap.get(cv2.CAP_PROP_EXPOSURE)
print("默认曝光时间:", exposure_time)
# 获取默认曝光度 35
exposure_compensation  = cap.get(cv2.CAP_PROP_EXPOSURE)
print("默认曝光度:", exposure_compensation)

# # 获取曝光时间的范围
# min_exposure = cap.get(cv2.CAP_PROP_MIN_EXPOSURE)
# max_exposure = cap.get(cv2.CAP_PROP_MAX_EXPOSURE)
# print("exposure time range: ", (min_exposure, max_exposure))

# def callback(event):
#     pass

# stop = False
# def stopHandle(e, x, y, f, p):
#     global stop
#     if e == cv2.EVENT_LBUTTONDOWN:
#         stop = True


# cv2.namedWindow("img")
# cv2.createTrackbar("exposure_time", "img", 5, 10, callback)
# cv2.createTrackbar("exposure_compensation", "img", 5, 10, callback)


# while True:
#     exposure_time = cv2.getTrackbarPos("exposure_time", "img", )
#     exposure_compensation = cv2.getTrackbarPos("exposure_compensation", "img", )

#     cap.set(cv2.CAP_PROP_EXPOSURE, exposure_time)
#     cap.set(cv2.CAP_PROP_EXPOSURE, exposure_compensation / 10)

#     ret, frame = cap.read()
#     frame = cv2.GaussianBlur(frame, (3, 3), 0)

#     exposure_time  = cap.get(cv2.CAP_PROP_EXPOSURE)
#     exposure_compensation  = cap.get(cv2.CAP_PROP_EXPOSURE)
#     print("曝光时间:", exposure_time, "曝光度:", exposure_compensation, end="\r")

#     if stop:
#         # 保存相机参数
#         break

#     frame2 = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
#     out = np.hstack([frame, frame2])
#     out = cv2.resize(out, (int(out.shape[1] /  2), int(out.shape[0] /  2)))
#     cv2.imshow("img", out)
#     cv2.setMouseCallback("img", stopHandle)
#     cv2.waitKey(25)
