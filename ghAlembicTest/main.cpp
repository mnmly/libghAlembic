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
        
        int numFaces = 36;
        int numFaceCount = 36 / 3;
        
        AbcWriterAddPolyMesh(writer, "/mesh",
                             nullptr,
                             vertices, numVertices,
                             normals, numNormals,
                             uvs, numUvs,
                             faces, numFaces, numFaceCount, true);
        AbcWriterAddCurve(writer, "/curve", curveVertices, 10, 3, true, true);
        
        
        {
            // polycurve
            float vertices[39] = {-1.327575,7.071375,0,-6.900792,6.586747,0,-3.023771,9.656056,0,0.557088,9.413742,0,0.557088,9.413742,0,1.230182,5.994425,0,1.230182,5.994425,0,-1.779935,5.401882,0,-4.100722,5.967501,0,-6.335393,5.079017,0,-6.900792,2.413565,0,-2.969924,4.755932,0,4.111024,6.10212,0};
            int totalVertices = 39 / 3;
            int32_t numVertices[3] = {4, 2, 7};
            size_t numCurves = 3;
            bool periodic = false;
            
            float weights[13] = {1,1,1,1,1,1,1,1,1,1,1, 1, 1};
            uint8_t orders[3] = {2, 1, 5};
            float knots[18] = {0,0,7.064094,14.128188,14.128188,0,0,17.613125,17.613125,17.613125,17.613125,17.613125,28.79803,39.982934,39.982934,39.982934,39.982934,39.982934};
            AbcWriterAddCurveEx(writer, "/curve-ex-polycurve", vertices, totalVertices,
                                numVertices, numCurves,
                                periodic,
                                nullptr,
                                nullptr,
                                nullptr,
                                weights, orders, knots,
                                true);
            
        }
        
        
        {
            // polyline
            float vertices[21] = {-0.862103,1.790285,0,0.887956,2.972444,0,1.629702,1.790285,0,2.580065,2.64793,0,2.487347,3.123111,0,1.397906,2.752238,0,0.598211,1.790285,0};
            int totalVertices = 21 / 3;
            int32_t numVertices[1] = {totalVertices};
            size_t numCurves = 1;
            bool periodic = true;
            
            float weights[7] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            uint8_t orders[1] = {1};
            float knots[7] = {0,2.11192,3.507516,4.787651,5.271794,6.422632,7.673578};
            AbcWriterAddCurveEx(writer, "/curve-ex-polyline", vertices, totalVertices,
                                numVertices, numCurves,
                                periodic,
                                nullptr,
                                nullptr,
                                nullptr,
                                weights, orders, knots,
                                true);
            
        }
        
        {
            // nurb converted arc
            float vertices[24] = {0.565399,0.64617,0,-0.057114,0.856709,0,-0.64617,0.565399,0,-0.856709,-0.057114,0,-0.565399,-0.64617,0,0.057114,-0.856709,0,0.64617,-0.565399,0,0.856709,0.057114,0};
            int totalVertices = 24 / 3;
            int32_t numVertices[1] = {totalVertices};
            size_t numCurves = 1;
            bool periodic = true;
            
            float weights[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            uint8_t orders[1] = {2};
            float knots[9] = {0,0,0.5 * 3.1415, 0.5 * 3.1415,3.1415,3.1415,1.5 * 3.1415,1.5 * 3.1415,2.0 * 3.1415};
            AbcWriterAddCurveEx(writer, "/curve-ex-arc", vertices, totalVertices,
                                numVertices, numCurves,
                                periodic,
                                nullptr,
                                nullptr,
                                nullptr,
                                weights, orders, knots,
                                true);
            
        }
        
        {
            // deg4
            float vertices[15] = {0,0,0,1,1,0,2,0,0,3,1.97297,1.92972,4,0,0};
            int totalVertices = 15 / 3;
            int32_t numVertices[1] = {15/3};
            size_t numCurves = 1;
            bool periodic = false;
            
            float weights[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            uint8_t orders[1] = {4};
            float knots[8] = {0,0,0,0,5.656854,5.656854,5.656854,5.656854};
            AbcWriterAddCurveEx(writer, "/curve-ex-deg4", vertices, totalVertices,
                                numVertices, numCurves,
                                periodic,
                                nullptr,
                                nullptr,
                                nullptr,
                                weights, orders, knots,
                                true);
            
        }
        
        {
            // deg 5
            float vertices[30] = {0.006123,-3.535228,0,0.907552,-1.932689,0,1.208028,-4.307881,0,2.209615,-2.705342,0,2.896418,-2.175931,0,3.382903,-4.450965,0,4.284331,-2.748267,0,5.71517,-2.161623,0,5.672245,-4.264956,0,6.058571,-4.894525,0};
            int totalVertices = 30 / 3;
            int32_t numVertices[1] = {30/3};
            size_t numCurves = 1;
            bool periodic = false;
            float widths[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            size_t numWidth = totalVertices;
            float uvs[20] = {0.0f, 0.0f,
                            0.1f, 0.1f,
                            1.0f, 1.0f,
                            1.0f, 1.0f,
                            1.0f, 1.0f,
                0.0f, 0.0f,
                0.1f, 0.1f,
                1.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 1.0f
            };
            size_t numUvs = totalVertices;
            float normals[30] = {0.0f, 0.0f, 1.0f,
                                0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f
            };
            
            size_t numNormals = totalVertices;
            
            float weights[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            uint8_t orders[1] = {5};
            float knots[14] = {0,0,0,0,0,3.126332,6.252664,9.378995,12.505327,15.631659,15.631659,15.631659,15.631659,15.631659};
            size_t numKnots = 14;
            AbcWriterAddCurveEx(writer, "/curve-ex", vertices, totalVertices,
                                numVertices, numCurves,
                                periodic,
                                nullptr,
                                uvs,
                                normals,
                                weights, orders, knots,
                                true);
            
        }
        
        AbcWriterClose(writer);
		std::cout << "DONE wriing to sample.abc" << std::endl;
    }
    
    return 0;
}
