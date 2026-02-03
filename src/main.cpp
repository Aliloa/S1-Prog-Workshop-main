#include <sil/sil.hpp>
#include <iostream>
#include "random.hpp"

// -----------------------⭐
void keep_green_only(sil::Image &image) // Prend l'image par référence pour pouvoir la modifier
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}

void swap_canals(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        std::swap(color.r, color.b);
    }
}

void black_and_white(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        float gray = 0.3f * color.r + 0.59f * color.g + 0.11f * color.b;

        color.r = gray;
        color.g = gray;
        color.b = gray;
    }
}

void negatif(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = std::abs(color.r - 1.f);
        color.g = std::abs(color.g - 1.f);
        color.b = std::abs(color.b - 1.f);
    }
}

void gradient(sil::Image &image)
{
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float t = x / float(image.width() - 1);
            image.pixel(x, y).r = t;
            image.pixel(x, y).g = t;
            image.pixel(x, y).b = t;
        }
    }
}

// -----------------------⭐⭐

void mirror(sil::Image &image)
{
    int w = image.width();
    for (int x{0}; x < w / 2; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            std::swap(image.pixel(x, y), image.pixel(w - 1 - x, y));
        }
    }
}

void noise(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        int n = random_int(0, 4);
        if (n == 1)
        {
            color.r = random_float(0, 1);
            color.g = random_float(0, 1);
            color.b = random_float(0, 1);
        }
    }
}

void rotate(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    sil::Image rotated(h, w);
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            // rotation horaire 90°
            rotated.pixel(h - 1 - y, x) = image.pixel(x, y);
        }
    }
    image = rotated;
}

void split(sil::Image &image)
{
    sil::Image copy = image;
    int w = image.width();
    int h = image.height();
    int d = 20;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (x + d < w)
                image.pixel(x, y).r = copy.pixel(x + d, y).r;
            else
                image.pixel(x, y).r = copy.pixel(x, y).r; // bord -> garde la valeur originale

            // pour le bleu
            if (x - d >= 0)
                image.pixel(x, y).b = copy.pixel(x - d, y).b;
            else
                image.pixel(x, y).b = copy.pixel(x, y).b; // bord -> garde la valeur originale
        }
    }
}

void lighting(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    float p = 3; //<1 = éclaircir >1=assombrir
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            image.pixel(x, y).r = std::pow(image.pixel(x, y).r, p);
            image.pixel(x, y).g = std::pow(image.pixel(x, y).g, p);
            image.pixel(x, y).b = std::pow(image.pixel(x, y).b, p);
        }
    }
}

void circle(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            float dx = x - w / 2;
            float dy = y - h / 2;
            float distance2 = dx * dx + dy * dy;
            if (distance2 <= 100 * 100) // à l'intérieur du cercle
            {
                image.pixel(x, y).r = 1.0f; // blanc
                image.pixel(x, y).g = 1.0f;
                image.pixel(x, y).b = 1.0f;
            }
        }
    }
}

void empty_circle(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    int thickness = 15;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            float dx = x - w / 2;
            float dy = y - h / 2;
            float distance2 = dx * dx + dy * dy;
            if (distance2 <= 150 * 150 && distance2 >= (150 - thickness) * (150 - thickness)) // à l'intérieur du cercle
            {
                image.pixel(x, y).r = 1.0f; // blanc
                image.pixel(x, y).g = 1.0f;
                image.pixel(x, y).b = 1.0f;
            }
        }
    }
}

void animation(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    int total_frames = 50;
    int radius = 50; 

    for (int frame = 0; frame < total_frames; frame++)
    {
        // image temporaire pour dessiner le cercle
        sil::Image temp(w, h);

        // centre du cercle
        int cx = w / 2;
        int cy = h / 2;

        // dessiner le cercle sur temp
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int dx = x - cx;
                int dy = y - cy;
                if (dx * dx + dy * dy <= radius * radius)
                    temp.pixel(x, y) = {1.0f, 1.0f, 1.0f}; // blanc
            }
        }

        // recopie temp dans image avec décalage
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int newx = x + frame;
                if (newx < w)
                    image.pixel(newx, y) = temp.pixel(x, y);
            }
        }

        // sauvegarde la frame
        std::string filename = "output/animation/frame_" + std::to_string(frame) + ".png";
        image.save(filename);
    }
}

void example(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0;
        color.g = 0;
        color.b = 0;
    }
}

int main()
{
    {
        sil::Image image{"images/logo.png"};      // Lis l'image
        keep_green_only(image);                   // Utilise la fonction pour modifier l'image
        image.save("output/keep_green_only.png"); // Sauvegarde l'image
    }
    {
        sil::Image image{"images/logo.png"};
        swap_canals(image);
        image.save("output/swap_canals.png");
    }
    {
        sil::Image image{"images/logo.png"};
        black_and_white(image);
        image.save("output/black_and_white.png");
    }
    {
        sil::Image image{"images/logo.png"};
        negatif(image);
        image.save("output/negatif.png");
    }
    {
        sil::Image image{300 /*width*/, 200 /*height*/};
        gradient(image);
        image.save("output/gradient.png");
    }
    {
        sil::Image image{"images/logo.png"};
        mirror(image);
        image.save("output/mirror.png");
    }
    {
        sil::Image image{"images/logo.png"};
        noise(image);
        image.save("output/noise.png");
    }
    {
        sil::Image image{"images/logo.png"};
        rotate(image);
        image.save("output/rotate.png");
    }
    {
        sil::Image image{"images/logo.png"};
        split(image);
        image.save("output/split.png");
    }
    {
        sil::Image image{"images/logo.png"};
        lighting(image);
        image.save("output/lighting.png");
    }
    {
        sil::Image image{500 /*width*/, 500 /*height*/};
        circle(image);
        image.save("output/circle.png");
    }
    {
        sil::Image image{500 /*width*/, 500 /*height*/};
        empty_circle(image);
        image.save("output/empty_circle.png");
    }
    {
        sil::Image image{500 /*width*/, 500 /*height*/};
        animation(image);
    }
}