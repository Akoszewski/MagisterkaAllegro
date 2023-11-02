#include "Segmentation.h"

Segmentation::Segmentation()
  : maskTransparency(0)
{
    printf("Przezroczystosc: %d\n", maskTransparency);
    maskColors =
    {
        al_map_rgba(100, 0, 0, maskTransparency),
        al_map_rgba(0, 100, 0, maskTransparency),
        al_map_rgba(0, 0, 100, maskTransparency),
        al_map_rgba(255, 0, 0, maskTransparency),
        al_map_rgba(0, 255, 0, maskTransparency),
        al_map_rgba(0, 0, 255, maskTransparency),
        al_map_rgba(100, 100, 0, maskTransparency),
        al_map_rgba(0, 100, 100, maskTransparency),
        al_map_rgba(255, 255, 0, maskTransparency),
        al_map_rgba(0, 255, 255, maskTransparency),
    };

    SetStrategy(std::make_unique<Thresholding>(100));
}

void Segmentation::SetStrategy(std::unique_ptr<SegmentationStrategy> strategy)
{
    this->strategy = std::move(strategy);
    if (this->strategy) {
        printf("Strategy set\n");
    }
}

void Segmentation::initMask(std::shared_ptr<Image> img, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent)
{
    mask.x = orygImage->x + xStartPercent/100.0f * al_get_bitmap_width(orygImage->bmp);
    mask.y = orygImage->y + yStartPercent/100.0f * al_get_bitmap_height(orygImage->bmp);

    byte widthPercent = abs(xEndPercent - xStartPercent);
    byte heightPercent = abs(yEndPercent - yStartPercent);

    int width = widthPercent/100.0f * al_get_bitmap_width(orygImage->bmp);
    int height = heightPercent/100.0f * al_get_bitmap_height(orygImage->bmp);

    mask.bmp = al_create_bitmap(width, height);
    if (!mask.bmp) {
        puts("Failed to create new bitmap");
        return;
    }
}

void Segmentation::Init(std::shared_ptr<Image> img)
{
    orygImage = img;
    initMask(img, 0, 100, 60, 80);

    al_set_target_bitmap(mask.bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_set_target_backbuffer(al_get_current_display());

    strategy->Init(maskColors);
}

void Segmentation::NextStep()
{
    al_set_target_bitmap(mask.bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    if (strategy) { 
        strategy->NextStep(orygImage, mask);
    }
}

void Segmentation::Draw()
{
    mask.Draw();
}

Segmentation::~Segmentation() {}
