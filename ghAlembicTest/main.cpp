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
        
        auto writer = AbcWriterCreateInstance();
#ifdef _WIN32
		string path = "C:\\Users\\mnmly\\Downloads\\sample.abc";
		char* p = const_cast<char*>(path.c_str());
#elif __APPLE__
        string path = "/Users/mnmly/Downloads/sample.abc";
        char* p = const_cast<char*>(path.c_str());
#endif
        AbcWriterOpen(writer, p);

	float curveVertices[30] = { -39.717781,-64.104676,0,-2.998162,-50.084458,0,17.69835,-70.447155,0,52.415081,-76.122006,0,80.789332,-43.074349,0,95.312745,-14.032468,38.240563,127.189577,-49.083014,0,137.204019,-69.779526,0,154.562384,-34.728981,0,158.234346,-6.020915,0};
        
        float vertices[72] = { -1,-1,-1,-1,1,-1,1,1,-1,1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,-1,-1,1,-1,1,-1,-1,1,1,-1,-1,1,1,-1,1,1,1,1,-1,1,1,1,-1,-1,1,-1,-1,1,1,1,1,1,-1,-1,1,1,-1,1,1,1,1,-1,1,1 };
        
        int numVertices = 72;
        float normals[72] = {0,0,-1,0,0,-1,0,0,-1,0,0,-1,-1,0,0,-1,0,0,-1,0,0,-1,0,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1
 };
        int numNormals = 72;
        float uvs[48] = {1,0,1,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1
 };
        int numUvs = 24;
        
        int faces[36] = {0,1,2,2,3,0,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,14,15,12,16,17,18,18,19,16,20,21,22,22,23,20};
        
        int numFaces = 24;
        int numFaceCount = 24 / 4;
        
        AbcWriterAddPolyMesh(writer, "/mesh",
                             nullptr,
                             vertices, numVertices,
                             normals, numNormals,
                             uvs, numUvs,
                             faces, numFaces, numFaceCount, true);
        AbcWriterAddCurve(writer, "/curve", curveVertices, 10, 3, true, true);
        AbcWriterClose(writer);
		std::cout << "DONE wriing to sample.abc" << std::endl;
    }
    
    return 0;
}
