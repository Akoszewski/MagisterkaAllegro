// Assume 'image' is a binary image where pixels are either 0 or 1
// 'structuringElement' is a matrix (2D vector) representing the structuring element

std::unique_ptr<Image> erode(const Image& image, const std::vector<std::vector<int>>& structuringElement) {
    std::unique_ptr<Image> erodedImage = std::make_unique<Image>(image.width, image.height);

    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            bool erodePixel = false;
            for (int i = 0; i < structuringElement.size(); i++) {
                for (int j = 0; j < structuringElement[i].size(); j++) {
                    int nx = x + i - structuringElement.size() / 2;
                    int ny = y + j - structuringElement[i].size() / 2;
                    if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
                        if (structuringElement[i][j] == 1 && image.getPixel(nx, ny) == 0) {
                            erodePixel = true;
                        }
                    }
                }
            }
            erodedImage->setPixel(x, y, erodePixel ? 0 : 1);
        }
    }
    return erodedImage;
}

std::unique_ptr<Image> dilate(const Image& image, const std::vector<std::vector<int>>& structuringElement) {
    std::unique_ptr<Image> dilatedImage = std::make_unique<Image>(image.width, image.height);

    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            bool dilatePixel = false;
            for (int i = 0; i < structuringElement.size(); i++) {
                for (int j = 0; j < structuringElement[i].size(); j++) {
                    int nx = x + i - structuringElement.size() / 2;
                    int ny = y + j - structuringElement[i].size() / 2;
                    if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
                        if (structuringElement[i][j] == 1 && image.getPixel(nx, ny) == 1) {
                            dilatePixel = true;
                        }
                    }
                }
            }
            dilatedImage->setPixel(x, y, dilatePixel ? 1 : 0);
        }
    }
    return dilatedImage;
}

std::unique_ptr<Image> open(const Image& image, const std::vector<std::vector<int>>& structuringElement) {
    return dilate(*erode(image, structuringElement), structuringElement);
}
