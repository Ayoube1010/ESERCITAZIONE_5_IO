#include "Utils.hpp"
#include<iostream>
#include<fstream>
#include<sstream>
#include"Eigen/Eigen"
#include<vector>


namespace PolygonLibrary {

bool ImportMesh(PolygonalMesh& mesh)
{
    cout << "Importing Cell0Ds..." << endl;
    if(!ImportCell0Ds(mesh))
    {
        cerr << "Failed to import Cell0Ds" << endl;
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it->first << "\t values:";
            for(const unsigned int id : it->second)
            {
                cout << "\t" << id;
            }
            cout << endl;
        }
    }

    cout << "Importing Cell1Ds..." << endl;
    if(!ImportCell1Ds(mesh))
    {
        cerr << "Failed to import Cell1Ds" << endl;
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it->first << "\t values:";
            for(const unsigned int id : it->second)
            {
                cout << "\t" << id;
            }
            cout << endl;
        }
    }

    cout << "Importing Cell2Ds..." << endl;
    if(!ImportCell2Ds(mesh))
    {
        cerr << "Failed to import Cell2Ds" << endl;
        return false;
    }
    else
    {
        cout << "Cell2D marker:" << endl;
        for(auto it = mesh.Cell2DMarkers.begin(); it != mesh.Cell2DMarkers.end(); it++)
        {
            cout << "key:\t" << it->first << "\t values:";
            for(const unsigned int id : it->second)
            {
                cout << "\t" << id;
            }
            cout << endl;
        }

        for(unsigned int c = 0; c < mesh.NumberCell2D; c++) // Per ogni cella 2D
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c]; // La componente c-esima del vettore Cell2DEdges contiene il vettore di edges della cella c-esima
            unsigned int ne = edges.size(); // Numero di edges nella c-esima cella

            for(unsigned int e = 0; e < ne; e++) // Scorre tutti gli edges della c-esima cella 2D
            {
                const unsigned int edgeIndex = edges[e]; // Indice dello spigolo
                if (edgeIndex >= mesh.NumberCell1D)
                {
                    cerr << "Wrong mesh: invalid edge index" << endl;
                    return false;
                }

                const unsigned int origin = mesh.Cell1DVertices[edgeIndex][0]; // Vertice di origine
                const unsigned int end = mesh.Cell1DVertices[edgeIndex][1]; // Vertice di fine

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin); // Cerca origin nella lista dei vertici della c-esima cella 2D
                if(findOrigin == mesh.Cell2DVertices[c].end()) // Se non trovato
                {
                    cerr << "Wrong mesh: origin vertex not found in Cell2D vertices" << endl;
                    return false;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh: end vertex not found in Cell2D vertices" << endl;
                    return false;
                }

                // Verifica che gli spigoli abbiano lunghezza diversa da zero
                if (!CheckEdgeLengthNonZero(mesh.Cell0DCoordinates[origin], mesh.Cell0DCoordinates[end]))
                {
                    cerr << "Wrong mesh: edge with zero length found between vertices (" << origin << ", " << end << ")" << endl;
                    return false;
                }
            }

            // Verifica che l'area dei triangoli sia diversa da zero
            if (!CheckTriangleAreaNonZero(mesh.Cell2DVertices[c], mesh.Cell0DCoordinates))
            {
                cerr << "Wrong mesh: triangle with zero area found in Cell2D with ID: " << mesh.Cell2DId[c] << endl;
                return false;
            }
        }
    }
    return true;
}

// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./PolygonalMesh/Cell0Ds.csv");

    if(file.fail())
    {
        cerr << "Failed to open Cell0Ds.csv" << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); // Rimuove l'intestazione

    mesh.NumberCell0D = listLines.size(); // Numero di righe

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >> id >> marker >> coord(0) >> coord(1);

        cout << "Read Cell0D - ID: " << id << ", Marker: " << marker << ", Coordinates: (" << coord(0) << ", " << coord(1) << ")" << endl;

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);
        mesh.Cell0DMarkers[marker].push_back(id);
    }

    return true;
}

// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./PolygonalMesh/Cell1Ds.csv");

    if(file.fail())
    {
        cerr << "Failed to open Cell1Ds.csv" << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); // Rimuove l'intestazione

    mesh.NumberCell1D = listLines.size(); // Numero di righe

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >> id >> marker >> vertices(0) >> vertices(1);

        cout << "Read Cell1D - ID: " << id << ", Marker: " << marker << ", Vertices: (" << vertices(0) << ", " << vertices(1) << ")" << endl;

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);
        mesh.Cell1DMarkers[marker].push_back(id);
    }

    return true;
}

// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./PolygonalMesh/Cell2Ds.csv");

    if(file.fail())
    {
        cerr << "Failed to open Cell2Ds.csv" << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); // Rimuove l'intestazione

    mesh.NumberCell2D = listLines.size(); // Numero di righe

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int numVertices;
        unsigned int numEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        converter >> id >> marker >> numVertices >> numEdges;

        for (unsigned int i = 0; i < numVertices; i++)
        {
            unsigned int vertex;
            converter >> vertex;
            vertices.push_back(vertex);
        }

        // Leggiamo l'ultimo spigolo separatamente
        unsigned int lastEdge;
        converter >> lastEdge;
        edges.push_back(lastEdge);

        for (unsigned int i = 0; i < numEdges - 1; i++) // Sottraiamo 1 per l'ultimo spigolo già letto
        {
            unsigned int edge;
            converter >> edge;
            edges.push_back(edge);
        }

        cout << "Read Cell2D - ID: " << id << ", Marker: " << marker << ", Vertices: ";
        for (unsigned int v : vertices)
        {
            cout << v << " ";
        }
        cout << ", Edges: ";
        for (unsigned int e : edges)
        {
            cout << e << " ";
        }
        cout << endl;

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
        mesh.Cell2DMarkers[marker].push_back(id);
    }

    return true;
}

// Verifica che gli spigoli dei triangoli abbiano lunghezza diversa da zero
bool CheckEdgeLengthNonZero(const Vector2d& vertex1, const Vector2d& vertex2)
{
    double length = sqrt(pow(vertex2(0) - vertex1(0), 2) + pow(vertex2(1) - vertex1(1), 2));
    cout << "Checking edge length between vertices (" << vertex1(0) << ", " << vertex1(1) << ") and (" << vertex2(0) << ", " << vertex2(1) << "): " << length << endl;
    return length > 0;
}

// Verifica che l'area dei triangoli sia diversa da zero
bool CheckTriangleAreaNonZero(const vector<unsigned int>& vertices, const vector<Vector2d>& coordinates)
{
    if (vertices.size() != 3)
    {
        cerr << "Only triangles are supported in this check." << endl;
        return false;
    }

    Vector2d A = coordinates[vertices[0]];
    Vector2d B = coordinates[vertices[1]];
    Vector2d C = coordinates[vertices[2]];

    double area = 0.5 * fabs(A(0)*(B(1) - C(1)) + B(0)*(C(1) - A(1)) + C(0)*(A(1) - B(1)));
    cout << "Checking triangle area with vertices (" << A(0) << ", " << A(1) << "), (" << B(0) << ", " << B(1) << "), (" << C(0) << ", " << C(1) << "): " << area << endl;
    return area > 0;
}

}
