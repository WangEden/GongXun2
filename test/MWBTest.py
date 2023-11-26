# 手动白平衡
import cv2
import numpy as np
import copy


cap = cv2.VideoCapture("/dev/video0")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))


"""
notes:
1. 图像的色温:
    黑体在受热后, 发出的辐射光颜色应该是由黑-->红-->黄-->白-->蓝的变化规律,
    基于此, 对于RGB图像, 若要提高图像的色温, 应对R和G通道进行增强，B通道进行削弱，反之同理。

待完成：
    （1）理解并移植完美反射算法，并用numpy优化
    （2）
    
2. 颜色识别任务：
    （1）预先在补光和使用完美反射白平衡矫正的情况下，存好色环和物块的颜色HSV阈值（将物块和色环放于场地白色背景下进行录制）
    （2）之后更换场地所在环境，利用预先存好的阈值，搭配实时的白平衡处理进行颜色识别（随机打光）

要做的测试内容：
    （1）使用标准白卡背景进行完美反射白平衡时得到的Ratio值和圆盘白色背景以及色环白色背景（色环背景是白色吗）得到的Ratio值进行对比
    （2）写一个在调试阶段运行的程序，该程序会在进行完整跑圈的时候进行适应场地光线调节改变白平衡的参数（需要将算法中的参数提炼做成JSON或XML）
    （3）梯度法自适应阈值，将梯度处于某一范围的色块区域归于一类

    
"""
def limit(num):
    if num > 255: return 255
    elif num < 0: return 0
    else: return num


def fineTuneColorTemperature():
    # 将图像分块

    # 
    pass



def imageColorTemperatureGain(img: np.ndarray, gain: int):
    img_result = copy.deepcopy(img).astype(np.int16)
    img_result = img_result + gain
    img_result = np.clip(img_result, 0, 255).astype(np.uint8)
    return img_result
        

if __name__ == "__main__":
    # cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        gain_img = imageColorTemperatureGain(img=frame, gain=-100)
        out = np.hstack([frame, gain_img])
        out = cv2.resize(out, (int(out.shape[1] / 2), int(out.shape[0] / 2)))
        cv2.imshow("show", out)
        if cv2.waitKey(24) & 0XFF == ord('q'):
            break