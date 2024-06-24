#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonLibrary;

int main()
{


    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "Failed to import mesh" << endl;
        return 1;
    }

    cout << "Mesh imported successfully" << endl;
    return 0;
}

