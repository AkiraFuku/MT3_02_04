#include <Novice.h>
#include <imgui.h>
#include <KamataEngine.h>
#include "MassFunction.h"
#include "DrawFunction.h"
const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 traslate = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
	Triangle triangle = { {{ 0.0f, 2.0f, 0.0f }, { 1.0f,0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } }};
	Segment segment = { { 0.0f, 0.0f, -2.0f }, { 1.0f, 1.0f, 2.0f } };
	unsigned int segmentColor = WHITE; // グリッドの色

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		if (keys[DIK_W] != 0) cameraTranslate.z += 0.1f; // 前進
		if (keys[DIK_S] != 0) cameraTranslate.z -= 0.1f; // 後退	
		if (keys[DIK_A] != 0) cameraTranslate.x -= 0.1f; // 左移動
		if (keys[DIK_D] != 0) cameraTranslate.x += 0.1f; // 右移動
		if (keys[DIK_UP] != 0) cameraRotate.x += 0.1f; // 前進
		if (keys[DIK_DOWN] != 0) cameraRotate.x -= 0.1f; // 後退	
		if (keys[DIK_LEFT] != 0) cameraRotate.y -= 0.1f; // 左移動
		if (keys[DIK_RIGHT] != 0) cameraRotate.y += 0.1f; // 右移動

		ImGui::Begin("Window");
		ImGui::DragFloat3("triangle:P0", &triangle.vertces[0].x, 0.01f);
		ImGui::DragFloat3("triangle:P1", &triangle.vertces[1].x, 0.01f);
		ImGui::DragFloat3("triangle:P2", &triangle.vertces[2].x, 0.01f);
		//plane.normal=Normalize(plane.normal);
		//ImGui::DragFloat("Distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("Segment Start", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment End", &segment.diff.x, 0.01f);
		ImGui::End();

		if(isCollision(triangle,segment)) {
			segmentColor = RED; // 衝突している場合は赤色に変更
			
		} else {
			// 衝突していない場合の処理
			segmentColor = WHITE; // 衝突していない場合は白色に戻す
		}

		Matrix4x4 worldMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, traslate);
		//MatrixScreenPrintf(0, kRowHeight+10, worldMatrix, "worldMatrix");
		Matrix4x4 cameraMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f },cameraRotate,cameraTranslate );
		//MatrixScreenPrintf(0, kRowHeight * 20, cameraMatrix, "cameraMatrix");
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		//MatrixScreenPrintf(0, kRowHeight * 30, viewMatrix, "viewMatrix");
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kwindowWidth)/static_cast <float>(kwindowHight) , 0.1f, 100.0f);
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight  , projectionMatrix, "projectionMatrix");
		Matrix4x4 worldViewProjectionMatrix = Multiply( worldMatrix,Multiply(viewMatrix,projectionMatrix));
		Matrix4x4 viewProjectionMatrix =Multiply(viewMatrix,projectionMatrix);

		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight * 20, viewProjectionMatrix, "worldViewProjectionMatrix");
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kwindowWidth), static_cast<float>(kwindowHight), 0.0f, 1.0f);

		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(viewProjectionMatrix,viewportMatrix);
		DrawSegment(segment, viewProjectionMatrix, viewportMatrix, segmentColor);
		DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, WHITE);
		

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
