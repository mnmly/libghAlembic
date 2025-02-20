#include "ofxAlembicUtil.h"

// #include "H5public.h"

static bool inited = false;

void ofxAlembic::init()
{
	if (inited) return;
	inited = true;
	
//    ofLogVerbose("ofxAlembic") << "alembic version: " << Alembic::Abc::GetLibraryVersionShort();
	
//    H5dont_atexit();
}

//
//void ofxAlembic::transform(ofMesh &mesh, const glm::mat4 &m)
//{
//    vector<glm::vec3>& vertices = mesh.getVertices();
//
//    for (int i = 0; i < vertices.size(); i++)
//    {
//        vertices[i] = vertices[i] * m;
//    }
//
//    if (mesh.hasNormals())
//    {
//        vector<glm::vec3>& normals = mesh.getNormals();
//
//        for (int i = 0; i < normals.size(); i++)
//        {
//            const glm::vec3& v = normals[i];
//            normals[i] = ofVec4f(v.x, v.y, v.z, 0) * m;
//        }
//    }
//}

vector<glm::vec3> toOf(const vector<ofxAlembic::Point>& v)
{
	vector<glm::vec3> arr(v.size());
	for (int i = 0; i < v.size(); i++)
		arr[i] = v[i].pos;

	return arr;
}
