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
//bool IsCollision(const OBB& obb_1, const OBB& obb_2) {
//    const int32_t kAxis = 3;
//    for (size_t i = 0; i < kAxis; i++) {
//        // obb_1の軸
//        if (SeparationAxis(obb1.orientations[i], obb_1, obb_2)) {
//            return false;
//        }
//        // obb_2の軸
//        if (SeparationAxis(obb2.orientations[i], obb_1, obb_2)) {
//            return false;
//        }
//        for (size_t j = 0; j < kAxis; j++) {
//            Vector3 tmp = Cross(obb1.orientations[i], obb2.orientations[j]);
//            if (SeparationAxis(obb1.orientations[i], obb_1, obb_2)) {
//                return false;
//            }
//        }
//    }
//    return true;
//}
//
//void OBBoxCollider::OBBIndex(const OBB & obb, std::vector<Vector3>&outputvertices) {
//    std::vector<Vector3> vertices = {
//        {-obb.size},
//        {+obb.size.x, -obb.size.y, -obb.size.z},
//        {+obb.size.x, -obb.size.y, +obb.size.z},
//        {-obb.size.x, -obb.size.y, +obb.size.z},
//        {-obb.size.x, +obb.size.y, -obb.size.z},
//        {+obb.size.x, +obb.size.y, -obb.size.z},
//        {obb.size},
//        {-obb.size.x, +obb.size.y, +obb.size.z},
//    };
//
//    Matrix4x4 rotateMatrix = SetRotate(obb.orientations);
//    for (auto& vertex : vertices) {
//        vertex = Transform(vertex, rotateMatrix);
//        vertex = vertex + obb.center_;
//    }
//    output_vertices = vertices;
//}
//
//bool OBBoxCollider::SeparationAxis(const Vector3 axis, const OBB obb_1, const OBB obb_2) {// 分離軸
//    Vector3 L = axis;
//    // 頂点数
//    const int32_t kIndex = 8;
//    // 頂点格納用配列
//    std::vector<Vector3> vertices_1;
//    std::vector<Vector3> vertices_2;
//    // 配列に頂点を代入
//    OBBIndex(obb_1, vertices_1);
//    OBBIndex(obb_2, vertices_2);
//    // 距離を格納
//    float min_1 = Dot(vertices_1[0], L);
//    float max_1 = min_1;
//    float min_2 = Dot(vertices_2[0], L);
//    float max_2 = min_2;
//    for (size_t i = 1; i < kIndex; i++) {
//        float dot_1 = Dot(vertices_1[i], L);
//        float dot_2 = Dot(vertices_2[i], L);
//        // min/max比べる
//        min_1 = (std::min)(min_1, dot_1);
//        max_1 = (std::max)(max_1, dot_1);
//        min_2 = (std::min)(min_2, dot_2);
//        max_2 = (std::max)(max_2, dot_2);
//    }
//    float L1 = max_1 - min_1;
//    float L2 = max_2 - min_2;
//    float sumSpan = L1 + L2;
//    float longSpan = (std::max)(max_1, max_2) - (std::min)(min_1, min_2);
//    // 分離軸である
//    if (sumSpan < longSpan) {
//        return true;
//    }
//    return false;
//}