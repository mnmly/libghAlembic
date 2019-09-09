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
		string path = "C:\\Users\\Hiroaki Yamane\\Downloads\\sample.abc";
#elif __APPLE__
        string path = "/Users/mnmly/Downloads/sample.abc";

#endif
        char* p = const_cast<char*>(path.c_str());
        
        auto writer = AbcWriterCreateInstance();
        AbcWriterOpen(writer, p);

        float vertices[12] = {  -1, -2, 0,
                                -1, 2,  0,
                                1, -2,  0,
                                1,  2,  0 };
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
        AbcWriterClose(writer);
		std::cout << "DONE wriing to sample.abc" << std::endl;
    }
    
    return 0;
}
