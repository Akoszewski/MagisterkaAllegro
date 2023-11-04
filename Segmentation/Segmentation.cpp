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

void Segmentation::Init(std::shared_ptr<Image> img, std::unique_ptr<Mask> mask)
{
    orygImage = img;
    this->mask = std::move(mask);

    al_set_target_bitmap(this->mask->bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_set_target_backbuffer(al_get_current_display());

    strategy->Init(maskColors);
}

void Segmentation::NextStep()
{
    al_set_target_bitmap(mask->bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    if (strategy) { 
        strategy->NextStep(orygImage, *mask);
    }
}

void Segmentation::Draw()
{
    mask->Draw();
}

Segmentation::~Segmentation() {}
