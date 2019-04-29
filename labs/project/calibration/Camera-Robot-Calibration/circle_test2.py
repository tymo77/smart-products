# Untitled - By: pi - Tue Apr 9 2019

import sensor, image, time
import math
from pyb import UART

#Sensor Setup
#################################
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_contrast(0)
sensor.set_brightness(0)
sensor.set_gainceiling(2)
sensor.set_auto_exposure(False,exposure_us = 1000)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 4000)
sensor.set_auto_gain(False, value =100)

# Allocate extra frame-buffer for image-averaging.
fb1 = sensor.alloc_extra_fb(sensor.width(),sensor.height(),sensor.RGB565)

def find_circle(img):

    list_of_circs = img.find_circles(threshold=3500,r_min = 8, r_max = 20)
    #i = 0
    for circ in list_of_circs:
        #ROI is a rectangular subset of the circle for sampling.
        roi = (int(circ.x()-circ.r()/2),int(circ.y()-circ.r()/2),circ.r(),circ.r())
        #draw a rectangle on the internal frame buffer for debugging
        img.draw_rectangle(roi[0],roi[1],roi[2],roi[3])
        img.draw_circle(circ.x(),circ.y(),circ.r())
        print(img.compressed_for_ide(),end="");
    return list_of_circs

def send_circles(list_of_circles,img):
    print('------------------------------')
    num_circles=len(list_of_circles)
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
        print(index,circ.x(),circ.y(),circ.r(),rgb)
        index=index+1



clock = time.clock()
while(True):
    clock.tick()

    fb1.replace(sensor.snapshot().lens_corr(1.7))
    for i in range(10):
        img =fb1.blend(sensor.snapshot().lens_corr(1.7),alpha = 24)
    circle_list=find_circle(img)
    send_circles(circle_list,img)
    time.sleep(1000)


