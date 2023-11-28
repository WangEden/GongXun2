import cv2
import numpy as np


filename = './imgs/afterMWB.jpg'


def callback(event):
    pass


def precondition(img):
    # result = cv2.pyrMeanShiftFiltering(img, 15, 20)
    result = cv2.GaussianBlur(img, (3, 3), 0)
    return result


def Choose_Color():
    global filename
    image0 = cv2.imread(filename, 1)

    # img = cv2.resize(image0, (640, 480))
    # img = cv2.resize(image0, (int(image0.shape[1] / 4), int(image0.shape[0] / 4)))

    img = precondition(image0)

    cv2.imshow("img_pyr", img)
    cv2.waitKey(0)

    img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # img = cv2.erode(img, None, iterations=2)
    # img = image0
    '''
    目标：创建滑动条，把滑动条绑定到opencv窗口
    cv2.createTrackbar()函数，函数的第一个参数时滑动条的名字，第二个参数时滑动条被放置的窗口的名字，第三个参数是滑动条默认值，第四个参数时滑动条的最大值，第五个参数时回调函数，每次滑动都会调用回调函数。
    cv2.getTrackbarPos()，共有2个参数，第一个参数是滑动条名字，第二个时所在窗口偶，返回值是滑动条位置。
    '''

    cv2.imshow("image", img)

    cv2.createTrackbar("H_min", "image", 50, 255, callback)
    cv2.createTrackbar("H_max", "image", 150, 255, callback)

    cv2.createTrackbar("S_min", "image", 0, 255, callback)
    cv2.createTrackbar("S_max", "image", 255, 255, callback)

    cv2.createTrackbar("V_min", "image", 0, 255, callback)
    cv2.createTrackbar("V_max", "image", 255, 255, callback)

    while True:
        H_min = cv2.getTrackbarPos("H_min", "image", )
        S_min = cv2.getTrackbarPos("S_min", "image", )
        V_min = cv2.getTrackbarPos("V_min", "image", )

        H_max = cv2.getTrackbarPos("H_max", "image", )
        S_max = cv2.getTrackbarPos("S_max", "image", )
        V_max = cv2.getTrackbarPos("V_max", "image", )

        lower_hsv = np.array([H_min, S_min, V_min])
        upper_hsv = np.array([H_max, S_max, V_max])

        mask = cv2.inRange(img, lower_hsv, upper_hsv)

        # print("H_min = %d,H_max = %d,S_min = %d,S_max = %d,V_min = %d,V_max = %d"%(H_min,H_max,S_min,S_max,V_min,V_max))

        cv2.imshow("mask", mask)

        if cv2.waitKey(1) & 0XFF == ord('q'):
            break


Choose_Color()
