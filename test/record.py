import numpy as np
import cv2

cap = cv2.VideoCapture(0)
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi',fourcc, 30.0, (640,480))
stop = False
def stopHandler(e, x, y, f, p):
    global stop, LightCenterX, LightCenterY
    if e == cv2.EVENT_LBUTTONDOWN:
        stop = True

while(cap.isOpened()):
    ret, frame = cap.read()
    if ret==True:
        frame = cv2.flip(frame,1)
        out.write(frame) # 保存视频
        cv2.imshow('frame', frame)
        cv2.setMouseCallback("frame", stopHandler)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        if stop:
            break
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
