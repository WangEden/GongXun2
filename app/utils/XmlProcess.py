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
    paraDomTree = ElementTree.parse("./setting/thresholdItem.xml")
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
    for i in range(4):
        item_node = paraDomTree.find(para[i])
        result.append(float(item_node.text))
    return tuple(result)


def xmlReadRateTuple() -> tuple:
    para = {
        0: "rateb", 1: "rateg", 2: "rater"
    }
    result = []
    paraDomTree = ElementTree.parse("./setting/rateTuple.xml")
    for i in range(3):
        item_node = paraDomTree.find(para[i])
        result.append(float(item_node.text))
    return tuple(result)


if __name__ == "__main__":
    print(xmlReadCommand("arriveYL", 0))
