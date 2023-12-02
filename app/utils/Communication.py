import serial, struct

uart = serial.Serial(  # 声明串口
    port="/dev/ttyTHS1",
    baudrate=115200,
    stopbits=1,
    timeout=1,
)

# 定义数据包，格式为2个帧头+4个字符数据+2个半整型数据+帧尾（11byte）
# 4个字符传输命令名，2个int传输xy方向的偏差
def send_data(cmd: list, i, f):
    a, b, c, d = cmd
    data = struct.pack(
        "<bbbbhh",  # 四个字符作为命令, 两个浮点作为xy偏差
        ord(str(a)),  # 字符1
        ord(str(b)),  # 字符2
        ord(str(c)),  # 字符3
        ord(str(d)),  # 字符4
        int(i),  # 半整型数据1
        int(f),  # 半整型数据2
    )
    uart.write(data)


def send_cmd(cmd: str):
    data = struct.pack(
        "<bbbb",  # 四个字符作为命令, 两个浮点作为xy偏差
        ord(str(cmd[0])),  # 字符1
        ord(str(cmd[1])),  # 字符2
        ord(str(cmd[2])),  # 字符3
        ord(str(cmd[3]))   # 字符4
    )
    uart.write(data)


def recv_data():
    return uart.read(4).decode("utf-8", 'ignore')


if __name__ == "__main__":
    pass
