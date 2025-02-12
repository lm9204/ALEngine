# Rendering Engine

Vulkan api을 활용하여 총 4개의 RenderPass로 구성된 렌더링 시스템 구축.  
**Background Pass, Shadow Map Pass, Main RenderPass (Deferred Shading), GUI Pass**의 순서로 렌더링 진행   
Main RenderPass 내에서는 **2개의 Subpass**가 존재  

## Rendering Data Flow
1. **Background Pass**: HDR 이미지를 Cubemap으로 변환하고 Background를 렌더링
2. **Shadow Map Pass**: 각 광원 유형(Spotlight, Directional Light, Point Light)에 대한 Shadow Map 생성
3. **Main RenderPass (Deferred Shading)**:
   - **Subpass 1 - Geometry Pass**: G-Buffer 생성 (Position, Normal, Albedo, PBR 정보 저장)
   - **Subpass 2 - Lighting Pass**: G-Buffer 데이터 및 Shadow Map을 활용한 조명 연산 및 합성
4. **GUI Pass**: 최종 렌더링 결과를 ImGui를 통해 Viewport에 출력

---

## Background Pass
### HDR → Cubemap 변환
- HDR 이미지를 **Cubemap Texture로 변환** (초기 한번 변환)

### Skybox 렌더링
- Background를 별도 렌더패스로 렌더링
- 최종 이미지가 Lighting Pass의 Subpass로 전달되어 합성됨

---

## Shadow Map Pass
- Depth-Only Pass를 활용하여 ShadowMap 생성
- Spotlight 및 Directional Light는 2D Depth Texture 사용
- Point Light는 CubeMap 형태로 ShadowMap 생성

| 광원 유형        | Shadow Map 타입        |
|----------------|-----------------|
| Directional Light | Depth Texture (2D) |
| Spot Light        | Depth Texture (2D) |
| Point Light      | CubeMap Depth Texture (6면) |

---

## Main RenderPass (Deferred Shading)
### Geometry Pass (Subpass 1)
- Vulkan API를 사용하여 **Subpass**를 활용해 G-Buffer를 생성
- Fragment Shader에서 Position, Normal, Albedo, PBR 정보를 각각의 렌더 타겟에 저장
- **Attachment**에 데이터를 저장하여 다음 Subpass로 전달

### G-Buffer 구성 요소
| Buffer  | 역할 |
|---------|----------------------|
| Position | 월드 공간 좌표 |
| Normal   | 월드 공간 법선 |
| Albedo   | 표면 기본 색상 |
| PBR      | Roughness, Metallic, AO |

### Lighting Pass (Subpass 2)
- Vulkan API를 활용하여 **Geometry Pass의 G-Buffer 데이터를 Subpass로 직접 전달받음**
- ShadowMap을 사용하여 **그림자 처리** 
- 최대 4개의 광원까지 실시간 그림자 가능
- Background (Skybox Pass에서 생성된 이미지)를 조명과 함께 합성

### 조명 모델
- PBR 기반 조명 계산 (GGX BRDF 사용)
- **입력값:** Position, Normal, Albedo, Roughness, Metallic, AO
- **출력값:** 조명이 반영된 최종 이미지

---

## GUI Pass
### ImGui를 통한 결과 출력
- Vulkan API를 활용하여 **Swapchain Image로 제출**
- Lighting Pass에서 최종적으로 생성된 이미지를 GUI Viewport에 렌더링
- 사용자 친화적인 UI 제공 노력

