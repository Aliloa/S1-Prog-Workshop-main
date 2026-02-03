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

void draw_circle(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            float dx = x - w / 2;
            float dy = y - h / 2;
            float diametre = dx * dx + dy * dy; // calcul du diametre d'un cercle
            if (diametre <= 100 * 100)          // à l'intérieur du cercle
            {
                image.pixel(x, y).r = 1.0f; // blanc
                image.pixel(x, y).g = 1.0f;
                image.pixel(x, y).b = 1.0f;
            }
        }
    }
}

void draw_empty_circle(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    int thickness = 10;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            float dx = x - w / 2;
            float dy = y - h / 2;
            float diametre = dx * dx + dy * dy;
            if (diametre <= 100 * 100 && diametre >= (100 - thickness) * (100 - thickness)) // à l'intérieur du cercle
            {
                image.pixel(x, y).r = 1;
                image.pixel(x, y).g = 1;
                image.pixel(x, y).b = 1;
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
    int speed = 5;

    for (int i = 0; i < total_frames; i++)
    {
        // reset image
        for (glm::vec3 &p : image.pixels())
            p = glm::vec3(0.f);

        sil::Image circle(w, h);
        draw_circle(circle); // cercle centré

        int offsetX = i * speed;
        int offsetY = 0;

        // recopie circle dans image avec décalage
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                if (circle.pixel(x, y).r > 0.f)
                {
                    int nx = x + offsetX;
                    int ny = y + offsetY;

                    if (nx >= 0 && nx < w && ny >= 0 && ny < h)
                        image.pixel(nx, ny) = circle.pixel(x, y);
                }
            }
        }

        // sauvegarder la frame
        std::string filename = "output/animation/frame_" + std::to_string(i) + ".png";
        image.save(filename);
    }
}

// -----------------------⭐⭐⭐

void rosace(sil::Image &image)
{
    int w = image.width();
    int h = image.height();

    int circles = 6;
    int radius = 50;

    // cercle central
    {
        sil::Image circle(w, h);
        draw_empty_circle(circle);

        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                if (circle.pixel(x, y).r > 0.f)
                    image.pixel(x, y) = circle.pixel(x, y);
    }

    // cercles autour
    for (int i = 0; i < circles; i++)
    {
        float angle = i * 2.f * M_PI / circles;
        int offsetX = static_cast<int>(cos(angle) * 2 * radius);
        int offsetY = static_cast<int>(sin(angle) * 2 * radius);

        sil::Image circle(w, h);
        draw_empty_circle(circle);

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                if (circle.pixel(x, y).r > 0.f)
                {
                    int nx = x + offsetX;
                    int ny = y + offsetY;

                    if (nx >= 0 && nx < w && ny >= 0 && ny < h)
                        image.pixel(nx, ny) = circle.pixel(x, y);
                }
            }
        }
    }
}

// -----------------------⭐⭐
void mosaique(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    // sil::Image copy(w, h);
    int step = 5; // on prend un pixel tous les 5

    int mini_w = w / step;
    int mini_h = h / step;
    sil::Image mini(mini_w, mini_h);
    for (int x = 0; x < w; x += step)
    {
        for (int y = 0; y < h; y += step)
        {
            mini.pixel(x / step, y / step) = image.pixel(x, y); // on "réduit" visuellement
        }
    }

    // répéter 5 fois en hauteur et en largeur
    sil::Image result(mini_w * 5, mini_h * 5); // image finale
    for (int i = 0; i < 5; i++)                // largeur
    {
        for (int j = 0; j < 5; j++) // hauteur
        {
            for (int x = 0; x < mini_w; x++)
            {
                for (int y = 0; y < mini_h; y++)
                {
                    result.pixel(x + i * mini_w, y + j * mini_h) = mini.pixel(x, y);
                }
            }
        }
    }

    image = result; // on remplace l'image originale
}

