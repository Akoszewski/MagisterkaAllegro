#include "Segmentation.h"

Segmentation::Segmentation()
{
    SetStrategy(std::make_unique<Thresholding>(140));
}

void Segmentation::SetStrategy(std::unique_ptr<SegmentationStrategy> strategy)
{
    this->strategy = std::move(strategy);
}

void Segmentation::Init(std::shared_ptr<Image> img, std::unique_ptr<Mask> mask)
{
    orygImage = img;
    this->mask = std::move(mask);

    al_set_target_bitmap(this->mask->bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_set_target_backbuffer(al_get_current_display());

    strategy->Init(this->mask->maskColors);
}

void Segmentation::RunStep()
{
    al_set_target_bitmap(mask->bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    if (strategy) { 
        strategy->RunStep(orygImage, *mask);
    } else {
        printf("Segmentation strategy is not set\n");
    }
}

void Segmentation::Draw()
{
    mask->Draw();
}

Segmentation::~Segmentation() {}
