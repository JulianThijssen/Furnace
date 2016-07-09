#include "normalgenerator.h"

#include "TextureLoader.h"
#include "util/vector3f.h"
#include "Image.h"
#include "mesh.h"
#include "util/color.h"
#include "util/math.h"

#include <iostream>

Image NormalGenerator::getNormalMap(Mesh& mesh, int size) {
    Image image(size, size);

    for (int i = 0; i < mesh.indices.size(); i += 3) {
        unsigned int index = mesh.indices[i];

        Vector3f v1 = mesh.vertices[mesh.indices[i + 0]];
        Vector3f v2 = mesh.vertices[mesh.indices[i + 1]];
        Vector3f v3 = mesh.vertices[mesh.indices[i + 2]];

        Vector2f t1 = mesh.texCoords[mesh.indices[i + 0]];
        Vector2f t2 = mesh.texCoords[mesh.indices[i + 1]];
        Vector2f t3 = mesh.texCoords[mesh.indices[i + 2]];

        Vector3f n1 = mesh.normals[mesh.indices[i + 0]];
        Vector3f n2 = mesh.normals[mesh.indices[i + 1]];
        Vector3f n3 = mesh.normals[mesh.indices[i + 2]];

        int min_x = Math::floor(Math::min(t1.x, t2.x, t3.x) * size);
        int min_y = Math::floor(Math::min(t1.y, t2.y, t3.y) * size);
        int max_x = Math::ceil(Math::max(t1.x, t2.x, t3.x) * size);
        int max_y = Math::ceil(Math::max(t1.y, t2.y, t3.y) * size);

        min_x = Math::clamp(min_x, 0, size);
        min_y = Math::clamp(min_y, 0, size);
        max_x = Math::clamp(max_x, 0, size);
        max_y = Math::clamp(max_y, 0, size);

        for (int x = min_x; x < max_x; x++) {
            for (int y = min_y; y < max_y; y++) {
                bool draw = false;

                for (int xx = 0; xx <= 1; xx++) {
                    for (int yy = 0; yy <= 1; yy++) {
                        float gx = (float)(x + xx) / size;
                        float gy = (float)(y + yy) / size;

                        float alpha = ((t2.y - t3.y) * (gx - t3.x) + (t3.x - t2.x) * (gy - t3.y)) /
                            ((t2.y - t3.y) * (t1.x - t3.x) + (t3.x - t2.x) * (t1.y - t3.y));
                        float beta = ((t3.y - t1.y) * (gx - t3.x) + (t1.x - t3.x) * (gy - t3.y)) /
                            ((t2.y - t3.y) * (t1.x - t3.x) + (t3.x - t2.x) * (t1.y - t3.y));
                        float gamma = 1 - alpha - beta;

                        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                            draw = true;
                        }
                    }
                }

                if (draw) {
                    int ny = size-1 - y;

                    Color color = Color::fromVector(n1);

                    image.setColor(x, ny, color);
                }
            }
        }
    }

    return image;
}
