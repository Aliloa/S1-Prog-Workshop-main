#include <sil/sil.hpp>
#include <iostream>
#include <algorithm>
#include "random.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

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
    float p = 1.5; //<1 = éclaircir >1=assombrir
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

// -----------------------⭐⭐(⭐)
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

        //copier les pixels 1 par 1
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                if (circle.pixel(x, y).r > 0)
                {
                    image.pixel(x, y) = circle.pixel(x, y);
                }
            }
        }
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

// -----------------------⭐⭐⭐

// passer linear RGB en sRGB
float rgb_to_srgb(float c)
{
    if (c <= 0.0031308f)
        return 12.92f * c;
    else
        return 1.055f * powf(c, 1.0f / 2.4f) - 0.055f;
}

struct Lab
{
    float L;
    float a;
    float b;
};
struct RGB
{
    float r;
    float g;
    float b;
};

// passer de okLab en RGB linéaire
RGB oklab_to_linear_rgb(Lab c)
{
    float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
    float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
    float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    return {
        +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
        -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
        -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}

void color_gradient(sil::Image &image)
{
    Lab lab_red = {0.62796f, 0.22486f, 0.12585f};    // rouge sRGB
    Lab lab_green = {0.86644f, -0.23389f, 0.17950f}; // vert sRGB

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float t = x / float(image.width() - 1);
            // glm::vec3 color = glm::mix(red, green, t);

            // Interpolation en OkLab
            Lab lab_color;
            lab_color.L = lab_red.L + t * (lab_green.L - lab_red.L);
            lab_color.a = lab_red.a + t * (lab_green.a - lab_red.a);
            lab_color.b = lab_red.b + t * (lab_green.b - lab_red.b);

            // Reconvertir en RGB linéaire
            RGB color = oklab_to_linear_rgb(lab_color);
            // retour en sRGB
            color.r = rgb_to_srgb(color.r);
            color.g = rgb_to_srgb(color.g);
            color.b = rgb_to_srgb(color.b);

            image.pixel(x, y).r = color.r;
            image.pixel(x, y).g = color.g;
            image.pixel(x, y).b = color.b;
        }
    }
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
            float bayer_value = bayer[by][bx] * 2; // multiplier pour assombrir / éclaircir (dépend si seuil inversé) l'image un peu

            if (brightness > bayer_value)               // 1-bayer_value = inverser pour que les noirs soient plus noirs
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
    std::vector<float> brightness_values;

    // trouver min et max
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec3 color = image.pixel(x, y);
            // calcul luminosité
            float brightness = (0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b);
            brightness_values.push_back(brightness);
        }
    }
    // trier le tableau pour avoir la plus grosse et petite valeur
    std::sort(brightness_values.begin(), brightness_values.end());

    float min_brightness = brightness_values.front();
    float max_brightness = brightness_values.back();
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

            // éviter division par 0 pixel noir
            if (brightness > 0.0f)
                image.pixel(x, y) = color * (difference / brightness);
        }
    }
}

// -----------------------⭐⭐⭐⭐
// Fonction donnée:
glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

void vortex(sil::Image &image)
{
    int w = image.width();
    int h = image.height();
    sil::Image copy = image; // copier l'image originale

    glm::vec2 center_of_rotation(w / 2, h / 2);

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec2 position(x, y);

            float distance = glm::distance(position, center_of_rotation); // Distance entre le pixel et le centre du vortex
            float angle = distance * 0.1f;                                // intensité du vortex

            glm::vec2 src = rotated(position, center_of_rotation, angle);

            // int sx = int(src.x);
            // int sy = int(src.y);

            // empecher l'image de sortir
            // if (sx < 0 || sx >= w || sy < 0 || sy >= h)
            // {
            //     continue;
            // }
            int sx = glm::clamp(int(src.x), 0, w - 1);
            int sy = glm::clamp(int(src.y), 0, h - 1);

            image.pixel(x, y) = copy.pixel(sx, sy);
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

// -----------------------⭐⭐⭐⭐⭐
void kuwahara(sil::Image &image)
{
    int w = image.width();
    int h = image.height();

    // ajouter du bruit à l'image
    // noise(image);

    sil::Image copy = image;

    // tableau fixe 4x4 pour les indices de chaque quadrant
    glm::ivec2 carde[4][4] = {
        {{-2, -2}, {-1, -2}, {-2, -1}, {-1, -1}}, // carré 1 : haut-gauche
        {{1, -2}, {2, -2}, {1, -1}, {2, -1}},     // carré 2 : haut-droite
        {{-2, 1}, {-1, 1}, {-2, 2}, {-1, 2}},     // carré 3 : bas-gauche
        {{1, 1}, {2, 1}, {1, 2}, {2, 2}}          // carré 4 : bas-droite
    };

    float std_valeurs[4];
    glm::vec3 moyennes[4]; // pour stocker la moyenne de chaque quadrant

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec3 best_avg(0); // stoque la somme des couleurs RGB
            int count = 0;

            // parcourir les 4 quadrants
            for (int q = 0; q < 4; q++)
            {
                glm::vec3 somme(0);
                glm::vec3 sum_sq(0);
                int count = 0;

                // parcourir les 4 pixels du quadrant
                for (int k = 0; k < 4; k++)
                {
                    int nx = x + carde[q][k].x;
                    int ny = y + carde[q][k].y;

                    // vérifier qu'on reste dans l'image
                    if (nx >= 0 && nx < w && ny >= 0 && ny < h)
                    {
                        glm::vec3 c = copy.pixel(nx, ny);
                        somme += c;
                        sum_sq += c * c; // pour calculer la variance
                        count++;
                    }
                }

                // si on a au moins un pixel dans le quadrant
                if (count > 0)
                {
                    glm::vec3 moyenne = somme / float(count); // moyenne
                    glm::vec3 var = sum_sq / float(count) - moyenne * moyenne;
                    glm::vec3 std_dev = glm::sqrt(var); // calcul standart deviation (dit dans la video)

                    float std_val = std_dev.r + std_dev.g + std_dev.b; // une seule valeur
                    std_valeurs[q] = std_val;                          // stocker les valeur de standart deviation
                    moyennes[q] = moyenne;                             // stocker les couleurs moyennes
                }
            }

            // trouver la plus petite valeur de standart deviation et recupérer son index
            auto it = std::min_element(std_valeurs, std_valeurs + 4);

            int best_q = it - std_valeurs;

            // appliquer la couleur moyenne du quadrant le plus homogène
            image.pixel(x, y) = moyennes[best_q]; // utiliser l'index de standart deviation le plus bas pour retrouver la moyenne qui lui correspond
        }
    }
}

