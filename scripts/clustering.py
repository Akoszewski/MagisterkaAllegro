# Please write a python3 algorithm that will open the file ./pictures/cropped_images/roiCropped.png
# and read intensity and position of each pixel and create 3D space with one dimention for x, one for y and one for intensity.
# I would like to have possibility to assign a weigh to each of the dimentions and they should be first normalized
# intensities should be normalized from (0 - 255) to (0 : 1000), pixel x from (0 : imageWidth) to (0 : 1000) and pixel y from (0 : imageHeight)
# to (0:1000). After the normalization the weight should rescale each dimension data according to the weight parameter.
# Parameters for weight will be from 0 to 1.0. By default they should be equal to 1.0. After that perform clasterization
# of the data with MeanShift algorithm. At the end please display the image and color every pixel according to its cluster.
                                                                                                               
import cv2
import numpy as np
from sklearn.cluster import MeanShift
import matplotlib.pyplot as plt

def normalize_and_weight(data, min_val, max_val, weight):
    """
    Normalize data to a range (0, 1000) and then apply the weight.
    """
    normalized_data = 1000 * (data - min_val) / (max_val - min_val)
    return normalized_data * weight

def process_image(file_path, weight_x=0.0, weight_y=0.0, weight_intensity=1.0):
    # Load image in grayscale to get intensity values
    image = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
    if image is None:
        return None, None

    # Get image dimensions
    height, width = image.shape

    # Create meshgrid for pixel coordinates
    x, y = np.meshgrid(np.arange(width), np.arange(height))

    # Normalize and weight dimensions
    x_normalized = normalize_and_weight(x, 0, width, weight_x)
    y_normalized = normalize_and_weight(y, 0, height, weight_y)
    intensity_normalized = normalize_and_weight(image, 0, 255, weight_intensity)

    # Reshape for clustering
    data = np.stack((x_normalized.ravel(), y_normalized.ravel(), intensity_normalized.ravel()), axis=1)

    # Perform MeanShift clustering
    clustering = MeanShift(bandwidth=30).fit(data)
    labels = clustering.labels_

    # Color each pixel according to its cluster
    label_image = labels.reshape((height, width))
    unique_labels = np.unique(labels)

    # Create a color map for clusters
    color_map = plt.get_cmap("nipy_spectral")
    colored_image = color_map(label_image / np.max(label_image))

    return image, colored_image

# Process the image
# file_path = '../pictures/cropped_images/roiCropped.png'
file_path = 'out.png'

original_image, colored_image = process_image(file_path)

# Display original and clustered images if the process was successful
if original_image is not None and colored_image is not None:
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    plt.imshow(original_image, cmap='gray')
    plt.title('Original Image')
    plt.axis('off')

    plt.subplot(1, 2, 2)
    plt.imshow(colored_image)
    plt.title('Clustered Image')
    plt.axis('off')

    plt.show()
else:
    print("Failed to process the image. Please check the file path or file format.")

import os
os.environ['QT_PLUGIN_PATH'] = '/usr/lib/qt/plugins'  # Adjust the path according to your system
