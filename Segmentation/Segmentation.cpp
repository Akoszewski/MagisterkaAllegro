#include "Segmentation.h"

Segmentation::Segmentation()
{}

void Segmentation::Init(std::shared_ptr<const Image> img)
{
    orygImage = img;

    int percentageInterval = 20;
    for (int i = 0; i < 100; i += percentageInterval)
    {
        masks.emplace_back(*orygImage.get(), 0, 100, i, i+percentageInterval);
        strategies.push_back(std::make_unique<KMeans>(3));
    }
    // masks.emplace_back(orygImage, 0, 100, 0, 20);
    // strategies.push_back(std::make_unique<KMeans>(2));

    // masks.emplace_back(orygImage, 0, 100, 20, 40);
    // strategies.push_back(std::make_unique<KMeans>(2));

    // masks.emplace_back(orygImage, 0, 100, 40, 60);
    // strategies.push_back(std::make_unique<KMeans>(2));

    // masks.emplace_back(orygImage, 0, 100, 60, 80);
    // strategies.push_back(std::make_unique<KMeans>(2));

    // masks.emplace_back(orygImage, 0, 100, 80, 100);
    // strategies.push_back(std::make_unique<KMeans>(2));

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
        strategies[i]->RunStep(*orygImage.get(), masks[i]);
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
