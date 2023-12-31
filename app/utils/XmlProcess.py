from xml.etree import ElementTree
import numpy as np


def xmlReadCommand(tag: str, mode: int):
    paraDomTree = ElementTree.parse("./setting/command.xml")
    _ = paraDomTree.find(tag).text
    if mode == 0:
        return _  # -> str
    elif mode == 1:
        return list(_)  # -> ['a', 'b', 'c', 'd']


def xmlReadItemThreshold(color: str):  # rank: [min:[], max:[]]
    _min, _max = [], []
    _ = 0
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        _ = int(file.read())
    paraDomTree = None
    if _ == 0:
        paraDomTree = ElementTree.parse("./setting/thresholdItemBlack.xml")
    elif _ == 1:
        paraDomTree = ElementTree.parse("./setting/thresholdItemWhite.xml")
    colorNode = paraDomTree.find(f'color[@category="{color}"]')
    floors = colorNode.findall('./*/floor')
    ceilings = colorNode.findall('./*/ceiling')
    for i in range(3):
        _min.append(int(floors[i].text))
        _max.append(int(ceilings[i].text))
    _min = np.array(_min)
    _max = np.array(_max)
    rank = np.array([_min, _max])
    return rank


# def xmlReadItemThresholds():  # rank: [min:[], max:[]]
#     result = []
#     _min, _max = [], []
#     paraDomTree = ElementTree.parse("./setting/thresholdItem.xml")
#     for i in range(3):
#         colorNode = paraDomTree.find(f'color[@category="{color}"]')
#         floors = colorNode.findall('./*/floor')
#         ceilings = colorNode.findall('./*/ceiling')
#         for i in range(3):
#             _min.append(int(floors[i].text))
#             _max.append(int(ceilings[i].text))
#         _min = np.array(_min)
#         _max = np.array(_max)
#         rank = np.array([_min, _max])
#     return rank


def xmlReadRingThreshold(color: str):  # rank: [min:[], max:[]]
    _min, _max = [], []
    _ = 0
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        _ = int(file.read())
    paraDomTree = None
    if _ == 0:
        paraDomTree = ElementTree.parse("./setting/thresholdRingBlack.xml")
    elif _ == 1:
        paraDomTree = ElementTree.parse("./setting/thresholdRingWhite.xml")
    colorNode = paraDomTree.find(f'color[@category="{color}"]')
    floors = colorNode.findall('./*/floor')
    ceilings = colorNode.findall('./*/ceiling')
    for i in range(3):
        _min.append(int(floors[i].text))
        _max.append(int(ceilings[i].text))
    _min = np.array(_min)
    _max = np.array(_max)
    rank = np.array([_min, _max])
    return rank


def xmlReadCapSetting(tag: int) -> float:
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue"
    }
    paraDomTree = ElementTree.parse("./setting/capSetting.xml")
    item_node = paraDomTree.find(para[tag])
    return float(item_node.text)


def xmlReadCapSettings() -> tuple:
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue"
    }
    result = []
    paraDomTree = ElementTree.parse("./setting/capSetting.xml")
    item_node = None
    color = 0
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = int(file.read())
    if color == 0:
        item_node = paraDomTree.find(f'color[@tag="black"]')
    elif color == 1:
        item_node = paraDomTree.find(f'color[@tag="white"]')
    for i in range(4):
        item = item_node.find(para[i])
        result.append(float(item.text))
    return tuple(result)


def xmlReadRateTuple() -> tuple:
    para = {
        0: "rateb", 1: "rateg", 2: "rater"
    }
    result = []
    paraDomTree = ElementTree.parse("./setting/rateTuple.xml")
    item_node = None
    color = 0
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = int(file.read())
    if color == 0:
        item_node = paraDomTree.find(f'color[@tag="black"]')
    elif color == 1:
        item_node = paraDomTree.find(f'color[@tag="white"]')
    for i in range(3):
        item = item_node.find(para[i])
        result.append(float(item.text))
    return tuple(result)


def xmlReadLightCenter():
    with open("/home/jetson/color.txt", "r", encoding="utf-8-sig") as file:
        color = file.read()
        tag = int(color)
        paraDomTree = ElementTree.parse("./setting/lightCenter.xml")
        if tag == 0:
            item_node = paraDomTree.find(f'color[@tag="black"]')
        elif tag == 1:
            item_node = paraDomTree.find(f'color[@tag="white"]')
        x = int(item_node.find("x").text)
        y = int(item_node.find("y").text)
    return tuple([x, y])


if __name__ == "__main__":
    l = []
    print(xmlReadRingThreshold("green"))
