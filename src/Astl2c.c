#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"

int main(void)
{

    FILE *file;
    char file_path[] = "./stl_files/pin.STL";
    file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "%s:%d: [Error] failed to open input file: '%s', %s\n", __FILE__, __LINE__, file_path, strerror(errno));
        exit(1);
    }

    char header[STL_HEADER_SIZE];
    fread(header, STL_HEADER_SIZE, 1, file);
    dprintSTRING(header);

    uint32_t num_of_tri;
    fread(&num_of_tri, STL_NUM_SIZE, 1, file);
    dprintINT(num_of_tri);

    Mesh mesh;
    ada_init_array(Tri, mesh);
    for (size_t i = 0; i < num_of_tri; i++) {
        Tri temp_tri;

        fread(&(temp_tri.normal.x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.normal.y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.normal.z), STL_NUM_SIZE, 1, file);

        fread(&(temp_tri.points[0].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[0].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[0].z), STL_NUM_SIZE, 1, file);

        fread(&(temp_tri.points[1].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[1].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[1].z), STL_NUM_SIZE, 1, file);
        
        fread(&(temp_tri.points[2].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[2].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[2].z), STL_NUM_SIZE, 1, file);

        fseek(file, STL_ATTRIBUTE_BITS_SIZE, SEEK_CUR);

        temp_tri.to_draw = true;
        temp_tri.light_intensity = 1;
        temp_tri.center.x = (temp_tri.points[0].x + temp_tri.points[1].x + temp_tri.points[2].x) / 3;
        temp_tri.center.y = (temp_tri.points[0].y + temp_tri.points[1].y + temp_tri.points[2].y) / 3;
        temp_tri.center.z = (temp_tri.points[0].z + temp_tri.points[1].z + temp_tri.points[2].z) / 3;
        temp_tri.z_min = fmin(temp_tri.points[0].z, fmin(temp_tri.points[1].z, temp_tri.points[2].z));
        temp_tri.z_max = fmax(temp_tri.points[0].z, fmax(temp_tri.points[1].z, temp_tri.points[2].z));

        ada_appand(Tri, mesh, temp_tri);
    }
    // AE_PRINT_MESH(mesh);

    return 0;
}
