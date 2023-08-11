#include <iostream>
#include <QApplication>
#include <QDir>
#include "mesh.h"

GeometricWorld::GeometricWorld()
{
    _mesh = Mesh();
    double width=0.5, depth=0.6, height=0.8;
    _bBox.push_back(Point(-0.5*width,-0.5*depth,-0.5*height)); //0
    _bBox.push_back(Point(-0.5*width,0.5*depth,-0.5*height)); // 1
    _bBox.push_back(Point(0.5*width,-0.5*depth,-0.5*height)); // 2
    _bBox.push_back(Point(-0.5*width,-0.5*depth,0.5*height)); // 3
    qDebug() << "init \n";
}

// The following functions could be displaced into a module OpenGLDisplayGeometricWorld that would include mesh.h

// Draw a Point
void glPointDraw(const Point & p) {
    glVertex3f(p._x, p._y, p._z);
}

//Example with a bBox
void GeometricWorld::draw() {
    _mesh.drawMesh();
    /*glColor3d(1,0,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glPointDraw(_bBox[2]);
    glEnd();

    glColor3d(0,1,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glPointDraw(_bBox[3]);
    glEnd();

    glColor3d(0,0,1);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glPointDraw(_bBox[1]);
    glEnd();

    glColor3d(1,1,0);*/

}

//Example with a wireframe bBox
void GeometricWorld::drawWireFrame() {
    glColor3d(0,1,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glEnd();
    glColor3d(0,0,1);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glEnd();
    glColor3d(1,0,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glEnd();
}

Face::~Face() {}
int Face::getNextVertex(int v) {
    if (v < 2)
        return vertices[v++];
    return vertices[0];
}

std::pair<int,int> Face::adjacentFace(Face f) {
    int s1 = -1, s2 = -1;
    for(int v : vertices) {
        for(int i=0 ; i<3 ; i++) {
            if (v == f.vertices[i]) {
                if(s1 >= 0) {
                    s2 = i;
                }
                else {
                    s1 = i;
                }
            }
        }
    }
    if (s2 >= 0)
        return std::make_pair(s1,s2);

    return std::make_pair(NULL,NULL);
}


Mesh::~Mesh() {}

Mesh::Mesh() {
    // Load a mesh from a .off file
    vertices = {};
    faces = {};
    // Lire le fichier off
    // QFile file("/home/judith/Documents/M2/GAM/GAM2022_Seance1/queen.off");
    QFile file(QDir::homePath() +"/Documents/M2INFO/GEOALGO/GAM2022_Seance1/queen.off");
    //QFile file(QDir::homePath() +"/Documents/M2/GAM/GAM2022_Seance1/queen.off");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    qDebug() << "after file opening\n";

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList data = line.split(" ");
        bool ok;
        if(data.size() == 3) {
            vertices.append(Point(data.at(0).toDouble(), data.at(1).toDouble(), data.at(2).toDouble()));
        } else if( data.size() == 4) {
            faces.append(Face(data.at(1).toInt(&ok, 10), data.at(2).toInt(&ok, 10), data.at(3).toInt(&ok, 10)));
        }
    }

    // Supprimer le premier élément du vecteur (qui n'est qu'informatif et pas un sommet)
    vertices.pop_front();


    // 2 faces
    /*vertices.push_back(Point(0,0,0)); // 0
    vertices.push_back(Point(0,1,0)); // 1
    vertices.push_back(Point(1,0,0)); // 2
    faces.push_back(Face(0,1,2));

    vertices.push_back(Point(1,0,0)); // 3
    vertices.push_back(Point(1,1,0)); // 4
    vertices.push_back(Point(0,1,0)); // 5
    faces.push_back(Face(3,4,5));*/

}


void Mesh::drawMesh() {
    // Mesh::buildMap();
    addVertex(Point(0.5,0.2,0));
    for (int i = 0; i < faces.size(); i++) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Dessiner en wireframe
        glColor3d(1,0,0);
        glBegin(GL_TRIANGLES);
        for (int j = 0; j < 3; j++) {
            glPointDraw(vertices[faces[i].vertices[j]]);
        }
        glEnd();
    }
}

