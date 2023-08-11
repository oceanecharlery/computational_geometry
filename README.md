# Géométrie Algorithmique et Maillages
## Océane CHARLERY, Judith MILLET


Ce projet peut être lancé de deux manières :
- En générant un maillage à partir d'un couple de triangles : plus approprié pour le tests des fonctions de triangulation
- En créant le maillage à partir d'un fichier .off

### Mesh
Un mesh contient comme attributs un tableau de faces, un tableau de sommets (Points) et un tableau de laplacian.
Une face inclut un tableau de 3 ids de sommets et un tableau d'ids de faces opposées à ces sommets dans l'ordre des sommets.
Un Laplacian est un tableau de 3 doubles.

### Fonctions

- insideFace() : Déterminer si un point est à l'intérieur d'un triangle.
- aireSignee() : Déterminer l'orientation d'un point par rapport à un triangle.
- addVertex() : Ajout d'un sommet dans un triangle -> triangulation. Permet de split une face en trois faces tout en gardant les caractéristiques d'un maillage correct.
- oppositeFaces() : Créer pour chaque face un array contenant les faces opposées à chaque sommet de la face courante. Va permettre de "se déplacer" dans le mesh. 
- buildMap() : Création d'une map contenant
	- clé : arète 
	- valeur : l'id de la face contenant l'arète et l'id de la face opposée au sommet opposé à l'arète
- edgeFlip(f1Id,f2Id) : Permet de flip l'arête entre la face f1 et la face f2.
- calculLaplacian(sId) : Permet de calculer le laplacian du sommet s.

### Itérateurs
- Circulateur (pas encore fonctionnel)
- Itérateur de face (bidirectionnel)
	- Un constructeur par copie
	- Surcharge de l'opérateur ++ 
	- Surcharge de l'opérateur --
	- Surcharge de l'opérateur ==
	- Surcharge de l'opérateur !=
	- Surcharge de l'opérateur *
- Itérateur de sommets (bidirectionnel)
	- Un constructeur par copie
	- Surcharge de l'opérateur ++ 
	- Surcharge de l'opérateur --
	- Surcharge de l'opérateur ==
	- Surcharge de l'opérateur !=
	- Surcharge de l'opérateur *

La classe Mesh offre également des fonctions permettant d'instancier un itérateur pointant soit sur le premier, soit sur la case mémoire suivante au dernier élément de ses vecteurs de sommets et de faces.
