#ifndef QGL_SHADER_H
#define QGL_SHADER_H

#include "qgl/common.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#define SHADER_VERTEX    0b00000001
#define SHADER_FRAGMENT  0b00000010
#define SHADER_GEOMETRY  0b00000100
#define SHADER_COMPUTE   0b00001000
#define SHADER_PROGRAM   0b00010000

#define TYPE_READING     0b00100000
#define TYPE_COMPILATION 0b01000000
#define TYPE_LINKING     0b10000000

using namespace std;

enum QGlShaderProgramType {
    GraphicWithoutGeometry,
    GraphicWithGeometry,
    Compute
};

struct QGlShaderDef {
    uint16_t type;
    string   path;
    string   code;
    uint32_t id;
};

struct QGlShaderInfo {
    QGlShaderDef vertex;
    QGlShaderDef fragment;
    QGlShaderDef geometry;
    QGlShaderDef compute;
};


class QGlShaderReport {
private:
    uint16_t error;
    string message;

public:
    QGlShaderReport(): error(0), message("") { }
    QGlShaderReport(const int err, const string msg): error(err), message(msg) { }

    void setReport(const int, const string);
    bool success();

    const string what() const throw();
};


class QGlShader {
private:
    unsigned int         id;
    QGlShaderInfo        shader;
    QGlShaderReport      report;
    QGlShaderProgramType type;

    bool readShader(QGlShaderDef&);
    bool compile(QGlShaderDef&);
    bool link();

    bool checkErrors(QGlShaderDef&);
    bool checkErrors(uint32_t, uint16_t);

public:
    QGlShader() {}
    QGlShader(const char*, const char*, const char* = nullptr);
    QGlShader(const string, const string, const string = "");
    QGlShader(const char*);
    QGlShader(const string);

    uint32_t getID() { return this->id; };
    bool     build();
    void     use();

    QGlShaderDef getShader(uint16_t);

    bool            wasSuccessful()    { return this->report.success(); }
    string          getReport()        { return this->report.what();    }
    QGlShaderReport getReportHandler() { return this->report;           }

    void setBool (const string&, bool) const;
    void setInt  (const string&, int) const;
    void setFloat(const string&, float) const;
    void setVec2 (const string&, const glm::vec2&) const;
    void setVec2 (const string&, float, float) const;
    void setVec3 (const string&, const glm::vec3&) const;
    void setVec3 (const string&, float, float, float) const;
    void setVec4 (const string&, const glm::vec4&) const;
    void setVec4 (const string&, float, float, float, float) const;
    void setMat2 (const string&, const glm::mat2&) const;
    void setMat3 (const string&, const glm::mat3&) const;
    void setMat4 (const string&, const glm::mat4&) const;

};

#endif