//------------------------------------------------------------------------------
//
// quickGL - A quick and easy to use OpenGL wrapper
//
// RUNTIME LIBRARIES PACKAGE
//    shader.cpp
//
// DESCRIPTION:
// -----------
// Manages shaders and the programs they're included in.
//
// FROM:
// -----
//      Learn OpenGL (https://learnopengl.com/)
//
// DISCLAIMER: Changes to the original source have been made.
//------------------------------------------------------------------------------

#include "qgl/shader.hpp"


const unordered_map<uint16_t, int> QGlShaderType_to_GL = {
    { SHADER_VERTEX,   GL_VERTEX_SHADER},
    { SHADER_FRAGMENT, GL_FRAGMENT_SHADER},
    { SHADER_GEOMETRY, GL_GEOMETRY_SHADER},
    { SHADER_COMPUTE,  GL_COMPUTE_SHADER},
};


void QGlShaderReport::setReport(const int err, const string msg) {
    error   = err;
    message = msg;
}

bool QGlShaderReport::success(void) {
    return error == 0;
}

const string QGlShaderReport::what() const throw ( ) {
    string log("");
    if (error & TYPE_READING)     log += "Could not read ";
    if (error & TYPE_COMPILATION) log += "Compilation on ";
    if (error & TYPE_LINKING)     log += "Linking on ";
    if (error & SHADER_VERTEX)    log += "vertex shader ";
    if (error & SHADER_FRAGMENT)  log += "fragment shader ";
    if (error & SHADER_GEOMETRY)  log += "geometry shader ";
    if (error & SHADER_COMPUTE)   log += "compute shader ";
    if (error & SHADER_PROGRAM)   log += "shader program ";
    log += "(message: " + message + ").";
    return log;
}

/*
QGlShader::QGlShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) :
    QGlShader(string(vertexPath), string(fragmentPath), geometryPath == nullptr ? "" : string(geometryPath)) {}

QGlShader::QGlShader(const char* computePath) :
    QGlShader(string(computePath)) {}

QGlShader::QGlShader(const string vertexPath, const string fragmentPath, const string geometryPath) {
    this->withShaders(vertexPath, fragmentPath, geometryPath);
}

QGlShader::QGlShader(const string computePath) {
    this->withShaders(computePath);
}
*/

QGlShader::QGlShader(const char* rootPath) {
    this->rootPath = fs::path(rootPath);
}


string QGlShader::canonicalPath(fs::path root, string path) {
    if (fs::path(path).is_absolute())
        return path;
    return (root / path).string();
}


QGlShader& QGlShader::withShaders(const string vertexPath, const string fragmentPath, const string geometryPath) {
    this->shader.vertex.path   = QGlShader::canonicalPath(this->rootPath, vertexPath);
    this->shader.vertex.type   = SHADER_VERTEX;
    this->shader.fragment.path = QGlShader::canonicalPath(this->rootPath, fragmentPath);
    this->shader.fragment.type = SHADER_FRAGMENT;
    if (geometryPath.empty()) {
        this->type = QGlShaderProgramType::GraphicWithoutGeometry;
    } else {
        this->shader.geometry.path = QGlShader::canonicalPath(this->rootPath, geometryPath);
        this->shader.geometry.type = SHADER_GEOMETRY;
        this->type = QGlShaderProgramType::GraphicWithGeometry;
    }
    return *this;
}


QGlShader& QGlShader::withShaders(const string computePath) {
    this->shader.compute.path = QGlShader::canonicalPath(this->rootPath, computePath);
    this->shader.compute.type = SHADER_COMPUTE;
    this->type = QGlShaderProgramType::Compute;
    return *this;
}


bool QGlShader::readShader(QGlShaderDef& shader) {
    try {
        ifstream file;
        file.exceptions(ifstream::failbit | ifstream::badbit);
        file.open(shader.path);
        stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        shader.code = sstream.str();
    } catch (ifstream::failure& e) {
        this->report.setReport(TYPE_READING | shader.type, string(e.what()));
        return false;
    }
    return true;
}


