from xml.etree import ElementTree
import numpy as np


def xmlReadCommand(tag: str, mode: int):
    paraDomTree = ElementTree.parse("./parameter.xml")
    messageNode = paraDomTree.find("message")
    _ = messageNode.find(tag).text
    if mode == 0:
        return _  # -> str
    elif mode == 1:
        return list(_)  # -> ['a', 'b', 'c', 'd']


def xmlReadThreshold(tag: str, color: str, rank: list):  # rank: [min:[], max:[]]
    _min, _max = [], []
    paraDomTree = ElementTree.parse("./parameter.xml")
    threshold_node = paraDomTree.find(f'threshold[@tag="{tag}"]')
    colorNode = threshold_node.find(f'color[@category="{color}"]')
    floors = colorNode.findall('./*/floor')
    ceilings = colorNode.findall('./*/ceiling')
    for i in range(3):
        _min.append(int(floors[i].text))
        _max.append(int(ceilings[i].text))
    _min = np.array(_min)
    _max = np.array(_max)
    rank.append(_min)
    rank.append(_max)


def xmlReadCapSetting(tag: int) -> float:
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue", 
        4: "rateb", 5: "rateg", 6: "rater"
    }
    paraDomTree = ElementTree.parse("./parameter.xml")
    capSetting_node = paraDomTree.find('capSetting')
    item_node = capSetting_node.find(para[tag])
    return float(item_node.text)


def xmlReadCapSettings() -> list:
    para = {
        0: "brightness", 1: "contrast", 2: "saturation", 3: "hue", 
        4: "rateb", 5: "rateg", 6: "rater"
    }
    result = []
    paraDomTree = ElementTree.parse("./parameter.xml")
    capSetting_node = paraDomTree.find('capSetting')
    for i in range(7):
        item_node = capSetting_node.find(para[i])
        result.append(float(item_node.text))
    return result


if __name__ == "__main__":
    print(xmlReadCapSetting(4), type(xmlReadCapSetting(4)))
