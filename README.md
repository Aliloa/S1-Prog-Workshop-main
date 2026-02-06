# RAPPORT
#### ⭐ Ne garder que le vert - `keep_green_only()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/keep_green_only.png"> |


---
#### ⭐ Échanger les canaux - `swap_canals()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/swap_canals.png"> |

---

#### ⭐ Noir & Blanc - `black_and_white()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/black_and_white.png"> |

---

#### ⭐ Négatif - `negatif()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/negatif.png"> |

---

#### ⭐ Dégradé - `gradient()`

| Avant | Après |
|-------|-------|
| / / / / | <img src="output/gradient.png"> |

---

#### ⭐⭐ Miroir - `mirror()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/mirror.png"> |

---

#### ⭐⭐ Image bruitée - `noise()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/noise.png"> |

---

#### ⭐⭐ Rotation de 90° - `rotate()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/rotate.png"> |

---

#### ⭐⭐ RGB split - `split()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/split.png"> |

---

#### ⭐⭐ Luminosité - `lighting()`

| Avant | Après |
|-------|-------|
| <img src="images/photo_faible_contraste.jpg"> | <img src="output/lighting.png"> |

---

#### ⭐⭐(⭐) Disque - `draw_circle()`

| Avant | Après |
|-------|-------|
| / / / / | <img src="output/circle.png" width="300px"> |

---

#### ⭐ Cercle (vide) - `draw_empty_circle()`

| Avant | Après |
|-------|-------|
| / / / / | <img src="output/empty_circle.png" width="300px"> |

---

#### ⭐⭐ Animation - `animation()`

| Avant | Après |
|-------|-------|
| / / / / | <img src="output/animation.gif" width="300px"> |

---

#### ⭐⭐⭐ Rosace - `rosace()`

| Avant | Après |
|-------|-------|
| / / / / | <img src="output/rosace.png" width="300px"> |

---

#### ⭐⭐ Mosaïque - `mosaique()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/mosaique.png"> |

_Pour cet exercice j'ai pris 1 pixel sur 5 de l'image originale pour les placer à la suite ce qui a rétréci l'image, et ensuite j'ai dupliqué_

---

#### ⭐⭐⭐⭐ Mosaïque miroir - `mosaique_mirror()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/mosaique_mirror.png"> |

---

#### ⭐⭐⭐ Glitch - `glitch()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/glitch.png"> |

---

#### ⭐⭐⭐ Tri de pixels - `pixel_sorting()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/pixel_sorting.png"> |

_Dans cet exercice j'ai eu du mal à trouver comment l'image originale a été faite.
J'ai crée un tableau qui trie les pixels de chaque ligne indépendament, ensuite j'ai utilisé la fonction `random_int` pour appliquer l'effet à 1 ligne sur 2. Et enfin j'ai fait en sorte que une seule portion de la ligne qui soit triée_

---

#### ⭐⭐⭐(⭐) Dégradés dans l'espace de couleur Lab - `color_gradient()`

| Avant | Après |
|-------|-------|
| <img src="output/gradient.png"> | <img src="output/color_gradient.png"> |

_J'ai beaucoup aimé la vidéo minecraft pour expliquer le sujet_

---

#### ⭐⭐⭐(⭐) Tramage - `tramage()`

| Avant | Après |
|-------|-------|
| <img src="images/photo.jpg"> | <img src="output/tramage.jpg"> |

_J'ai lu l'article **Ditherpunk** en entier et je l'ai trouvé super intéressant!
J'ai réutilisé la matrice de Bayer et appliqué le principe Ordered Dithering. Comme l'image était claire, j'ai pas inversé le seuil `(1 - Bayer value)`, comme l'auteur de l'article_

---

#### ⭐⭐⭐(⭐) Normalisation de l'histogramme - `normalisation()`

| Avant | Après |
|-------|-------|
| <img src="images/photo_faible_contraste.jpg"> | <img src="output/normalisation.jpg"> |

---

#### ⭐⭐⭐⭐ Vortex - `vortex()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/vortex.png"> |

---

#### ⭐⭐⭐⭐ Convolutions - `convolutions()`

| Avant | Après |
|-------|-------|
| <img src="images/logo.png"> | <img src="output/convolutions.png"> |

---

#### ⭐⭐⭐⭐⭐ Filtre de Kuwahara (effet peinture à l'huile) - `kuwahara()`

| Avant | Après | Avec l'effet noise |
|-------|-------|-------|
| <img src="images/photo.jpg"> | <img src="output/kuwahara.jpg"> | <img src="output/kuwahara_with_noise.jpg"> |

_Au début j'ai cru qu'il fallait obligatoirement appliquer l'effet `noise` pour que ça fonctionne, à cause la vidéo donnée dans l'exercice, et l'effet était bizarre mais après j'ai compris que c'était pas nécessaire_

---