#include <sil/sil.hpp>
#include <iostream>
#include <algorithm>
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
            image.pixel(x, y).r = std::pow(image.pixel(x, y).r, p); // valeur de couleur rouge puissande p
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
        int offsetx = static_cast<int>(cos(angle) * 2 * radius);
        int offsety = static_cast<int>(sin(angle) * 2 * radius);

        sil::Image circle(w, h);
        draw_empty_circle(circle);

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                if (circle.pixel(x, y).r > 0.f)
                {
                    int nx = x + offsetx;
                    int ny = y + offsety;

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
            mini.pixel(x / step, y / step) = image.pixel(x, y); // met 1 pixel sur 5 dans la nouvelle image pour la réduire
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

// -----------------------⭐⭐⭐
void color_gradient(sil::Image &image)
{
    glm::vec3 red = glm::vec3(1, 0, 0);
    glm::vec3 green = glm::vec3(0, 1, 0);
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float t = x / float(image.width() - 1);
            glm::vec3 color = glm::mix(red, green, t);
            image.pixel(x, y).r = color.r;
            image.pixel(x, y).g = color.g;
            image.pixel(x, y).b = color.b;
        }
    }
}

// -----------------------⭐⭐⭐
void pixel_sorting(sil::Image &image)
{
    int w = image.width();
    int h = image.height();

    std::vector<glm::vec3> pixels = image.pixels();

    // tri ligne par ligne
    for (int y = 0; y < h; y++)
    {
        if (random_int(0, 1) != 0) // 1 chance sur 2 de trier la ligne
            continue;

        int start = random_int(0, w - 1);
        int length = random_int(1, 150);
        if (start + length > w) // éviter que lenghth sorte de l'image
        {
            length = w - start;
        }

        std::vector<glm::vec3> row; // nouvau tableau pour la ligne en question

        for (int x = start; x < start + length; x++)
            row.push_back(pixels[y * w + x]);

        std::sort(row.begin(), row.end(),
                  [](const glm::vec3 &a, const glm::vec3 &b)
                  {
                      return (a.r + a.g + a.b) < (b.r + b.g + b.b); // tri selon la somme des composantes r+g+b
                  });

        for (int i = 0; i < length; i++)
            pixels[y * w + start + i] = row[i];
    }
    image.pixels() = pixels; // met à jour l'image avec le vector trié
}

// -----------------------⭐⭐⭐(⭐)
void tramage(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    float bayer[4][4] = {
        {-0.5, 0, -0.375, 0.125},
        {0.25, -0.25, 0.375, -0.125},
        {-0.3125, 0.1875, 60.4375, 0.0625},
        {0.4375, -0.0625, 0.3125, -0.1875}};

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec3 color = image.pixel(x, y);
            // calcul luminosité
            float brightness = (0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b);

            int bx = x % 4;
            int by = y % 4;
            float bayer_value = bayer[by][bx] * 1.8; // multiplier pour assombrir l'image un peu

            if (brightness > bayer_value)
                image.pixel(x, y) = glm::vec3(1, 1, 1); // blanc
            else
                image.pixel(x, y) = glm::vec3(0, 0, 0); // noir
        }
    }
}

// -----------------------⭐⭐⭐(⭐)
void normalisation(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    glm::vec3 darkest = image.pixel(0, 0);   // pixel le plus foncé
    glm::vec3 brightest = image.pixel(0, 0); // pixel le plus clair
    float min_brightness = 1.0f;
    float max_brightness = 0.0f;

    // trouver min et max
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec3 color = image.pixel(x, y);
            // calcul luminosité
            float brightness = (0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b);
            if (brightness < min_brightness)
                min_brightness = brightness;
            if (brightness > max_brightness)
                max_brightness = brightness;
        }
    }
    float range = max_brightness - min_brightness;

    if (range == 0)
        return; // éviter de diviser par 0

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec3 color = image.pixel(x, y);
            float brightness = 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
            float difference = (brightness - min_brightness) / range;

            glm::vec3 newColor = color * (difference / brightness);
            image.pixel(x, y) = newColor;
        }
    }
}

// -----------------------⭐⭐⭐⭐
void convolutions(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    sil::Image copy = image; // copier l'image originale

    int flou = 8; // intensité du flou
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec3 somme(0); // stoque la somme des couleurs RGB
            int count = 0;

            for (int dx = -flou; dx <= flou; dx++) // les voisins sur les coté
            {
                for (int dy = -flou; dy <= flou; dy++) // les voisins en haut et en bas
                {
                    int nx = x + dx;
                    int ny = y + dy; // coordonnées du voisin actuel

                    // vérifie qu'on est pas hors image
                    if (nx >= 0 && nx < w && ny >= 0 && ny < h)
                    {
                        somme += copy.pixel(nx, ny);
                        count++;
                    }
                }
            }
            // moyenne sur tous les pixels voisins
            image.pixel(x, y) = somme / float(count); // la somme de tt les pixels diviser par 9
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
    {
        sil::Image image{"images/logo.png"};
        pixel_sorting(image);
        image.save("output/pixel_sorting.png");
    }
    {
        sil::Image image{"images/photo_faible_contraste.jpg"};
        tramage(image);
        image.save("output/tramage.jpg");
    }
    {
        sil::Image image{"images/photo_faible_contraste.jpg"};
        normalisation(image);
        image.save("output/normalisation.jpg");
    }
    {
        sil::Image image{"images/logo.png"};
        convolutions(image);
        image.save("output/convolutions.png");
    }
}