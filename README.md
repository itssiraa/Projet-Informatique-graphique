# Rapport de Projet – Scène 3D  
## *Bateau sur l’eau*

### Objectif
L’objectif du projet était de concevoir une **scène 3D interactive réaliste**, à l’aide de **modélisation géométrique**, **texturation**, **éclairage** et **animation**.  
Le thème choisi est la représentation d’un **bateau naviguant sur l’eau**, intégré dans un environnement immersif.

---

### Structure et Modélisation

La scène est construite autour d’un **arbre de transformation hiérarchique**.  
Chaque partie du bateau est modélisée comme un nœud enfant de la **coque principale**, qui sert de **pivot central** pour toutes les transformations globales (rotation, translation).

#### Composants du bateau :
- **Coque** → réalisée avec une classe `Pyramid` personnalisée
- **Bords latéraux** → modélisés via la classe `Bords` (forme personnalisée)
- **Mâts et rambardes** → créés avec la classe `Cylinder`
- **Voiles** → formes triangulaires (`Triangle`)
- **Finitions (dômes, détails)** → `Demi_Sphere`

Tous ces objets dérivent d’une classe `Shape` commune, permettant une gestion uniforme du rendu.

---

### Texturation

Deux textures bois (`bois.jpg`, `bois1.jpg`) sont utilisées pour améliorer l’esthétique du bateau.  
Les shaders `texture.vert` et `texture.frag` prennent en charge le mappage UV, appliqué dans chaque forme via des coordonnées spécifiques.

---

### Animation et Interaction

L’animation du bateau repose sur la classe `BoatAnimator`. Elle gère :
- Le **déplacement du bateau dans la direction de son orientation**
- Les **rotations latérales** via les flèches gauche/droite
- Le **contrôle du mouvement** (pause/reprise) avec la barre d’espace
- Un **effet de tangage** du bateau (oscillation gauche/droite) pour simuler la houle

> Remarque : la rotation s’effectue autour d’un pivot situé à l’arrière du bateau, mais l’approximation de certaines transformations peut donner une impression de décalage.

---

### Caméra

Deux caméras ont été mises en place :
- **Caméra statique** par défaut, positionnée à l’arrière du bateau
- **Caméra orbitale** (mode activé au clic gauche), permettant :
  - Une rotation autour du bateau (drag souris)
  - Un zoom via la molette
  - Un retour au mode précédent via la touche `O`

Bien que fonctionnelles, ces caméras peuvent encore être améliorées :
- La caméra statique ne suit pas dynamiquement le bateau
- La caméra orbitale reste centrée sur le point d’origine du bateau

---

### Skybox et Environnement

L’arrière-plan est réalisé grâce à une **skybox cubique**, constituée de six textures (`right.jpg`, `left.jpg`, `top.jpg`, etc.).  
Elle est rendue avec un shader dédié (`skybox.vert` / `skybox.frag`) et désactivée du depth test standard (`GL_LEQUAL`) pour garantir son affichage permanent à l’arrière-plan.

---

### Océan réaliste

L’eau est simulée grâce à la classe `Water`, implémentée à partir d’un shader inspiré du projet open source [Ocean-OpenGL](https://github.com/stanislawfortonski/Ocean-OpenGL).  
Ce composant introduit :
- Une **géométrie plane animée** représentant la mer
- Un shader dynamique (`water.vert`, `water.frag`) pour simuler les **vagues**
- Une **lumière directionnelle** (type soleil) pour générer des reflets et nuances
- Des textures mouvantes qui accentuent l’illusion d’une **mer vivante**

Ce rendu permet une **intégration fluide avec la skybox**, renforçant l’immersion globale de la scène.

---

### Conclusion

Ce projet nous a permis d’explorer et de combiner plusieurs aspects fondamentaux de l’infographie 3D moderne :
- Modélisation hiérarchique par nœuds
- Application de textures mappées
- Animation contrôlée par clavier
- Caméras interactives
- Simulation dynamique d’un environnement réaliste

Il illustre l’ensemble des compétences abordées en cours, tout en laissant place à de futures améliorations (objets dynamiques supplémentaires, meilleures interactions caméra, etc.).
