# Untitled - By: pi - Tue Apr 9 2019

import sensor, image, time
import math
from pyb import UART

#Sensor Setup
#################################
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_contrast(3)
sensor.set_brightness(-3)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 4000)
sensor.set_auto_gain(False, value =100)

uart=UART(1,9600,timeout_char=1000)

RQST=0x01
ACK=0x02
GET_CIRCLE=0x05
DATA_START=0x03
DATA_END=0x04

def find_circle(img):

    list_of_circs = img.find_circles(threshold=3500,r_min = 10, r_max = 30)
    #i = 0
    for circ in list_of_circs:
        #ROI is a rectangular subset of the circle for sampling.
        roi = (int(circ.x()-circ.r()/2),int(circ.y()-circ.r()/2),circ.r(),circ.r())
        #draw a rectangle on the internal frame buffer for debugging
        img.draw_rectangle(roi[0],roi[1],roi[2],roi[3])
    return list_of_circs

def send_circles(uart,list_of_circles,img):
    num_circles=len(list_of_circles)
    uart.writechar(num_circles)
    index=0
    for circ in list_of_circles:
        roi = (int(circ.x()-circ.r()/2),int(circ.y()-circ.r()/2),circ.r(),circ.r())
        img_circ = img.copy(roi = roi)
        hist = img_circ.get_histogram()
        stat = hist.get_statistics()
        l_mean = stat.l_mean()
        a_mean = stat.a_mean()
        b_mean = stat.b_mean()
        rgb = image.lab_to_rgb((l_mean,a_mean,b_mean))
        uart.writechar(index)
        uart.writechar(circ.x())
        uart.writechar(circ.y())
        uart.writechar(circ.r())
        uart.writechar(rgb[0])
        uart.writechar(rgb[1])
        uart.writechar(rgb[2])
        print(index)
        print(circ.x())
        print(circ.y())
        print(circ.r())
        print(rgb[0])
        print(rgb[1])
        print(rgb[2])
        index=index+1


clock = time.clock()
while(True):
    clock.tick()
    while(uart.any() == 0):
        continue
    #print(20)
    while(uart.any() !=0):
        if uart.readchar()!=RQST:
            continue
        else:
            uart.writechar(ACK)
            break
    while(uart.any() == 0):
            continue
    while(uart.any() !=0):
        if uart.readchar()==GET_CIRCLE:
            img = sensor.snapshot().lens_corr(1.7)
            while(True):
                circle_list=find_circle(img)
                if(len(circle_list)>0):
                    break
                img = sensor.snapshot().lens_corr(1.7)
            send_circles(uart,circle_list,img)
            break
        else:
            print("wrong command!")
            continue
    time.sleep(1000)