bool Mesh::aireSignee(Face f, Point p) {
    Point p0 = vertices[f.vertices[0]];
    Point p1 = vertices[f.vertices[1]];
    Point p2 = vertices[f.vertices[2]];
    return insideFace(p, p0, p1) && insideFace(p, p1, p2) && insideFace(p, p2, p0);
}

bool Mesh::insideFace(Point p, Point p1, Point p2) {
    return (p1._x - p._x)*(p2._y - p._y) > (p2._y - p._y)*(p1._y - p._y);
}

void Mesh::addVertex(Point p) {
    // on trouve la face dans laquelle le point a été ajouté (avec intersection et distance)
    // Regarder aire signée de chaque arrête avec p

    // Sinon faire cas naif

    // on ajoute ce point au vector vertices
    // on modifie la face courante (changement id sommets)
    // on ajoute les 2 autres faces dans vector faces

    // Version naive


    Face f = faces[0]; // qui contient le point p
    vertices.push_back(p);
    int id_p = vertices.count() - 1;

    int id_s = f.vertices[0]; // id de l'ancien sommet à la position 0 dans la face
    f.vertices[0] = id_p; // remplacé par l'id du nouveau sommet


    faces.push_back(Face(id_p, id_s, f.vertices[1])); // Création d'une nouvelle face avec le nouveau sommet
    faces.push_back(Face(id_p, f.vertices[2], id_s)); // Création de la face adjacente ?

}


void Mesh::oppositeFaces(QMap<QPair<int, int>, QPair<int, int>> map) {
    QList<QPair<QPair<int, int>, int>> arretes = {qMakePair(qMakePair(0,1), 2),
                                                  qMakePair(qMakePair(0,2), 1),
                                                  qMakePair(qMakePair(1,2), 0)};

    // Pour chaque face du vecteur
    for(int i = 0; i < faces.size(); i++) {
        Face f = faces[i];
        // Pour chaque arète de la face
        for (int j = 0; j < 3; j++) {
            // 1. On vérifie dans quel sens l'arète est stockée dans la map
            // 2. Pour le sommet opposé à l'arète de la face courante :
            // on stocke à l'indice j la face opposée du sommet de rang j dans la face courante

            if (map.contains(qMakePair(arretes.at(j).first.first, arretes.at(j).first.second)))
                f.oppositeFaces[arretes.at(j).second] = map.value(qMakePair(arretes.at(j).first.first, arretes.at(j).first.second)).second;
            else
                f.oppositeFaces[arretes.at(j).second] = map.value(qMakePair(arretes.at(j).first.second, arretes.at(j).first.first)).second;
        }
    }

    //qDebug() << "id sommet : " << faces[111].vertices[0] << " id de la face opposée : " << faces[111].oppositeFaces[0] << "\n";

}



void Mesh::buildMap() {
    //key = pair of vertex indexes
    //value = index of the current face + the relative index of the vertex opposite to the edge in the face
    QMap<QPair<int, int>, QPair<int, int>> map;

    QList<QPair<QPair<int, int>, int>> arretes = {qMakePair(qMakePair(0,1), 2),
                                                  qMakePair(qMakePair(0,2), 1),
                                                  qMakePair(qMakePair(1,2), 0)};

    // pour chaque face
    // prendre une paire d'arrêtes
    // vérifier si elle n'existe pas déjà dans la map (+ checker l'autre sens de l'arrête)
    // Sinon remplir la map avec la face d'où on sort + voir si on trouve la face opposée (dans ce cas, remplir la seconde value avec l'id 0,1,2)
    // Si l'arète existe déjà la valeur est la face opposée au sommet opposé à l'arète en cours (faces adjacentes)

    // Boucle sur les faces
    for(int j = 0; j < faces.size(); j++) {
        // La face courante
        Face f = faces[j];
        // Boucle sur les cas possible d'arètes dans la face j
        for(int i=0 ; i<3 ; i++) {
            // Si l'arète n'existe pas dans la map
            if(!map.contains(qMakePair(f.vertices[arretes.at(i).first.first], f.vertices[arretes.at(i).first.second]))
                && !map.contains(qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first]))) {
                // On ajoute Map[<0, 1>] = <j,2>
                map[qMakePair(f.vertices[arretes.at(i).first.first], f.vertices[arretes.at(i).first.second])] = qMakePair(j, f.vertices[arretes.at(i).second]);
            } else {
                // Si l'arète existe déjà dans la map (dans un sens ou dans l'autre) : ajouter l'id de la face opposée de son sommet opposé
                if (map.contains(qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first]))) {
                    // Ancienne version :
                    // Si l'arète existe déjà : récupérer le sommet suivant de cette arète dans la face courante
                    // map[qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first])].second = f.getNextVertex( f.vertices[arretes.at(i).first.first] );

                    // On stocke l'id de la face adjacente (l'id de la face opposée au sommet opposé à l'arète) : l'id de la face courante
                    map[qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first])].second = j;
                    qDebug() << map[qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first])] <<"\n";
                } else if (map.contains(qMakePair(f.vertices[arretes.at(i).first.first], f.vertices[arretes.at(i).first.second]))) {
                    // Ancienne version :
                    // Si l'arète existe déjà : récupérer le sommet suivant de cette arète dans la face courante (dans l'autre sens)
                    // map[qMakePair(f.vertices[arretes.at(i).first.first], f.vertices[arretes.at(i).first.second])].second = f.getNextVertex( f.vertices[arretes.at(i).first.second] );

                    // On stocke l'id de la face adjacente (l'id de la face opposée au sommet opposé à l'arète) : l'id de la face courante
                    qDebug() << map[qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first])] <<"\n";
                    map[qMakePair(f.vertices[arretes.at(i).first.first], f.vertices[arretes.at(i).first.second])].second = j;
                    qDebug() << map[qMakePair(f.vertices[arretes.at(i).first.second], f.vertices[arretes.at(i).first.first])] <<"\n";
                }
            }
        }
    }

    oppositeFaces(map);
}

