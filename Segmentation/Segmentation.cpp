#include "Segmentation.h"

Segmentation::Segmentation()
{}

void Segmentation::Init(std::shared_ptr<Image> img)
{
    orygImage = img;

    masks.emplace_back(orygImage, 0, 100, 0, 20);
    strategies.push_back(std::make_unique<Thresholding>(100));

    masks.emplace_back(orygImage, 0, 100, 20, 40);
    strategies.push_back(std::make_unique<Thresholding>(120));

    masks.emplace_back(orygImage, 0, 100, 40, 60);
    strategies.push_back(std::make_unique<Thresholding>(140));

    masks.emplace_back(orygImage, 0, 100, 60, 80);
    strategies.push_back(std::make_unique<Thresholding>(120));

    masks.emplace_back(orygImage, 0, 100, 80, 100);
    strategies.push_back(std::make_unique<Thresholding>(120));

    for (int i = 0; i < masks.size(); i++)
    {
        strategies[i]->Init(masks[i].maskColors);
    }
}

void Segmentation::RunStep()
{
    for (int i = 0; i < masks.size(); i++)
    {
        al_set_target_bitmap(masks[i].bmp.get());
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        strategies[i]->RunStep(orygImage, masks[i]);
    }
}

void Segmentation::Draw()
{
    for (auto &mask : masks)
    {
        mask.Draw();
    }
}

Segmentation::~Segmentation() {}
