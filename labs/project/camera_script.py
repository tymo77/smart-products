#Circle detect code for camera
import sensor
import image
import time
import math
from pyb import UART

#Sensor Setup
#################################
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_contrast(0)
sensor.set_brightness(0)
sensor.set_gainceiling(4)
sensor.set_auto_exposure(False,exposure_us = 1000)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 4000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

threshold_index = 0 #0_red,1_green, 2_blue
#threshold=[(30,100,15,127,15,127),
#                   (30,100,-64,-8,-32,32),
#                  (0,30,0,64,-128,0)]#original version
#Calibrated threshold
threshold = [(35,60,40,80,20,60),#red
             (50,90,-64,-20,-10,20),#green
             (30,90,-20,40,-80,0)]#blue
roi = [(5,0,150,100),(59,17,30,30),(50,28,46,43)]#ROI for start_0,check1_1,check2_2,TBD

# Allocate extra frame-buffer for image-averaging.
fb1 = sensor.alloc_extra_fb(sensor.width(),sensor.height(),sensor.RGB565)

uart = UART(1,9600,timeout_char=1000)
RQST = 0x01
ACK = 0x02
#Command order:
#COLOR<<min_radius<<max_radius<<METHOD<<POSITION(start,check1,check2)
RED = 0x03
GREEN = 0x04
METHOD_BLOB = 0x05
METHOD_CIRC = 0x06
POS_START = 0x07
POS_CHECK1 = 0x08
POS_CHECK2 = 0x09

def find_circs(img,desired_color,rmin,rmax,ROI):
    list_of_circs = img.find_circles(ROI,threshold=1000,r_min = rmin, r_max = rmax)
    center_coord = []
    for circ in list_of_circs:
        roi = (int(circ.x() - circ.r() / 2),int(circ.y() - circ.r() / 2),circ.r(),circ.r())
        img_circ = img.copy(roi = roi)
        hist = img_circ.get_histogram()
        stat = hist.get_statistics()
        l_mean = stat.l_mean()
        a_mean = stat.a_mean()
        b_mean = stat.b_mean()
        rgb = image.lab_to_rgb((l_mean,a_mean,b_mean))
        #if rgb[desired_color] < 200:
        #    continue
        center_coord.append([circ.x(),circ.y()])
        img.draw_circle(circ.x(),circ.y(),circ.r())
        img.draw_rectangle(int(ROI[0]),int(ROI[1]),int(ROI[2]),int(ROI[3]))
        print(img.compressed_for_ide(),end="")
    return center_coord

def find_blobs(img,desired_color,ROI):
    blob_list = img.find_blobs([threshold[desired_color]],roi=ROI,pixel_threshold = 10,area_threshold = 10 ,merge = False)
    center_coord = []
    for blob in blob_list:
        x = blob.x() + int(blob.w() / 2)
        y = blob.y() + int(blob.h() / 2)
        center_coord.append([x,y])
        #roi = (blob.x() - extension / 2, blob.y() - extension / 2, blob.w() +
        #extension, blob.h() + extension)
        img.draw_rectangle(int(ROI[0]),int(ROI[1]),int(ROI[2]),int(ROI[3]))
        print(img.compressed_for_ide(),end="")

    return center_coord

def send_coords(coords):
    print('------------------------------')
    num = len(coords)
    uart.writechar(num)
    index = 0
    for coord in coords:
        uart.writechar(index)
        uart.writechar(coord[0])
        uart.writechar(coord[1])
        print(index,coord[0],coord[1])
        index = index + 1

clock = time.clock()

while(True):
    clock.tick()
    while(uart.any() == 0):
        continue
    while(uart.any() != 0):
        if uart.readchar() != RQST:
            print("First command should be 'RQST'!")
            continue
        else:
            uart.writechar(ACK)
            print("Good Handshake!")
            break
    while(uart.any() == 0):
        continue
    while(uart.any() != 0):
        desired_color = uart.readchar()-3#0=red,1=green
        #print(desired_color)
        r_min = uart.readchar()
        r_max = uart.readchar()
        method = uart.readchar()-5#0=blob,1=circles
        #print(method)
        pos = uart.readchar()-7#0=start,1=check1,2=check2
        #print(pos)
        if (desired_color not in [0,1]) or (method not in [0,1]) or (pos not in [0,1,2]):
            print("invalid command")
            continue
        else:
            fb1.replace(sensor.snapshot().lens_corr(1.7))
            for i in range(10):
                img = fb1.blend(sensor.snapshot().lens_corr(1.7),alpha = 24)
            if method == 0:
                center_coord = find_blobs(img,desired_color,roi[pos])
            else:
                center_coord = find_circs(img,desired_color,r_min,r_max,roi[pos])
            send_coords(center_coord)