// -----------------------⭐⭐⭐⭐
void mosaique_mirror(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    // sil::Image copy(w, h);
    int step = 5; // on prend un pixel tous les 5

    int mini_w = w / step;
    int mini_h = h / step;
    sil::Image mini(mini_w, mini_h);
    for (int x = 0; x < w; x += step)
    {
        for (int y = 0; y < h; y += step)
        {
            mini.pixel(x / step, y / step) = image.pixel(x, y); // on "réduit" visuellement
        }
    }

    // répéter 5 fois en hauteur et en largeur
    sil::Image result(mini_w * 5, mini_h * 5); // image finale
    for (int i = 0; i < 5; i++)                // largeur
    {
        for (int j = 0; j < 5; j++) // hauteur
        {
            bool mirror_w = (i + j) % 2 == 1; // une image sur deux en largeur
            bool mirror_h = j % 2 == 1;       // une image sur deux en hauteur

            for (int x = 0; x < mini_w; x++)
            {
                for (int y = 0; y < mini_h; y++)
                {
                    int src_x = mirror_w ? (mini_w - 1 - x) : x;
                    int src_y = mirror_h ? (mini_h - 1 - y) : y;
                    result.pixel(x + i * mini_w, y + j * mini_h) = mini.pixel(src_x, src_y);
                }
            }
        }
    }

    image = result; // on remplace l'image originale
}

// -----------------------⭐⭐⭐
void glitch(sil::Image &image)
{
    int w = image.width();
    int h = image.height();

    for (int i = 0; i < 60; i++) // repter 20 fois
    {
        // générer rectangle aléatoire
        int rect_w = random_int(5, 40);     // largeur du rectangle
        int rect_h = random_int(2, 10);     // hauteur du rectangle
        int x1 = random_int(0, w - rect_w); // position de départ 1
        int y1 = random_int(0, h - rect_h);

        int x2 = random_int(0, w - rect_w); // position de départ 2
        int y2 = random_int(0, h - rect_h);

        // échanger tous les pixels du rectangle
        for (int dx = 0; dx < rect_w; dx++)
        {
            for (int dy = 0; dy < rect_h; dy++)
            {
                std::swap(
                    image.pixel(x1 + dx, y1 + dy),
                    image.pixel(x2 + dx, y2 + dy));
            }
        }
    }
}

void color_gradient(sil::Image &image)
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

void example2(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
        }
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
    // {
    //     sil::Image image{"images/logo.png"};
    //     swap_canals(image);
    //     image.save("output/swap_canals.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     black_and_white(image);
    //     image.save("output/black_and_white.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     negatif(image);
    //     image.save("output/negatif.png");
    // }
    // {
    //     sil::Image image{300 /*width*/, 200 /*height*/};
    //     gradient(image);
    //     image.save("output/gradient.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     mirror(image);
    //     image.save("output/mirror.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     noise(image);
    //     image.save("output/noise.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     rotate(image);
    //     image.save("output/rotate.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     split(image);
    //     image.save("output/split.png");
    // }
    // {
    //     sil::Image image{"images/photo_faible_contraste.jpg"};
    //     lighting(image);
    //     image.save("output/lighting.png");
    // }
    // {
    //     sil::Image image{500 /*width*/, 500 /*height*/};
    //     draw_circle(image);
    //     image.save("output/circle.png");
    // }
    // {
    //     sil::Image image{500 /*width*/, 500 /*height*/};
    //     draw_empty_circle(image);
    //     image.save("output/empty_circle.png");
    // }
    // {
    //     sil::Image image{500 /*width*/, 500 /*height*/};
    //     animation(image);
    // }
    // {
    //     sil::Image image{600 /*width*/, 600 /*height*/};
    //     rosace(image);
    //     image.save("output/rosace.png");
    // }
    {
        sil::Image image{"images/logo.png"};
        mosaique(image);
        image.save("output/mosaique.png");
    }
    {
        sil::Image image{"images/logo.png"};
        mosaique_mirror(image);
        image.save("output/mosaique_mirror.png");
    }
    {
        sil::Image image{"images/logo.png"};
        glitch(image);
        image.save("output/glitch.png");
    }
    {
        sil::Image image{300 /*width*/, 200 /*height*/};
        color_gradient(image);
        image.save("output/color_gradient.png");
    }
}