bool QGlShader::compile(QGlShaderDef& shader) {
    const char* code = shader.code.c_str();
    shader.id = glCreateShader(QGlShaderType_to_GL.at(shader.type));
    glShaderSource(shader.id, 1, &code, NULL);
    glCompileShader(shader.id);
    return this->checkErrors(shader);
}


bool QGlShader::link() {
    this->id = glCreateProgram();

    switch (this->type) {
        case QGlShaderProgramType::Compute:
            glAttachShader(this->id, this->shader.compute.id);
            break;

        case QGlShaderProgramType::GraphicWithGeometry:
            glAttachShader(this->id, this->shader.geometry.id);

        case QGlShaderProgramType::GraphicWithoutGeometry:
            glAttachShader(this->id, this->shader.vertex.id);
            glAttachShader(this->id, this->shader.fragment.id);

        default:
            break;
    }

    glLinkProgram(this->id);
    if (!this->checkErrors(this->id, SHADER_PROGRAM))
        return false;

    switch (this->type) {
        case QGlShaderProgramType::Compute:
            glDeleteShader(this->shader.compute.id);
            break;

        case QGlShaderProgramType::GraphicWithGeometry:
            glDeleteShader(this->shader.geometry.id);

        case QGlShaderProgramType::GraphicWithoutGeometry:
            glDeleteShader(this->shader.vertex.id);
            glDeleteShader(this->shader.fragment.id);

        default:
            break;
    }

    return true;
}


bool QGlShader::build() {
    switch (this->type) {
        case QGlShaderProgramType::Compute:
            if (!this->readShader(this->shader.compute))  return false;
            if (!this->compile(this->shader.compute))     return false;
            break;

        case QGlShaderProgramType::GraphicWithGeometry:
            if (!this->readShader(this->shader.geometry)) return false;
            if (!this->compile(this->shader.geometry))    return false;

        case QGlShaderProgramType::GraphicWithoutGeometry:
            if (!this->readShader(this->shader.vertex))   return false;
            if (!this->compile(this->shader.vertex))      return false;
            if (!this->readShader(this->shader.fragment)) return false;
            if (!this->compile(this->shader.fragment))    return false;
            break;

        default:
            return false;
    }

    return this->link();
}


bool QGlShader::checkErrors(QGlShaderDef& shader) {
    return this->checkErrors(shader.id, shader.type);
}

bool QGlShader::checkErrors(uint32_t id, uint16_t type) {
    GLint success;
    GLchar log[1024];
    const int GL_TYPE_STATUS = (type & SHADER_PROGRAM) ? GL_LINK_STATUS : GL_COMPILE_STATUS;
    const uint16_t QGL_TYPE  = (type & SHADER_PROGRAM) ? TYPE_LINKING   : TYPE_COMPILATION;

    glGetShaderiv(id, GL_TYPE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 1024, NULL, log);
        report.setReport(QGL_TYPE | type, string(log));
        return false;
    }
    return true;
}


void QGlShader::use() {
    glUseProgram(this->id);
}


QGlShaderDef QGlShader::getShader(uint16_t type) {
    switch (type) {
        case SHADER_VERTEX:   return this->shader.vertex;
        case SHADER_FRAGMENT: return this->shader.fragment;
        case SHADER_GEOMETRY: return this->shader.geometry;
        case SHADER_COMPUTE:  return this->shader.compute;
        default: throw std::runtime_error("Unrecognized shader type.");
    }
}


void QGlShader::setBool(const string& name, bool value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int) value);
}


void QGlShader::setInt(const string& name, int value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}


void QGlShader::setFloat(const string& name, float value) const {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}


void QGlShader::setVec2(const string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}


void QGlShader::setVec2(const string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
}


void QGlShader::setVec3(const string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}


void QGlShader::setVec3(const string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
}


void QGlShader::setVec4(const string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}


void QGlShader::setVec4(const string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
}


void QGlShader::setMat2(const string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void QGlShader::setMat3(const string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void QGlShader::setMat4(const string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