int Face::findOppositeFace(int f) {
    return oppositeFaces[0] == f ? 0 :
           oppositeFaces[1] == f ? 1 :
           oppositeFaces[2] == f ? 2 : -1;
}

void Mesh::edgeFlip(unsigned int idf1, unsigned int idf2) {
    // On récupère les 2 faces
    auto &f1 = faces[idf1];
    auto &f2 = faces[idf2];

    // id dans les tableaux des sommets de l'arrête
    int s1 = f1.findOppositeFace(idf2);
    int s2 = f2.findOppositeFace(idf1);
    if(s1 < 0 || s2 < 0) return;


    // on récupère les 2 autres positions pour les 2 faces
    int s1i = (s1 + 1) % 3, s1ii = (s1 + 2) % 3;
    int s2i = (s2 + 1) % 3, s2ii = (s2 + 2) % 3;

    // changer les sommets
    f1.vertices[s1i] = f2.vertices[s2];
    f2.vertices[s2i] = f1.vertices[s1];

    // changer les faces opposées
    f1.oppositeFaces[s1] = f2.oppositeFaces[s2ii];
    f2.oppositeFaces[s2] = f1.oppositeFaces[s1ii];
    f1.oppositeFaces[s1ii] = idf2;
    f2.oppositeFaces[s2ii] = idf1;

    int sa = faces[f1.oppositeFaces[s1]].findOppositeFace(idf2);
    int sb = faces[f2.oppositeFaces[s2]].findOppositeFace(idf1);
    faces[f1.oppositeFaces[s1]].oppositeFaces[sa] = idf1;
    faces[f2.oppositeFaces[s2]].oppositeFaces[sb] = idf2;
}


double Mesh::aire(int sId){
    double aire = 0;
    Point s = vertices[sId];


    // trouver toutes les faces contenant s
    // auto faces = findFaces(s); // renvoie un circulator ?
    // auto facesBegin = faces;
    do {
        // Récupérer le barycentre de la face i ?
        // Point P, Q, R les 3 points de la face
        // aire += ((Q-P)%(R-P)).norm() * 0.5f * 0.3333f; // 0.333 = 1/3
        //

    } while (0/* ++faces != facesBegin */);
    return aire;
}

void Mesh::calculLaplacian(int sId){
    // Formule pour le sommet i => (Somme(cota + cot b)(uj - ui)) / 2Ai;

    double Ai = 2*aire(sId);
    Laplacian laplacian;

    // trouver points adjacents au sommet
    do {
        Point p;
        // faire la somme
        // Somme(cot a + cot b)(uj - ui)
    } while (0/* pour tous les points adjacents */);

    Laplacians[sId] = laplacian;

}
