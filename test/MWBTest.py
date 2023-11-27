# 手动白平衡
import cv2
import numpy as np
import copy


cap = cv2.VideoCapture("/dev/video0")
cap.set(3, 640)
cap.set(4, 480)
cap.set(6, cv2.VideoWriter.fourcc(*'MJPG'))
# 获取默认亮度值
brightness = cap.get(cv2.CAP_PROP_BRIGHTNESS)
print("默认亮度值:", brightness)
# 获取默认对比度值
contrast = cap.get(cv2.CAP_PROP_CONTRAST)
print("默认对比度:", contrast)
# 获取默认饱和度值
saturation = cap.get(cv2.CAP_PROP_SATURATION)
print("默认饱和度:", saturation)
# 获取默认色调值
hue = cap.get(cv2.CAP_PROP_HUE)
print("默认色调值:", hue)

clickCnt = 2
mwbRect = []


"""
notes:
1. 图像的色温:
    黑体在受热后, 发出的辐射光颜色应该是由黑-->红-->黄-->白-->蓝的变化规律,
    基于此, 对于RGB图像, 若要提高图像的色温, 应对R和G通道进行增强，B通道进行削弱，反之同理。

待完成：
    （1）理解并移植完美反射算法，并用numpy优化 【OK】
    （2）因为目前定位准确，所以改善为区域白平衡（即以一定区域作为参考位置进行矫正）
    
2. 颜色识别任务：
    （1）预先在补光和使用完美反射白平衡矫正的情况下，存好色环和物块的颜色HSV阈值（将物块和色环放于场地白色背景下进行录制）
    （2）之后更换场地所在环境，利用预先存好的阈值，搭配实时的白平衡处理进行颜色识别（随机打光）

要做的测试内容：
    （1）使用标准白卡背景进行完美反射白平衡时得到的Ratio值和圆盘白色背景以及色环白色背景（色环背景是白色吗）得到的Ratio值进行对比
    （2）写一个在调试阶段运行的程序，该程序会在进行完整跑圈的时候进行适应场地光线调节改变白平衡的参数（需要将算法中的参数提炼做成JSON或XML）
    （3）梯度法自适应阈值，将梯度处于某一范围的色块区域归于一类

    
"""
def fineTuneColorTemperature():
    # 将图像分块

    # 
    pass


def imageColorTemperatureGain(img: np.ndarray, gain: int):
    img_result = copy.deepcopy(img).astype(np.int16)
    img_result = img_result + gain
    img_result = np.clip(img_result, 0, 255).astype(np.uint8)
    return img_result


def perfectReflection(img: np.ndarray): # 谨记要使用numpy优化
    use1 = copy.deepcopy(img).astype(np.int64)
    result = copy.deepcopy(img).astype(np.int64)
    # 创建一个单通道矩阵存储R，G，B三通道像素值的和
    sum_matrix = np.sum(use1, axis=2)  # 按z轴方向求和
    # 获取白卡区域的三通道像素值和作为阈值（手动标注）--> MaxVal
    # 假设白卡区域为左上角(lx0, ly0, w160, h120)
    MaxVal = np.max(use1[0:120, 0:160, :])
    Threshold = np.max(sum_matrix[0:120, 0:160])
    # print("MaxVal", MaxVal)
    # print("Threshold", Threshold)
    # 计算三通道之和超过阈值的像素点的各通道像素值的平均值 --> AvgB, AvgG, AvgB
    # 取出大于零的部分
    temp_img = sum_matrix - Threshold
    # temp_img = np.clip(temp_img, 0, 766)
    temp_mask = np.clip(temp_img, 0, 1)
    b, g, r = cv2.split(use1)
    b = b * temp_mask
    # print("AvgB", b.dtype)
    # cv2.imshow("b", b.astype(np.uint8))
    size = img.shape[0] * img.shape[1]
    bn0 = np.count_nonzero(b == 0)
    AvgB = np.sum(b) / (size - bn0)
    g = g * temp_mask
    gn0 = np.count_nonzero(g == 0)
    AvgG = np.sum(g) / (size - gn0)
    r = r * temp_mask
    rn0 = np.count_nonzero(r == 0)
    AvgR = np.sum(r) / (size - rn0)
    # 对每个像素点进行均值化处理 --> val * MaxVal / AvgX
    rb, rg, rr = cv2.split(result)
    rb = rb * MaxVal / AvgB
    rg = rg * MaxVal / AvgG
    rr = rr * MaxVal / AvgR
    result = np.stack((rb, rg, rr), axis=2).astype(np.uint8)
    print("RateB:", MaxVal / AvgB, "RateG:", MaxVal / AvgG, "RateR:", MaxVal / AvgR)
    return result


def setPoinit(e, x, y, f, p):
    global clickCnt, mwbRect
    if e == cv2.EVENT_LBUTTONDOWN:
        clickCnt -= 1
        print((x, y))
        mwbRect.append((x, y))


def getRateBGR():
    global clickCnt, cap, mwbRect
    while True:
        ret, frame = cap.read()
        cv2.imshow("getRate", frame)
        cv2.setMouseCallback("getRate", setPoinit)
        cv2.waitKey(25)

        if clickCnt == 0:
            use1 = copy.deepcopy(frame).astype(np.int64)
            sum_matrix = np.sum(use1, axis=2)  # 按z轴方向求和
            print(mwbRect)
            MaxVal = np.max(use1[mwbRect[0][1]:mwbRect[1][1], mwbRect[0][0]:mwbRect[1][0], :])
            print("MaxVal", MaxVal)
            Threshold = np.max(sum_matrix[mwbRect[0][1]:mwbRect[1][1], mwbRect[0][0]:mwbRect[1][0]])
            temp_img = sum_matrix - Threshold
            temp_mask = np.clip(temp_img, 0, 1)
            b, g, r = cv2.split(use1)
            b = b * temp_mask
            size = frame.shape[0] * frame.shape[1]
            bn0 = np.count_nonzero(b == 0)
            AvgB = np.sum(b) / (size - bn0)
            g = g * temp_mask
            gn0 = np.count_nonzero(g == 0)
            AvgG = np.sum(g) / (size - gn0)
            r = r * temp_mask
            rn0 = np.count_nonzero(r == 0)
            AvgR = np.sum(r) / (size - rn0)
            rateB = MaxVal / AvgB
            rateG = MaxVal / AvgG
            rateR = MaxVal / AvgR
            if 0.6 < rateB < 1.4 and 0.6 < rateG < 1.4 and 0.6 < rateR < 1.4:
                cv2.destroyAllWindows()
                return (rateB, rateG, rateR)
            else:
                mwbRect.clear()
                clickCnt = 2
            print(rateB, rateG, rateR)
        

def useRateMWB(img: np.ndarray, rateTuple: tuple):
    rb, rg, rr = cv2.split(img.astype(np.int64))
    rb = rb * rateTuple[0]
    rg = rg * rateTuple[1]
    rr = rr * rateTuple[2]
    result = np.stack((rb, rg, rr), axis=2)
    result = np.clip(result, 0, 255).astype(np.uint8)
    return result


if __name__ == "__main__":
    # cap = cv2.VideoCapture(0)
    rateTuple = getRateBGR()
    print(rateTuple)
    while True:
        ret, frame = cap.read()
        gain_img = useRateMWB(frame, rateTuple)

        out = np.hstack([frame, gain_img])
        out = cv2.resize(out, (int(out.shape[1] / 3 * 2), int(out.shape[0] / 3 * 2)))
        cv2.imshow("show", out)
        if cv2.waitKey(24) & 0XFF == ord('q'):
            break