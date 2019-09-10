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

void Writer::addCurvesEx(const string& path, float* vertices, size_t totalVertices,
                         int32_t* numVertices, size_t numCurves,
                         bool periodic,
                         float* widths,
                         float* uvs,
                         float* normals,
                         float* weights, uint8_t* orders, float* knots,
                         bool _flipAxis)
{
    typedef OCurves Type;
    typedef Type::schema_type Schema;
    
    Type &object = getObject<Type>(path);
    Schema &schema = object.getSchema();
    
    size_t numKnots = 0;
    for (int i = 0; i < numCurves; i++) {
        numKnots += numVertices[i] + orders[i] - 1;
    }
    OFloatGeomParam::Sample widthSample = widths ? OFloatGeomParam::Sample(FloatArraySample((const float *)widths, totalVertices), kVertexScope) : OFloatGeomParam::Sample();
    OV2fGeomParam::Sample uvSample = uvs ? OV2fGeomParam::Sample(V2fArraySample((const V2f *)uvs, totalVertices), kVertexScope ) : OV2fGeomParam::Sample();
    ON3fGeomParam::Sample normalSample = normals ? ON3fGeomParam::Sample(V3fArraySample((const V3f *)normals, totalVertices), kVertexScope) : ON3fGeomParam::Sample();
    OCurvesSchema::Sample curves_sample(
                                        V3fArraySample( ( const V3f * ) vertices, totalVertices ),
                                        Int32ArraySample( numVertices, numCurves),
                                        kVariableOrder,
                                        periodic ? kPeriodic : kNonPeriodic,
                                        widthSample,
                                        uvSample,
                                        normalSample,
                                        kBsplineBasis,
                                        FloatArraySample( weights, totalVertices ),
                                        UcharArraySample( orders, numCurves ),
                                        FloatArraySample( knots, numKnots ));
    schema.set(curves_sample);
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

ghAPI void AbcWriterAddCurveEx(Writer* instance, const char* name,
                               float* vertices, size_t totalVertices,
                               int32_t* numVertices, size_t numCurves,
                               bool periodic,
                               float* widths,
                               float* uvs,
                               float* normals,
                               float* weights, uint8_t* orders, float* knots,
                               bool _flipAxis) {
    
    if (_flipAxis) {
        for (int i = 0; i < totalVertices * 3; i ++) {
            float tempY = vertices[i * 3 + 1];
            vertices[i * 3 + 1] = vertices[i * 3 + 2];
            vertices[i * 3 + 2] = -tempY;
        }
    }
    size_t numNormals = totalVertices * 3;
    instance->addCurvesEx(name, vertices, totalVertices,
                        numVertices, numCurves,
                        periodic,
                        widths,
                        uvs,
                        normals,
                        weights, orders, knots,
                        _flipAxis);
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
    
    
    // Faces && uv
    for ( int i = 0; i < numFaces; i++) {
        auto f = faces[i];
        mesh.faces.push_back( faces[ i ] );
        if (numUVs > 0) {
            mesh.uvs.push_back(V2f(uvs[f * 2], uvs[f * 2 + 1]));
        }
        Imath::Vec3<float> n( normals[f * 3], normals[f * 3+ 1], normals[f * 3 + 2]);
        mesh.normals.push_back( n );
    }
    mesh.numUvs = mesh.uvs.size() / 2;
    mesh.numNormals = mesh.normals.size() / 3;

    std::cout << mesh.uvs.size() << std::endl;

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
