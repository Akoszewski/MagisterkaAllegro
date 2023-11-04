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

void Segmentation::prepareGfx(int regionIdx)
{
    al_clear_to_color(al_map_rgb(0, 50, 255));
    orygImage->Draw();

    al_set_target_bitmap(masks[regionIdx].bmp.get());
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
}

void Segmentation::RunStep()
{
    for (int i = 0; i < masks.size(); i++)
    {
        int maxVariance = 0;
        int bestThreshold = 0;
        for (int j = 0; j < 255; j++)
        {
            prepareGfx(i);
            int variance = strategies[i]->RunStep(orygImage, masks[i], j);
            Draw();

            if (maxVariance < variance) {
                maxVariance = variance;
                bestThreshold = j;
            }
        }
        printf("Best threshold: %d\n", bestThreshold);
        prepareGfx(i);
        strategies[i]->RunStep(orygImage, masks[i], bestThreshold);
        Draw();
    }
}

void Segmentation::Draw()
{
    for (auto &mask : masks)
    {
        mask.Draw();
    }
    // al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
    al_flip_display();
}

Segmentation::~Segmentation() {}
