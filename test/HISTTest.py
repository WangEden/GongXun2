import numpy as np
import cv2
import matplotlib.pyplot as plt


img = cv2.imread("HSVTestResult2.jpg")
hist = np.bincount(img.ravel(), minlength=256)

img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
# plt.hist(img.ravel(), 256, [0, 256])
# plt.plot(hist)
color = ('b','g','r')
for i,col in enumerate(color):
    histr = cv2.calcHist([img],[i],None,[256],[0,256])
    plt.plot(histr,color = col)
    plt.xlim([0,256])
plt.show()


