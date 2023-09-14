#include <utils.h>

int read_circle_definition() {
    int circle_vertexes = 5;
    const char * const path = "graphics.config";
    FILE* file;
    fopen_s(&file, path, "r");
    fscanf_s(file, "circle: %d", &circle_vertexes);
    fclose(file);

    return circle_vertexes;
}
