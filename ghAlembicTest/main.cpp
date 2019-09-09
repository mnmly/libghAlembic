//
//  main.cpp
//  ghAlembicTest
//
//  Created by HIROAKI YAMANE on 17/08/2019.
//  Copyright © 2019 HIROAKI YAMANE. All rights reserved.
//

#include <iostream>
#include <string>
#include "ofxAlembicWriter.h"

int main(int argc, const char * argv[]) {    
    {
#ifdef _WIN32
		string path = "C:\\Users\\mnmly\\Downloads\\sample.abc";
		char* p = const_cast<char*>(path.c_str());
		AbcWriterOpen(writer, p);
#elif __APPLE__
        string path = "/Users/mnmly/Downloads/sample.abc";

#endif
        char* p = const_cast<char*>(path.c_str());
        
        float vertices[12] = { -1, -2, 0, -1,  2, 0, 1, -2, 0, 1,  2, 0};
	float curveVertices[30] = { -39.717781,-64.104676,0,-2.998162,-50.084458,0,17.69835,-70.447155,0,52.415081,-76.122006,0,80.789332,-43.074349,0,95.312745,-14.032468,38.240563,127.189577,-49.083014,0,137.204019,-69.779526,0,154.562384,-34.728981,0,158.234346,-6.020915,0};
        int numVertices = 4;
        float normals[12] = {-1,-2,0,
                            -1,2, 0,
                            1,-2,0,
                            1,2,0 };
        int numNormals = 4;
        // float uvs[0] = {1.0, 1.0, 1.0, };
        // int numUvs = 0;
        int faces[6] = {0,2, 3, 0,3, 1};
        int numFaces = 6;
        int numFaceCount = 2;
        
        AbcWriterAddPolyMesh(writer, "/mesh",
                             nullptr,
                             vertices, numVertices,
                             normals, numNormals,
                             nullptr, 0,
                             faces, numFaces, numFaceCount, true);
        AbcWriterAddCurve(writer, "/curve", curveVertices, 10, 3, true, true);
        AbcWriterClose(writer);
		std::cout << "DONE wriing to sample.abc" << std::endl;
    }
    
    return 0;
}