// -----------------------⭐⭐⭐⭐⭐⭐
void square_step(std::vector<std::vector<int>> &map, int chunk_size, int half, int w, int h, int roughness)
{
    for (int y = 0; y < h - 1; y += chunk_size)
    {
        for (int x = 0; x < w - 1; x += chunk_size)
        {
            int one = map[y][x];                            // haut gauche
            int two = map[y][x + chunk_size];               // haut droite
            int three = map[y + chunk_size][x];             // bas gauche
            int four = map[y + chunk_size][x + chunk_size]; // bas droite

            int center = (one + two + three + four) / 4 + random_int(-roughness, roughness);
            map[y + half][x + half] = center; // centre du carré
        }
    }
}

void diamond_step(std::vector<std::vector<int>> &map, int chunk_size, int half, int w, int h, int roughness)
{
    for (int y = 0; y < h; y += half)
    {
        for (int x = (y / half) % 2 == 0 ? half : 0; x < w; x += chunk_size)
        {
            int sum = 0;
            int count = 0;

            // haut
            if (y - half >= 0)
            {
                sum += map[y - half][x];
                count++;
            }
            // bas
            if (y + half < h)
            {
                sum += map[y + half][x];
                count++;
            }
            // gauche
            if (x - half >= 0)
            {
                sum += map[y][x - half];
                count++;
            }
            // droite
            if (x + half < w)
            {
                sum += map[y][x + half];
                count++;
            }

            int val = sum / count + random_int(-roughness, roughness);
            map[y][x] = val;
        }
    }
}

void diamond_square(sil::Image &image)
{
    int w = image.width();
    int h = image.height();

    std::vector<std::vector<int>> map(h, std::vector<int>(w, 0));

    // SETUP: initialiser les 4 coins
    map[0][0] = random_int(5, 20);         // haut gauche
    map[0][w - 1] = random_int(5, 20);     // haut droite
    map[h - 1][0] = random_int(5, 20);     // bas gauche
    map[h - 1][w - 1] = random_int(5, 20); // bas droite

    int chunk_size = w - 1;
    int roughness = 15;

    while (chunk_size > 1)
    {
        int half = chunk_size / 2;
        square_step(map, chunk_size, half, w, h, roughness);
        diamond_step(map, chunk_size, half, w, h, roughness);

        chunk_size /= 2;
        roughness = std::max(1, roughness / 2); // diminue progressivement
    }

    // Normaliser pour passer en glm::vec3
    int max_val = 0;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            max_val = std::max(max_val, map[y][x]);
        }
    }

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float val = map[y][x] / float(max_val);
            image.pixel(x, y) = glm::vec3(val);
        }
    }
}

int main()
{
    // {
    //     sil::Image image{"images/logo.png"};      // Lis l'image
    //     keep_green_only(image);                   // Utilise la fonction pour modifier l'image
    //     image.save("output/keep_green_only.png"); // Sauvegarde l'image
    // }
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
    {
        sil::Image image{600 /*width*/, 600 /*height*/};
        rosace(image);
        image.save("output/rosace.png");
    }
    // {
    //     sil::Image image{"images/logo.png"};
    //     mosaique(image);
    //     image.save("output/mosaique.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     mosaique_mirror(image);
    //     image.save("output/mosaique_mirror.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     glitch(image);
    //     image.save("output/glitch.png");
    // }
    // {
    //     sil::Image image{300 /*width*/, 200 /*height*/};
    //     color_gradient(image);
    //     image.save("output/color_gradient.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     pixel_sorting(image);
    //     image.save("output/pixel_sorting.png");
    // }
    // {
    //     sil::Image image{"images/photo.jpg"};
    //     tramage(image);
    //     image.save("output/tramage.jpg");
    // }
    // {
    //     sil::Image image{"images/photo_faible_contraste.jpg"};
    //     normalisation(image);
    //     image.save("output/normalisation.jpg");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     convolutions(image);
    //     image.save("output/convolutions.png");
    // }
    // {
    //     sil::Image image{"images/logo.png"};
    //     vortex(image);
    //     image.save("output/vortex.png");
    // }
    // {
    //     sil::Image image{"images/photo.jpg"};
    //     kuwahara(image);
    //     image.save("output/kuwahara.jpg");
    // }
    {
        sil::Image image{129 /*width*/, 129 /*height*/}; // valeur doit être 2^n + 1
        diamond_square(image);
        image.save("output/diamond_square.png");
    }
}