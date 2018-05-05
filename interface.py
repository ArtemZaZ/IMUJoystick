import gi
import ParseData
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk


class Interface:
    def __init__(self):
        self.builder = Gtk.Builder()
        self.builder.add_from_file("interface.glade")
        self.window = self.builder.get_object("window1")
        self.startButton = self.builder.get_object("StartButton")
        self.stopButton = self.builder.get_object("StopButton")
        self.vibrateButton = self.builder.get_object("VibrateButton")
        self.window.connect("delete-event", self.delete_event)
        self.startButton.connect("clicked", self.sendStart)
        self.stopButton.connect("clicked", self.sendStop)
        self.vibrateButton.connect("clicked", self.sendVibrate)
        #self.ISP = ParseData.IMUStickParser(portname="/dev/ttyUSB0", baudrate=9600)
        self.ISP = ParseData.IMUStickParser(address="00:21:13:04:D9:63", portnumber=1)

    def delete_event(self, widget, event, data=None):
        Gtk.main_quit()
        self.ISP.exit()

    def start(self):
        self.ISP.start()
        self.window.show_all()
        Gtk.main()

    def sendStart(self, w):
        self.ISP.write(b"<START 0000>")

    def sendStop(self, w):
        self.ISP.write(b"<STOP 0000>")

    def sendVibrate(self, w):
        self.ISP.write(b"<VIBRATE 0003>")