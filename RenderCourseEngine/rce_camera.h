#pragma once
#include <SBLMath/Matrix44.hpp>
#include <SBLMath/Vector3.hpp>

namespace RCE {
	namespace Camera {
		using namespace SBL::Math;

		Vector3 DEFAULT_FORWARD = Vector3(0, 0, 1);
		Vector3 DEFAULT_UP = Vector3(0, 1, 0);


		Vector3 camPosition = Vector3(0, 0, -2);
		Vector3 camForward = Vector3(0, 0, 1);
		Vector3 camUp = Vector3(0, 1, 0);
		float fov = PI / 2;

		bool leftMouseWasDown = false;
		int32_t startX = 0;
		int32_t startY = 0;

		float cameraYaw;
		float cameraPitch;


		float mouseYaw;
		float mousePitch;

		inline void EventUpdate(char keycode, bool leftMouseDown, int32_t mouseX, int32_t mouseY) {
			constexpr float moveStep = 0.1f;
			if (keycode == 'W') {
				camPosition += camForward * moveStep;
			}
			if (keycode == 'S') {
				camPosition -= camForward * moveStep;
			}
			if (keycode == 'D') {
				camPosition += CrossProduct(camUp, camForward) * moveStep;
			}
			if (keycode == 'A') {
				camPosition -= CrossProduct(camUp, camForward) * moveStep;
			}

			if (leftMouseDown) {
				if (!leftMouseWasDown) {
					startX = mouseX;
					startY = mouseY;
					leftMouseWasDown = true;
				}

				auto mouseDeltaX = mouseX - startX;
				auto mouseDeltaY = mouseY - startY;

				mouseYaw = -mouseDeltaY * 0.01f;
				mousePitch = -mouseDeltaX * 0.01f;

				auto rotationMatrix = Matrix44::RotationX(cameraYaw + mouseYaw) * Matrix44::RotationY(cameraPitch + mousePitch);

				auto forward = rotationMatrix * Vector4(DEFAULT_FORWARD.x, DEFAULT_FORWARD.y, DEFAULT_FORWARD.z, 1);
				forward /= forward.w;
				camForward = Vector3(forward.x, forward.y, forward.z);
			
				auto up = rotationMatrix * Vector4(DEFAULT_UP.x, DEFAULT_UP.y, DEFAULT_UP.z, 1);
				up /= up.w;
				camUp = Vector3(up.x, up.y, up.z);
			}
			else if (leftMouseWasDown) {
				leftMouseWasDown = false;
				cameraYaw += mouseYaw;
				cameraPitch += mousePitch;

				mouseYaw = 0;
				mousePitch = 0;
			}

			// TODO: Cache transform matrix?
			//transformMatrix = MakeCameraTransform(cam.Position, cam.Forward, cam.Up);
		}

		inline Matrix44 MakeCameraTransform(Vector3 camPosition, Vector3 camDirection, Vector3 camUp) {

			auto e = camPosition;
			auto g = camDirection;
			auto t = camUp;

			auto w = g / Length(g); // Left hand coordinate system!
			auto u = CrossProduct(t, w);
			u = u / Length(u);
			auto v = CrossProduct(w, u);

			Matrix44 m1 = Matrix44(
				u.x, u.y, u.z, 0,
				v.x, v.y, v.z, 0,
				w.x, w.y, w.z, 0,
				0, 0, 0, 1
			);

			Matrix44 m2 = Matrix44(
				1, 0, 0, -e.x,
				0, 1, 0, -e.y,
				0, 0, 1, -e.z,
				0, 0, 0, 1
			);
			return m1 * m2;
		}

		inline Matrix44 MakeCameraCanonicalView(float fovVert, float widthOverHeight, float nearPlane, float farPlane) {
			// Implicitly assumes height = 1

			// Create a projection that takes points from (x,y,z) to (x/z, y/z, ~z)
			auto proj = Matrix44(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 1, // z = z + w
				0, 0, 1, 0 // w = z
			);

			// Scale x and y based on fov and scale/translate z based on planes ([n, f] -> [0, 1])
			auto y_fov_scale = 1 / tan(fovVert / 2);
			auto x_fov_scale = y_fov_scale * 1 / widthOverHeight;
			auto n = nearPlane;
			auto f = farPlane;
			auto fov_scale = Matrix44(
				x_fov_scale, 0, 0, 0,
				0, y_fov_scale, 0, 0,
				0, 0, 1 / (f - n), -n / (f - n),
				0, 0, 0, 1
			);

			return fov_scale * proj;
		}
	}
}
