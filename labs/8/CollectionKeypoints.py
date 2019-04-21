#Collect Keypoints for Calibration By Dylan DeSantis
##################################
# This program finds corners of a checkered board and
# outputs them to a text file and saves the image
##################################

import sensor, image, time

#Sensor Setup
#################################
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
#sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_contrast(3)
sensor.set_brightness(3)
sensor.set_gainceiling(16)
#sensor.set_framesize(sensor.VGA)
sensor.set_framesize(sensor.QQVGA)
#sensor.set_windowing((480,400))
#sensor.set_lens_correction(True,25,500)
sensor.skip_frames(time = 4000)
sensor.set_auto_gain(False, value =100)
clock = time.clock()
#################################

#Initialize Parameters
#################################
fo = open("corners.txt","w")
iters = 0
cal_points = []
unique_cal_points = []
cond = 0
break_away = 0
#################################

#Main Loop
#################################
# You may need to adjust the thresholds for your own
# Camera. This will save an image and corners to your
# Camera, and will take up alot of space so make sure
# you delete the files after you export them.
#################################
while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(1.7)#.morph(1,[2,4,-2,-4,2,4,-2,-4,2],mult=10,add=0).gaussian(3)
    if cond ==1:
        break_away = 1
    for rec in img.find_rects(threshold=1900):
        corners = rec.corners()
        cal_points.append((corners[0][0],corners[0][1]))
        cal_points.append((corners[1][0],corners[1][1]))
        cal_points.append((corners[2][0],corners[2][1]))
        cal_points.append((corners[3][0],corners[3][1]))
        #img.draw_rectangle((rec.x(),rec.y(),rec.w(),rec.h()),color =(255,0,0))
        iters +=4
        time.sleep(100)

    if iters>200 and break_away ==0:
        cond = 1
        temp_unique_points = set()
        itersa = 0
        img = sensor.snapshot().lens_corr(1.7)
        for pt in cal_points:
            if pt not in temp_unique_points:
                temp_unique_points.add(pt)
                unique_cal_points.append((pt[0],pt[1]))

        for upt in unique_cal_points:
            str1a = '{},{},{}'.format(itersa,upt[0],upt[1])
            #print(str1a)
            #str1b = '{}'.format(iters)
            itersa +=1
            #img = sensor.snapshot().lens_corr(1.7)
            #img.draw_string(upt[0],upt[1],str1b,color=(255,0,0))
            #img.draw_cross(int(upt[0]),(upt[1]),size=1,color=(255,0,0))
            fo.write(str1a+'\n')
        fo.close()
        del unique_cal_points
        del temp_unique_points
        img.save("test_image_label")
    if break_away ==1:
        break


