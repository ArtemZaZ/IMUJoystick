import serial
import RTCEventMaster
import threading
import socket
import time


class EventError(Exception):  # Ошибка события
    pass


class IMUStickParser(threading.Thread):
    def __init__(self, address=None, portnumber=None, portname=None, baudrate=None):
        threading.Thread.__init__(self)
        self.IO = None
        if portname and baudrate:
            self.port = serial.Serial(portname, baudrate)  # Открытие порта
            self.IO = "COM"
        elif address and portnumber:
            self.sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
            self.sock.connect((address, portnumber))
            self.IO = "BLU"
        else:
            raise IOError("Не указан источник")
        self.data = ['', '', '']  # распарсенныее данные
        self.Exit = False  # метка выхода из потока
        self.fixMaxMessageLen = 14  # максимальная возможная длина сообщения
        self.eventDict = {  # Словарь событий
            "START": RTCEventMaster.EventBlock("START"),  # событие начала работы
            "STOP": RTCEventMaster.EventBlock("STOP"),  # событие окончания работы
            "BUT": RTCEventMaster.EventBlock("BUT"),    # событие нажатия/отжатия кнопки
            "PRD": RTCEventMaster.EventBlock("PRD"),  # события чтения углов
            "ERROR": RTCEventMaster.EventBlock("ERROR")  # событие ошибки чтения углов
        }
        self.eventMaster = RTCEventMaster.EventMaster()
        self.eventMaster.append(self.eventDict.get("START"))
        self.eventMaster.append(self.eventDict.get("STOP"))
        self.eventMaster.append(self.eventDict.get("ERROR"))
        self.eventMaster.append(self.eventDict.get("PRD"))
        self.eventMaster.append(self.eventDict.get("BUT"))
        self.eventMaster.start()

    def exit(self):
        self.Exit = True
        self.eventMaster.exit()
        if self.IO == "COM":
            self.port.close()
        if self.IO == "BLU":
            self.sock.close()

    def connectFun(self, toEvent, fun):  # ф-ия подключения обработчика события по имени события
        event = self.eventDict.get(toEvent)
        if not event:
            raise EventError(toEvent + ": There is no such event")

        def voidFun():  # Все обработчики событий имеют в качестве параметра 1
            # аргумент - список с распарсенными данными
            fun(self.data[:])

        event.setfun(voidFun)

    def run(self):
        while not self.Exit:  # пока не вышли из потока
            mes = self.readMessage()  # прочитать сообщение
            if mes:  # если оно существует
                self.parseMessage(mes)  # распарсить сообщение
                self.eventDict.get(self.data[0]).push()  # вызвать соответствующий обработчик

    def readByte(self):
        if self.IO == "COM":
            return self.port.read()
        if self.IO == "BLU":
            return self.sock.recv(1)

    def readMessage(self):
        buf = b''  # временный буффер
        temp = self.readByte()  # читаем побайтово
        while temp != b'<':  # читаем сообщениенаходящееся в скобках <>
            temp = self.readByte()  # и если оно превышает положенный размер возвращаем None
        size = 0
        temp = self.readByte()
        while temp != b'>':
            if (temp == b'<') or (size > self.fixMaxMessageLen):
                return None
            buf += temp
            size += 1
            temp = self.readByte()
        return buf

    def write(self, byteStr):
        if self.IO == "COM":
            self.port.write(byteStr)
        elif self.IO == "BLU":
            self.sock.send(byteStr)

    def parseMessage(self, mes):
        try:
            listbuf = list(map(bytes, mes.split()))  # разделение сообщения на токены и запись их в список
            if listbuf[0] == b"PRD":  # Приходящие данные: <комманда, данные, ...>
                self.data[0] = "PRD"  # PR - команда, означающая, что пришли углы
                self.data[1] = float(int(listbuf[1], 16) >> 8)/256*360  # pitch
                self.data[2] = float(int(listbuf[1], 16) & 0xFF)/256*360  # roll
                return

            if listbuf[0] == b"BUT":
                self.data[0] = "BUT"
                self.data[1] = int(listbuf[1], 16) >> 8    # номер кнопки
                self.data[2] = int(listbuf[1], 16) & 0xFF  # положение
                return

            if listbuf[0] == b"START":
                self.data[0] = "START"
                return

            if listbuf[0] == b"STOP":
                self.data[0] = "STOP"
                return

            self.data[0] = "ERROR"

        except:
            self.data[0] = "ERROR"
