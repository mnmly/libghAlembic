#pragma once

#include <glm/glm.hpp>
#include <Alembic/AbcGeom/All.h>
#include <iostream>
//#include <Alembic/AbcCoreHDF5/All.h>

using namespace std;
// #include "ofMain.h"

#include "ofxAlembicUtil.h"

namespace ofxAlembic
{
    struct Mesh {
        
        int numVertices;
        int numNormals;
        int numUvs;
        int numFaces;
        
        string name;
        
        vector<Imath::Vec3<float>> vertices;
        vector<Imath::Vec3<float>> normals;
        vector<Imath::Vec2<float>> uvs;
        vector<Alembic::Abc::int32_t> faces;
        vector<Alembic::Abc::int32_t> faceCounts;
        
        void clear (){
            numVertices = 0;
            numNormals = 0;
            numUvs = 0;
            numFaces = 0;
            name = "";
            vertices.clear();
            normals.clear();
            uvs.clear();
            faces.clear();
            faceCounts.clear();
        }
    };

    
    class Polyline;
    class ofCamera;

    
    
class PolyMesh;
class Points;
class Curves;
class Camera;
class XForm;

struct Point;

enum Type
{
	POINTS = 0,
	CURVES,
	POLYMESH,
	CAMERA,
	XFORM,
	UNKHOWN
};
}

class ofxAlembic::XForm
{
public:
	Imath::M44f mat;
	
	XForm() {}
    XForm(const glm::mat4& matrix);
	
	void draw();
	
	void get(Alembic::AbcGeom::OXformSchema &schema) const;
	void set(Alembic::AbcGeom::IXformSchema &schema, float time);
};

class ofxAlembic::PolyMesh
{
public:
    Mesh mesh;

	PolyMesh() {}
    PolyMesh(const Mesh& mesh) : mesh(mesh) {}

	void get(Alembic::AbcGeom::OPolyMeshSchema &schema) const;
//    void set(Alembic::AbcGeom::IPolyMeshSchema &schema, float time);

	void draw();
};

struct ofxAlembic::Point
{
	uint64_t id;
    glm::vec3 pos;
	
	Point() : id(-1) {}
	Point(const glm::vec3& pos) : id(-1), pos(pos) {}
	Point(uint64_t id, const glm::vec3& pos) : id(id), pos(pos) {}
	
	Point(float x, float y, float z) : id(-1), pos(x, y, z) {}
	Point(uint64_t id, float x, float y, float z) : id(id), pos(x, y, z) {}
};


class ofxAlembic::Points
{
public:
    vector<Point> points;
	
	Points() {}
    Points(const vector<glm::vec3>& points);
	void get(Alembic::AbcGeom::OPointsSchema &schema) const;
	void set(Alembic::AbcGeom::IPointsSchema &schema, float time);

	void draw();
};

class ofxAlembic::Curves
{
public:
    vector<Polyline> curves;

	Curves() {}
	Curves(const vector<Polyline> &curves) : curves(curves) {}

	void get(Alembic::AbcGeom::OCurvesSchema &schema) const;
	void set(Alembic::AbcGeom::ICurvesSchema &schema, float time);

	void draw();
};

class ofxAlembic::Camera
{
public:

	Camera() : width(0), height(0) {}
    Camera(const ofCamera& camera) : width(0), height(0) {}
	
    void get(Alembic::AbcGeom::OCameraSchema &schema) const;
	void set(Alembic::AbcGeom::ICameraSchema &schema, float time);
	
    void setViewport(int width, int height) { this->width = width, this->height = height; }
	
    void updateParams(ofCamera &camera, glm::mat4 xform);
    void updateSample(const ofCamera &camera);
	
	void draw();
	
protected:
	
	int width, height;
	Alembic::AbcGeom::CameraSample sample;
};

class ofxAlembic::Polyline
{
public:
    Polyline() {}
    Polyline(const std::vector<glm::vec3>& verts);
    size_t size() const;
    const glm::vec3& operator[] (int index) const;
private:
    vector<glm::vec3> points;
    void addVertices(const vector<glm::vec3>& verts);
    void clear();
};
