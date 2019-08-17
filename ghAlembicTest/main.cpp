//
//  main.cpp
//  ghAlembicTest
//
//  Created by HIROAKI YAMANE on 17/08/2019.
//  Copyright © 2019 HIROAKI YAMANE. All rights reserved.
//

#include <iostream>
#include "ofxAlembicWriter.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    string path = "/Users/mnmly/Downloads/sample.abc";
    
    {
        
        auto writer = AbcWriterCreateInstance();
        AbcWriterOpen(writer, "/Users/mnmly/Downloads/sample.abc");
        
        float vertices[12] = { -1, -2,0,-1,2,0,1,-2,0,1,2, 0};
        int numVertices = 4;
        float normals[12] = {-1,-2,0,-1,2, 0,1,-2,0,1,2,0 };
        int numNormals = 4;
        float uvs[0] = {};
        int numUvs = 0;
        int faces[6] = {0,2, 3, 0,3, 1};
        int numFaces = 6;
        int numFaceCount = 2;
        
        AbcWriterAddPolyMesh(writer, "/mesh",
                             vertices, numVertices,
                             normals, numNormals,
                             uvs, numUvs,
                             faces, numFaces, numFaceCount, true);
        AbcWriterClose(writer);
        
    }
    
    return 0;
}
