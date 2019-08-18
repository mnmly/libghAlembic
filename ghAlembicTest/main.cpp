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
		std::cout << "00000" << std::endl;

        auto writer = AbcWriterCreateInstance();
#ifdef _WIN32
		string path = "C:\\Users\\Hiroaki Yamane\\Downloads\\sample.abc";
		char* p = const_cast<char*>(path.c_str());
		AbcWriterOpen(writer, p);
#elif __APPLE__
		AbcWriterOpen(writer, "/Users/mnmly/Downloads/sample.abc");
#endif
        
        float vertices[12] = { -1, -2,0,-1,2,0,1,-2,0,1,2, 0};
        int numVertices = 4;
        float normals[12] = {-1,-2,0,-1,2, 0,1,-2,0,1,2,0 };
        int numNormals = 4;
        // float uvs[0] = {1.0, 1.0, 1.0, };
        // int numUvs = 0;
        int faces[6] = {0,2, 3, 0,3, 1};
        int numFaces = 6;
        int numFaceCount = 2;
        
        AbcWriterAddPolyMesh(writer, "/mesh",
                             vertices, numVertices,
                             normals, numNormals,
                             nullptr, 0,
                             faces, numFaces, numFaceCount, true);
        AbcWriterClose(writer);
		std::cout << "??" << std::endl;
    }
    
    return 0;
}
