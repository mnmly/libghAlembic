#include "ofxAlembicWriter.h"
#include <iostream>

using namespace ofxAlembic;
using namespace Alembic::AbcGeom;

bool Writer::open(const string& path, float fps, Alembic::AbcCoreFactory::IFactory::CoreType type)
{
	ofxAlembic::init();
    if ( type == Alembic::AbcCoreFactory::IFactory::kOgawa) {
        archive = OArchive(Alembic::AbcCoreOgawa::WriteArchive(), path);
    }
//    } else if ( type == Alembic::AbcCoreFactory::IFactory::kHDF5 ) {
//        archive = OArchive(Alembic::AbcCoreHDF5::WriteArchive(), path);
//    }
	if (!archive.valid()) return false;

	archive.setCompressionHint(1);

	inv_fps = 1. / fps;
	rewind();

	return true;
}

void Writer::close()
{
	map<string, Alembic::AbcGeom::OObject*>::iterator it = object_map.begin();
	while (it != object_map.end())
	{
		delete it->second;
		it++;
	}

	object_map.clear();

	if (archive.valid())
		archive.reset();
}

void Writer::addPoints(const string& path, const Points& points)
{
	typedef OPoints Type;
	typedef Type::schema_type Schema;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	points.get(schema);
}

void Writer::addPolyMesh(const string& path, const PolyMesh& polymesh, const string& materialName)
{
	typedef OPolyMesh Type;
	typedef Type::schema_type Schema;
    

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();
    
    // Add materials to slot
    string _materialName = materialName == "" ? object.getName() : materialName;
    auto faceset = schema.createFaceSet(_materialName);
    Alembic::AbcGeom::OFaceSetSchema::Sample facesetSamp(polymesh.mesh.faces);
    faceset.getSchema().set(facesetSamp);
    
	polymesh.get(schema);
}

void Writer::addCurves(const string& path, const Curves& curves)
{
	typedef OCurves Type;
	typedef Type::schema_type Schema;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	curves.get(schema);
}

void Writer::addXform(const string& path, const XForm& xform)
{
	typedef OXform Type;
	typedef Type::schema_type Schema;
	
	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();
	
	xform.get(schema);
}

void Writer::addCamera(const string& path, const Camera& camera)
{
	typedef OCamera Type;
	typedef Type::schema_type Schema;
	
	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();
	
	camera.get(schema);
}

void Writer::addCamera(const string& path, const ofCamera& ofcamera)
{
//    addXform(path, ofcamera.getGlobalTransformMatrix());
	
	Camera camera(ofcamera);
	camera.updateSample(ofcamera);
	addCamera(path + "/cameraShape", camera);
}

// time

void Writer::setTime(float time)
{
	current_time = time;
}

void Writer::flashFrame()
{
	setTime(getTime() + inv_fps);
}

void Writer::rewind()
{
	setTime(0);
}


ghAPI Writer* AbcWriterCreateInstance() {
    auto writer = new Writer();
    return writer;
}

ghAPI bool AbcWriterOpen(Writer* instance, char* filepath) {
    return instance->open(filepath);
}

ghAPI void AbcWriterClose(Writer* instance) {
    instance->close();
}

ghAPI void AbcWriterAddCurve(Writer* instance, const char* name,
	float* vertices, int numVertices, int degree, bool periodic, bool _flipAxis) {
	ofxAlembic::Polyline c;
	c.degree = degree;
	c.periodic = periodic;
	c.addVertices(vertices, numVertices, _flipAxis);
	Curves curves;
	curves.curves.push_back(c);
	instance->addCurves(name, curves);
}

ghAPI void AbcWriterAddPolyMesh(Writer* instance, const char *name,
                                const char *materialName,
                                float* vertices, int numVertices,
                                float* normals, int numNormals,
                                float* uvs, int numUVs,
                                int* faces, int numFaces, int numFaceCount, bool _flipAxis) {

    auto mesh = Mesh();
    mesh.name = name;
    
    // Vertices
    for ( int i = 0; i < numVertices; i += 3) {
        Imath::Vec3<float> v;
        if (_flipAxis) {
            v.setValue( vertices[i], vertices[i + 2], -vertices[i + 1]);  // Rhino → Blender coordinate system
        } else {
            v.setValue( vertices[i], vertices[i + 1], vertices[i + 2]);
        }
        mesh.vertices.push_back( v );
    }
    mesh.numVertices = numVertices;
    
    // Normals
    for ( int i = 0; i < numNormals; i += 3) {
        Imath::Vec3<float> n( normals[i], normals[i + 1], normals[i + 2]);
        mesh.normals.push_back( n );
    }
    mesh.numNormals = numNormals;
    
    // UVs
    for ( int i = 0; i < numUVs; i += 2) {
        Imath::Vec2<float> u(uvs[i], uvs[i + 1]);
        mesh.uvs.push_back(u);
    }
    mesh.numUvs = numUVs;
    
    
    // Faces
    for ( int i = 0; i < numFaces; i++) {
        mesh.faces.push_back( faces[ i ] );
    }
    for (int i = 0; i < numFaceCount; i++) {
        mesh.faceCounts.push_back(3);
    }
    mesh.numFaces = numFaceCount;
    
    auto polymesh = PolyMesh(mesh);
    if (materialName == nullptr) {
        instance->addPolyMesh(name, polymesh);
    } else {
        instance->addPolyMesh(name, polymesh, materialName);
    }
}
