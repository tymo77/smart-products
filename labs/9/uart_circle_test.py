# Untitled - By: pi - Tue Apr 9 2019

import sensor, image, time
import math
from pyb import UART

#Sensor Setup
#################################
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_contrast(3)
sensor.set_brightness(0)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 4000)
sensor.set_auto_gain(False, value =100)

uart = UART(1, 9600, timeout_char = 1000)


def get_circles(img):
    list_of_circs = img.find_circles(threshold=3500,r_min = 10, r_max = 30)

    i = 0
    for circ in list_of_circs:
        #ROI is a rectangular subset of the circle for sampling.
        roi = (int(circ.x()-circ.r()/2),int(circ.y()-circ.r()/2),circ.r(),circ.r());
        #draw a rectangle on the internal frame buffer for debugging
        img.draw_circle(circ.x(),circ.y(),circ.r())
        img.draw_rectangle(roi[0],roi[1],roi[2],roi[3])
    return list_of_circs

def send_circles(uart, list_of_circles,img):
    i = 0
    for circ in list_of_circles:
        #ROI is a rectangular subset of the circle for sampling.
        roi = (int(circ.x()-circ.r()/2),int(circ.y()-circ.r()/2),circ.r(),circ.r());

        #get roi color info
        img2 = img.copy(roi = roi)
        hist = img2.get_histogram()
        stat = hist.get_statistics()
        l_mean = stat.l_mean()
        a_mean = stat.a_mean()
        b_mean = stat.b_mean()
        rgb = image.lab_to_rgb((l_mean,a_mean,b_mean))

        #send
        uart.writechar(i)
        print(i)
        uart.writechar(circ.x())
        print(circ.x())
        uart.writechar(circ.y())
        print(circ.y())
        uart.writechar(circ.r())
        print(circ.r())
        uart.writechar(rgb[0])
        uart.writechar(rgb[1])
        uart.writechar(rgb[2])
        print(rgb)

        i = i +1



clock = time.clock()
while(True):
    clock.tick()
    msg = None
    while(uart.any() !=0):
        msg = uart.readchar()
        print(msg)
    if (msg == 1):
        uart.writechar(2)
    elif(msg == 5):
        img = sensor.snapshot().lens_corr(1.7)
        list_of_circles = get_circles(img)
        uart.writechar(len(list_of_circles))
        send_circles(uart,list_of_circles,img)
