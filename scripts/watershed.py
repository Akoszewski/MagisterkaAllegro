import cv2
import numpy as np

# Read the image in grayscale
image = cv2.imread('../pictures/cropped_images/roiCropped.png', 0)  # 0 for grayscale

# Convert the grayscale image to 3-channel color image
color_image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)

# Thresholding to find markers
ret, thresh = cv2.threshold(image, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

# Noise removal (optional)
kernel = np.ones((3,3), np.uint8)
opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=2)

# Determining the background area
sure_bg = cv2.dilate(opening, kernel, iterations=3)

# Determining the foreground area
dist_transform = cv2.distanceTransform(opening, cv2.DIST_L2, 5)
ret, sure_fg = cv2.threshold(dist_transform, 0.7*dist_transform.max(), 255, 0)

# Finding the unknown region
sure_fg = np.uint8(sure_fg)
unknown = cv2.subtract(sure_bg, sure_fg)

# Marker labelling
ret, markers = cv2.connectedComponents(sure_fg)

# Adding 1 to all labels so that sure background is not 0, but 1
markers = markers + 1

# Marking the region of unknown with zero
markers[unknown == 255] = 0

# Applying the Watershed algorithm on the color image
markers = cv2.watershed(color_image, markers)
color_image[markers == -1] = [255, 0, 0]  # Mark boundaries in red

# Displaying the result
cv2.imshow('Segmented Image', color_image)
cv2.waitKey(0)
cv2.destroyAllWindows()

