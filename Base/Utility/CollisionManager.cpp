#include "CollisionManager.h"


void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	//std::list<Collider*>::iterator itrA = colliders_.begin();
	//for (; itrA != colliders_.end(); ++itrA) {
	//	// イテレータAからコライダーを取得
	//	Collider* colliderA = *itrA;
	//	// イテレータBはイテレータAの次の要素から回す(重複を避ける)
	//	std::list<Collider*>::iterator itrB = itrA;
	//	itrB++;
	//	for (; itrB != colliders_.end(); ++itrB) {
	//		Collider* colliderB = *itrB;
	//		//円と円
	//		/*if (colliderA->GetId() == ColliderType::Circle && colliderB->GetId() == ColliderType::Circle) {
	//			CheckCollisionCircle(colliderA, colliderB);
	//		}*/
	//		//AABBとAABB
	//	}
	//}
	std::list<BoxCollider*>::iterator BoxitrA = BoxColliders_.begin();
	for (; BoxitrA != BoxColliders_.end(); ++BoxitrA) {
		// イテレータAからコライダーを取得
		BoxCollider* colliderA = *BoxitrA;
		// イテレータBはイテレータAの次の要素から回す(重複を避ける)
		std::list<BoxCollider*>::iterator BoxitrB = BoxitrA;
		BoxitrB++;
		for (; BoxitrB != BoxColliders_.end(); ++BoxitrB) {
			BoxCollider* colliderB = *BoxitrB;
			CheckCollisionBox(colliderA, colliderB);
		}
	}
}

//void CollisionManager::CheckCollisionCircle(Collider * colliderA, Collider * colliderB) {
//	// 判定対象AとBの座標
//	Vector3 posA = { 1.0f,1.0f,1.0f }, posB = {1.0f, 1.0f,1.0f};
//	//posA = colliderA->GetWorldPosition();
//	//posB = colliderB->GetWorldPosition();
//	float Length =(float)sqrt(
//	    (posB.x - posA.x) * (posB.x - posA.x) +
//		(posB.y - posA.y) * (posB.y - posA.y) +
//	    (posB.z - posA.z) * (posB.z - posA.z));
//	// コライダーのフィルターの値でビット演算
//	if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0 && (colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
//		return;
//	}
//	else if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0) {
//		if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//			// コライダーAの衝突時コールバック
//			colliderA->OnCollision(colliderB->GetcollitionAttribute());
//		}
//	}
//	else if ((colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
//		if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//			// コライダーBの衝突時コールバック
//			colliderB->OnCollision(colliderA->GetcollitionAttribute());
//		}
//	}
//	else {
//		if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//			// コライダーAの衝突時コールバック
//			colliderA->OnCollision(colliderB->GetcollitionAttribute());
//			// コライダーBの衝突時コールバック
//			colliderB->OnCollision(colliderA->GetcollitionAttribute());
//		}
//	}
//}

void CollisionManager::CheckCollisionBox(BoxCollider * colliderA, BoxCollider * colliderB)
{
	// 判定対象AとBの座標
	AABB posA, posB;
	posA = colliderA->GetSize();
	posB = colliderB->GetSize();

	// コライダーのフィルターの値でビット演算
	if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0 && (colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
		return;
	}

	if ((posA.min.x <= posB.max.x && posA.max.x >= posB.min.x) && //x軸
		(posA.min.y <= posB.max.y && posA.max.y >= posB.min.y) && //y軸
		(posA.min.z <= posB.max.z && posA.max.z >= posB.min.z)    //z軸
		) {
		if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0) {
			// コライダーAの衝突時コールバック
			colliderA->OnCollision(colliderB);
		}
		else if ((colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
			// コライダーBの衝突時コールバック
			colliderB->OnCollision(colliderA);
		}
		else {
			// コライダーAの衝突時コールバック
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバック
			colliderB->OnCollision(colliderA);
		}
	}

}

//void CollisionManager::CheckCollitionOBBox(OBBoxCollider* colliderA, OBBoxCollider* colliderB)
//{
//	OBB obb_1, obb_2;
//	const int32_t kAxis = 3;
//	for (size_t i = 0; i < kAxis; i++) {
//		// obb_1の軸
//		if (SeparationAxis(colliderA->GetOrientations(i), obb_1, obb_2)) {
//			return;
//		}
//		// obb_2の軸
//		if (SeparationAxis(colliderB->GetOrientations(i), obb_1, obb_2)) {
//			return;
//		}
//		for (size_t j = 0; j < kAxis; j++) {
//			Vector3 tmp = Cross(colliderA->GetOrientations(i), colliderB->GetOrientations(j));
//			if (SeparationAxis(colliderA->GetOrientations(i), obb_1, obb_2)) {
//				return;
//			}
//		}
//	}
//
//	if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0) {
//		// コライダーAの衝突時コールバック
//		colliderA->OnCollision(colliderB);
//	}
//	else if ((colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
//		// コライダーBの衝突時コールバック
//		colliderB->OnCollision(colliderA);
//	}
//	else {
//		// コライダーAの衝突時コールバック
//		colliderA->OnCollision(colliderB);
//		// コライダーBの衝突時コールバック
//		colliderB->OnCollision(colliderA);
//	}
//}
//bool CollisionManager::SeparationAxis(const Vector3 axis, const OBB obb_1, const OBB obb_2) {// 分離軸
//	Vector3 L = axis;
//	// 頂点数
//	const int32_t kIndex = 8;
//	// 頂点格納用配列
//	std::vector<Vector3> vertices_1;
//	std::vector<Vector3> vertices_2;
//	// 配列に頂点を代入
//	OBBIndex(obb_1, vertices_1);
//	OBBIndex(obb_2, vertices_2);
//	// 距離を格納
//	float min_1 = Dot(vertices_1[0], L);
//	float max_1 = min_1;
//	float min_2 = Dot(vertices_2[0], L);
//	float max_2 = min_2;
//	for (size_t i = 1; i < kIndex; i++) {
//		float dot_1 = Dot(vertices_1[i], L);
//		float dot_2 = Dot(vertices_2[i], L);
//		// min/max比べる
//		min_1 = (std::min)(min_1, dot_1);
//		max_1 = (std::max)(max_1, dot_1);
//		min_2 = (std::min)(min_2, dot_2);
//		max_2 = (std::max)(max_2, dot_2);
//	}
//	float L1 = max_1 - min_1;
//	float L2 = max_2 - min_2;
//	float sumSpan = L1 + L2;
//	float longSpan = (std::max)(max_1, max_2) - (std::min)(min_1, min_2);
//	// 分離軸である
//	if (sumSpan < longSpan) {
//		return true;
//	}
//	return false;
//}