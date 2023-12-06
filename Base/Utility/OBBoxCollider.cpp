#include "OBBoxCollider.h"

void OBBoxCollider::Initalize()
{
	Collider::SetId(ColliderType::OBBox);
}

void OBBoxCollider::SetOrientations(Matrix4x4 Matrix)
{
	//X成分
	obb_.orientations[0] = GetXAxis(Matrix);
	obb_.orientations[1] = GetYAxis(Matrix);
	obb_.orientations[2] = GetZAxis(Matrix);

}


void OBBoxCollider::OBBIndex(const OBB & obb, std::vector<Vector3>&outputvertices) {
    std::vector<Vector3> vertices = {
        {-obb.size},
        {+obb.size.x, -obb.size.y, -obb.size.z},
        {+obb.size.x, -obb.size.y, +obb.size.z},
        {-obb.size.x, -obb.size.y, +obb.size.z},
        {-obb.size.x, +obb.size.y, -obb.size.z},
        {+obb.size.x, +obb.size.y, -obb.size.z},
        {obb.size},
        {-obb.size.x, +obb.size.y, +obb.size.z},
    };

    Matrix4x4 rotateMatrix = SetRotate(obb.orientations);
    for (auto& vertex : vertices) {
        vertex = VectorTransform(vertex, rotateMatrix);
        vertex = vertex + obb.center;
    }
    output_vertices = vertices;
}

