#ifndef MESH_H
#define MESH_H

// #include <QGLWidget> //UpgradeQt6: #include <QOpenGLWidget>
#include <QOpenGLWidget>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QList>
#include <QPair>
#include <iterator>



// TO MODIFY
class Point
{
public:
    double _x;
    double _y;
    double _z;

    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
};

class Face
{
public:
    int vertices[3];
    int oppositeFaces[3]; // Sauvegarde la face opposée à un sommet

    Face(int v, int v2, int v3) {
        vertices[0] = v;
        vertices[1] = v2;
        vertices[2] = v3;
    }

    // Ajouter une fonction qui renvoie le sommet suivant ? (pour remplir la map)
    int getNextVertex(int v);
    /** Retourne une paire d'id équivalent au 2 sommets communs aux 2 faces, une parie nulle sinon **/
    std::pair<int,int> adjacentFace(Face f);
    /** Retourne l'id dans le tableau de faces opposées selon une valeur de face **/
    int findOppositeFace(int f);
    ~Face();

};

struct Laplacian
{
public:
    double l[3];
};


class Iterator_on_faces
{
public:
    Iterator_on_faces(Face *face) : p(face) {}
    Iterator_on_faces(const Iterator_on_faces &it) : p(it.p) {} // Constructeur par copie
    Iterator_on_faces& operator++() { ++p; return *this; }
    Iterator_on_faces& operator--() { --p; return *this; }
    bool operator== (Iterator_on_faces &it) const { return p == it.p; }
    bool operator!= (Iterator_on_faces &it) const { return p != it.p; }
    Face& operator*() { return *p; }
    ~Iterator_on_faces() { }

private:
    Face *p;
};

class Iterator_on_vertices {
public:
    Iterator_on_vertices(Point *vertex) : v(vertex) {}
    Iterator_on_vertices(const Iterator_on_vertices &it) : v(it.v) {} // Constructeur par copie
    Iterator_on_vertices& operator++() { ++v; return *this; }
    Iterator_on_vertices& operator--() { --v; return *this; }
    bool operator== (Iterator_on_vertices &it) const { return v == it.v; }
    bool operator!= (Iterator_on_vertices &it) const { return v != it.v; }
    Point& operator*() { return *v; }
    ~Iterator_on_vertices() { }

private:
    Point *v;
};



// Itérer sur les faces incidentes à un sommet dans le sens trigonométrique
class Circulator_on_faces {
public:
    Circulator_on_faces() {}
    //Face& operator*() { return Face(0,0,0); } // Retourner l'adresse d'une face
    Face operator++() {return Face(0,0,0);} // La face incidente suivante
};

//** TP : TO MODIFY

class Mesh
{
private:
    // (Q ou STL)Vector of vertices
    QVector<Point> vertices;
    // (Q ou STL)Vector of faces
    QVector<Face> faces;
    // (Q ou STL)Vector of faces
    QVector<Laplacian> Laplacians;
  // Those who do not know about STL Vectors should have a look at cplusplus.com examples
public:
    // Constructors automatically called to initialize a Mesh (default strategy)
    Mesh();
    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void drawMesh();
    void drawMeshWireFrame();
    void buildMap();
    void addVertex(Point p);
    bool aireSignee(Face f, Point p);
    bool insideFace(Point p, Point p1, Point p2);    
    void edgeFlip(unsigned int f1, unsigned int f2);
    void calculLaplacian(int sId);
    double aire(int sId);
    Iterator_on_faces faces_begin() { return Iterator_on_faces(&faces[0]); }
    Iterator_on_faces faces_past_the_end() { return Iterator_on_faces(faces.end()); }
    Iterator_on_vertices vertices_begin() { return Iterator_on_vertices(&vertices[0]); }
    Iterator_on_vertices vertices_past_the_end() { return Iterator_on_vertices(vertices.end()); }
    // incident_faces(Sommet & v);
    void oppositeFaces(QMap<QPair<int, int>, QPair<int, int>> map); // Créer un tableau des faces opposées à chaque sommet
};

class GeometricWorld //Here used to create a singleton instance
{
  QVector<Point> _bBox;  // Bounding box
public :
  GeometricWorld();
  void draw();
  void drawWireFrame();
  // ** TP Can be extended with further elements;
  Mesh _mesh;
};


#endif // MESH_H
