#pragma once

//#include "ofMain.h"

#include "ofxAlembicType.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>
//#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/AbcMaterial/All.h>

#include <iostream>


#ifdef _WIN32
#define ghAPI extern "C" __declspec(dllexport)
#elif __APPLE__
#define ghAPI extern "C"
#endif



namespace ofxAlembic
{
    
    
    std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }
    
    std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }
    
    std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        return ltrim(rtrim(str, chars), chars);
    }
    
    //--------------------------------------------------
    vector <string> ofSplitString(const string & source, const string & delimiter, bool ignoreEmpty, bool trimFlag) {
        vector<string> result;
        if (delimiter.empty()) {
            result.push_back(source);
            return result;
        }
        string::const_iterator substart = source.begin(), subend;
        while (true) {
            subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
            string sub(substart, subend);
            if(trimFlag) {
                
                sub = trim(sub);
            }
            if (!ignoreEmpty || !sub.empty()) {
                result.push_back(sub);
            }
            if (subend == source.end()) {
                break;
            }
            substart = subend + delimiter.size();
        }
        return result;
    }
    
    //--------------------------------------------------
    string ofJoinString(const vector<string>& stringElements, const string& delimiter){
        string str;
        if(stringElements.empty()){
            return str;
        }
        auto numStrings = stringElements.size();
        string::size_type strSize = delimiter.size() * (numStrings - 1);
        for (const string &s : stringElements) {
            strSize += s.size();
        }
        str.reserve(strSize);
        str += stringElements[0];
        for (decltype(numStrings) i = 1; i < numStrings; ++i) {
            str += delimiter;
            str += stringElements[i];
        }
        return str;
    }

    
    
class Writer;
}

class ofxAlembic::Writer
{
public:

	~Writer() { close(); }

    bool open(const string& path, float fps = 30, Alembic::AbcCoreFactory::IFactory::CoreType type = Alembic::AbcCoreFactory::IFactory::kOgawa);
	void close();

	void addPoints(const string& path, const Points& points);
    void addPolyMesh(const string& path, const PolyMesh& polymesh, const string& materialName = "");
	void addCurves(const string& path, const Curves& curves);
    void addCurvesEx(const string& path, float* vertices, int totalVertices,
                     int* numVertices, int numCurves,
                     bool periodic,
                     float* widths,
                     float* uvs,
                     float* normals,
                     float* weights, int* orders, float* knots,
                     bool _flipAxis);

	void addXform(const string& path, const XForm& xform);
	void addCamera(const string& path, const Camera& camera);
	void addCamera(const string& path, const ofCamera& camera);

	void setTime(float time);
	float getTime() const { return current_time; }

	void rewind();

	void flashFrame();

protected: 
	map<string, Alembic::AbcGeom::OObject*> object_map;
	Alembic::AbcGeom::OArchive archive;
    Alembic::AbcGeom::OObject root;
    Alembic::Abc::OObject materials;
    Alembic::AbcMaterial::OMaterial materialA;
	float inv_fps;
	float current_time;

	template <typename T>
	T& getObject(const string& path)
	{
		using namespace Alembic::AbcGeom;
    
		// validation
		if (path.empty()
			|| path[0] != '/'
			|| path[path.size() - 1] == '/')
		{
//            ofLogError("ofxAlembic::Writer") << "invalid path: '" << path << "'";
			throw;
		}
		
		map<string, OObject*>::iterator it = object_map.find(path);
		
		// return if already created
		if (it != object_map.end())
			return *(T*)(it->second);

		vector<string> e = ofSplitString(path, "/", true, true);
		string new_object_name = e.back();
		
		TimeSampling Ts(inv_fps, current_time);
		Alembic::Util::uint32_t tsidx = archive.addTimeSampling(Ts);
		
		if (e.size() > 1)
		{
			// root object
			string parent_path = "/" + ofJoinString(vector<string>(e.begin(), e.end() - 1), "/");
			
			map<string, OObject*>::iterator parent_it = object_map.find(parent_path);
			if (parent_it == object_map.end())
			{
//                ofLogError("ofxAlembic::Writer") << "parent object not found: '" << path << "'";
				throw;
			}
			
			OObject* parent_object = parent_it->second;
			
			T *t = new T(*parent_object, new_object_name);
			t->getSchema().setTimeSampling(tsidx);
			object_map[path] = t;
			
			return *t;
		}
		else
		{
			T *t = new T(archive.getTop(), new_object_name);
			t->getSchema().setTimeSampling(tsidx);
			object_map[path] = t;
			
			return *t;
		}
	}
};

#pragma mark - exposed APIs

ghAPI ofxAlembic::Writer* AbcWriterCreateInstance();
ghAPI bool AbcWriterOpen(ofxAlembic::Writer* instance, char* filepath);
ghAPI void AbcWriterClose(ofxAlembic::Writer* instance);
ghAPI void AbcWriterAddPolyMesh(ofxAlembic::Writer* instance, const char *name,
                                const char *materialName,
                                float* vertices, int numVertices,
                                float* normals, int numNormals,
                                float* uvs, int numUVs,
                                int* faces, int numFaces, int numFaceCount, bool _flipAxis);
ghAPI void AbcWriterAddCurve(ofxAlembic::Writer* instance, const char* name,
	float* vertices, int numVertices, int degree, bool periodic, bool _flipAxis);
ghAPI void AbcWriterAddCurveEx(ofxAlembic::Writer* instance, const char* name,
                               float* vertices, int totalVertices,
                               int* numVertices, int numCurves,
                               bool periodic,
                               float* widths,
                               float* uvs,
                               float* normals,
                               float* weights, int* orders, float* knots,
                               bool _flipAxis);
