import numpy as np
import cv2

image = cv2.imread('pictures/roi.bmp', cv2.IMREAD_GRAYSCALE)

# Apply median filter
median_filtered_image = cv2.medianBlur(image, 5)  # Here, 5 is the kernel size

cv2.imwrite('pictures/median.png', median_filtered_image)
