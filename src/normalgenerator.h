#ifndef NORMALGENERATOR_H
#define NORMALGENERATOR_H

class Image;
class Mesh;

class NormalGenerator
{
public:
    static Image getNormalMap(Mesh& mesh, int size);
};

#endif // NORMALGENERATOR_H
