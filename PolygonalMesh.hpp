#pragma once

#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonLibrary{

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0; // Numero di righe del file Cell0Ds.csv
    std::vector<unsigned int> Cell0DId = {}; // Vettore di ID delle celle 0D
    std::vector<Vector2d> Cell0DCoordinates = {}; // Vettore delle coordinate delle celle 0D
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; // Mappa dei marker delle celle 0D

    unsigned int NumberCell1D = 0; // Numero di righe del file Cell1Ds.csv
    std::vector<unsigned int> Cell1DId = {}; // Vettore di ID delle celle 1D
    std::vector<Vector2i> Cell1DVertices = {}; // Vettore dei vertici delle celle 1D
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; // Mappa dei marker delle celle 1D

    unsigned int NumberCell2D = 0; // Numero di righe del file Cell2Ds.csv
    std::vector<unsigned int> Cell2DId = {}; // Vettore di ID delle celle 2D
    std::vector<vector<unsigned int>> Cell2DVertices = {}; // Vettore dei vertici delle celle 2D
    std::vector<vector<unsigned int>> Cell2DEdges = {}; // Vettore degli spigoli delle celle 2D
    std::map<unsigned int, list<unsigned int>> Cell2DMarkers = {}; // Mappa dei marker delle celle 2D
};


}
