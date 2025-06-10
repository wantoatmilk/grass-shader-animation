#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class ModelLoader
{
public:
    // Lädt das Modell und füllt die Vektoren
    bool loadModel(const std::string &path);

    // Öffentliche Datencontainer
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

private:
    void processMesh(class aiMesh *mesh);
};

#endif // MODELLOADER_H