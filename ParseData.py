import serial
import RTCEventMaster
import threading


class EventError(Exception):  # Ошибка события
    pass


class IMUStickParser(threading.Thread):
    def __init__(self, portname, baudrate):
        threading.Thread.__init__(self)
        self.port = serial.Serial(portname, baudrate)  # Открытие порта
        self.data = []  # распарсенныее данные
        self.Exit = False  # метка выхода из потока
        self.fixMaxMessageLen = 10  # максимальная возможная длина сообщения
        self.eventDict = {  # Словарь событий
            "START": RTCEventMaster.EventBlock("START"),  # событие начала работы
            "STOP": RTCEventMaster.EventBlock("STOP"),  # событие окончания работы
            "PR": RTCEventMaster.EventBlock("PR"),  # события чтения углов
            "ERROR": RTCEventMaster.EventBlock("ERROR")  # событие ошибки чтения углов
        }

    def exit(self):
        self.Exit = True
        self.port.close()

    def connectFun(self, toEvent, fun):  # ф-ия подключения обработчика события по имени события
        event = self.eventDict.get(toEvent)
        if not event:
            raise EventError(toEvent + ": There is no such event")

        def voidFun():  # Все обработчики событий имеют в качестве параметра 1
            # аргумент - список с распарсенными данными
            fun(self.data)

        event.setfun(voidFun)

    def run(self):
        while not self.Exit:  # пока не вышли из потока
            mes = self.readMessage()  # прочитать сообщение
            if mes:  # если оно существует
                self.parseMessage(mes)  # распарсить сообщение
                self.eventDict.get(self.data[0]).push()  # вызвать соответствующий обработчик

    def readMessage(self):
        buf = b''  # временный буффер
        temp = self.port.read()  # читаем побайтово
        while temp != b'<':  # читаем сообщениенаходящееся в скобках <>
            temp = self.port.read()  # и если оно превышает положенный размер возвращаем None
        size = 0
        temp = self.port.read()
        while temp != b'>':
            if (temp == b'<') or (size > self.fixMaxMessageLen):
                return None
            buf += temp
            size += 1
            temp = self.port.read()
        return buf

    def parseMessage(self, mes):
        try:
            listbuf = list(map(bytes, mes.split()))  # разделение сообщения на токены и запись их в список
            if listbuf[0] == b"PR":  # Приходящие данные: <комманда, данные, ...>
                self.data[0] = "PR"  # PR - команда, означающая, что пришли углы
                self.data[1] = float(listbuf[1])  # pitch
                self.data[2] = float(listbuf[2])  # roll
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
