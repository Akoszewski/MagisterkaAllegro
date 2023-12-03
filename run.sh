g++ main.cpp App.cpp Image.cpp Utils.cpp Mask.cpp \
    Segmentation/Segmentation.cpp Segmentation/KMeans.cpp \
    Segmentation/Thresholding.cpp Segmentation/Filtering.cpp \
    Segmentation/MorphOperations.cpp Segmentation/KMeansWrap.cpp \
    Segmentation/KMeans3D.cpp \
    `pkg-config --cflags --libs allegro-5 allegro_ttf-5 allegro_primitives-5 \
    allegro_memfile-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_dialog-5\
    allegro_color-5 allegro_audio-5 allegro_acodec-5` \
    -g -O0 -o app.exe || exit
./app.exe
