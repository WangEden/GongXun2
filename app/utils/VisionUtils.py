import numpy as np
import cv2
from PIL import Image, ImageDraw, ImageFont


# 判断前后两个画面是否有物体在运动
def moving_detect(frame1, frame2) -> bool:
    img1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY)
    img2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2GRAY)
    grey_diff = cv2.absdiff(img1, img2)  # 计算两幅图的像素差
    change = np.average(grey_diff)

    if change > 3:  # 当两幅图的差异大于给定的值后，认为画面有物体在动
        return True
    else:
        return False
    

def mask_find_b_boxs(_mask):
    retval, labels, stats, centroids = cv2.connectedComponentsWithStats(
        _mask, connectivity=8
    )
    stats = stats[stats[:, 4].argsort()]
    return stats[:-1]


def select_item(b_box):
    if len(b_box) == 0:
        return [0, 0, 0, 0, 0]
    if len(b_box) == 1:
        return b_box[0]


def get_the_most_credible_box(b_box):
    if len(b_box) == 0:
        return [0, 0, 0, 0, 0]
    if len(b_box) == 1:
        return b_box[0]
    b_box = sorted(b_box, key=lambda box: box[4], reverse=True)
    return b_box[0]


def cv2AddChineseText(img, text, position, textColor=(0, 255, 0), textSize=30):
    if (isinstance(img, np.ndarray)):  # 判断是否OpenCV图片类型
        img = Image.fromarray(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    # 创建一个可以在给定图像上绘图的对象
    draw = ImageDraw.Draw(img)
    # 字体的格式
    fontStyle = ImageFont.truetype(
        "/home/jetson/GongXun2/app/utils/simsun.ttc", textSize, encoding="utf-8")
    # 绘制文本
    draw.text(position, text, textColor, font=fontStyle)
    # 转换回OpenCV格式
    return cv2.cvtColor(np.asarray(img), cv2.COLOR_RGB2BGR)
    

# 获取色环的圆心像素坐标
def getCircleCenter(img:np.ndarray):
    result = []
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_binary = cv2.adaptiveThreshold(~img_gray, 255,
                                cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 15, -5)
    img_binary = cv2.medianBlur(img_binary, 3)
    circles = cv2.HoughCircles(img_binary, cv2.HOUGH_GRADIENT, 1, 100)
    if circles is not None and len(circles) != 0:
        circles = np.round(circles[0, :]).astype('int')
        for (x, y, r) in circles:
            result.append(tuple([x, y, r]))
    return result


def getTheCenterCircle(circles: list):
    pass



if __name__ == "__main__":
    pass