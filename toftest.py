from __future__ import division
import serial
import numpy as np   
import matplotlib.pyplot as plt


ser = serial.Serial('COM10', baudrate = 115200)


def find_distance(time):
        c = 3e8
        distance = c*time/2
        print(distance)
        return distance

def find_square(distance, beacon):
    top_left=[beacon[0]-distance,beacon[1]+distance]
    top_right=[beacon[0]+distance,beacon[1]+distance]
    bottom_left=[beacon[0]-distance,beacon[1]-distance]
    bottom_right=[beacon[0]+distance,beacon[1]-distance]
    return [top_left,top_right,bottom_left,bottom_right]
    
def line2(p1,p2):
    A=(p1[1]-p2[1])
    B=(p2[0]-p1[0])
    C=(p1[0]-p2[0]*p1[1])
    return A,B,-C
    
def intersection(L1,L2):
    D= L1[0]*L2[1]-L1[1]*L2[0]
    Dx=L1[2]*L2[1]-L1[1]*L2[2]
    Dy=L1[0]*L2[1]-L1[2]*L2[0]
    if D!=0:
        x=Dx/D
        y=Dy/D
        return x,y
def find_center(point1,point2):
    x=(point1[0]+point2[0])/2
    y=(point1[1]+point2[1])/2
    return [x,y]

def plot_rover(tx,ty):
    
    #plt.ion()

    newx=tx
    newy = ty
    print(tx,ty)
    fig = plt.figure()
    
    ax = fig.add_subplot()
    im = plt.imread("newmap.png")
    im = plt.imshow(im,extent=[-500,500,-500,500])
    ax.set(xlim=(-500, 500), ylim=(-500, 500))
    plt.plot(newx, newy, 'ro')
    
    
    plt.show()
    #plt.gcf().gca().remove()

    
    get_values()
    

def find_rover(time1,time2,time3,beacon1,beacon2,beacon3):
    d1=find_distance(time1)
    d2=find_distance(time2)
    d3=find_distance(time3)

    s1=find_square(d1, beacon1)
    s2=find_square(d2, beacon2)    
    s3=find_square(d3, beacon3)

    tsbr= intersection(line2(s1[1],s1[3]),line2(s3[2],s3[3]))
    tsbl =intersection(line2(s2[0],s1[2]),line2(s3[2],s3[3]))
    tstr= intersection(line2(s1[1],s1[3]),line2(s3[0],s3[1]))
    tst1=s2[0]

    target=find_center(tst1, tsbr)
    tx=str(round(target[0]))
    ty=str(round(target[1]))
   
    print(tx,ty)
    plot_rover(tx,ty)
    
def get_values():
    beacon1 = [0,150]
    beacon2 = [500,-200]
    beacon3 = [-250,-200]
    time1 =0
    time2=0
    time3=0
    
    count = 0
    addr =np.empty([],dtype=str)
    timeflight =np.array([],dtype=float)
      #np.delete(timeflight,0,0)
    

    while count < 3:
   
        line1 = ser.readline().decode('ascii')
        add,tof = line1.split(",")
        add=str(add)
        addr = np.append(addr,add)
        tof=float(tof)
        timeflight = np.append(timeflight,tof)
        print("From Node:",add,"ToF:",tof)
        count +=1
    time1 = timeflight[0]
    time2 = timeflight[1]
    time3 = timeflight[2]
    
    print(timeflight)
    
    find_rover(time1,time2,time3,beacon1,beacon2,beacon3)
    
    
get_values()





    
    
