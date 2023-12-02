import numpy as np
import cv2

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
    

if __name__ == "__main__":
    pass