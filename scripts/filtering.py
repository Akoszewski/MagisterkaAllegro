import numpy as np
import cv2


im = cv2.imread('../pictures/cropped_images/roiCropped.png', cv2.IMREAD_GRAYSCALE)
im = cv2.GaussianBlur(im, (21,21), 0)
cv2.imwrite("out.png", im)
