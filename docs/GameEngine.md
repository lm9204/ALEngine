# AfterLife Engine
이 세계에는 없는 뛰어난 성능을 가진 엔진을 개발하고 싶다는 마음을 담아 AfterLife Engine 이라는 이름을 붙였습니다. ALEngine은 Renderer, Physics, Animation, 간단한 Scripting이 지원되는 Game Engine입니다. 

## Engine Loop
### 입력 이벤트 처리
- Window, Key, Mouse 입력 이벤트 처리
- 각 이벤트가 발생함에 따라 등록된 Handler가 작동
### 게임 로직 처리
- Game Script를 Native Code로 변환
### 물리 계산
- Rigidbody를 갖고 있는 Entity에 대해 물리 연산 적용
### 오브젝트 상태, 위치 업데이트
- 물리 계산이 적용된 위치, 회전 결과를 오브젝트에 업데이트
### 카메라 위치, 시점
- Scene State에 따라 Camera 선택 (Editor Camera / Scene Caemra)
### 가시성 판단, 최적화
- Frustum Culling 적용 (예정)
### 렌더링
- MeshRenderer를 갖고 있는 Entity 렌더링

## Editor
### UI
- Scene Hierarchy : Scene Entity들의 계층적 구조
- Inspector : Entity Component 정보를 표시
- Content Browser : Project의 작업 공간 표시
### Utils (단축키)
- Ctrl + S : Scene 저장
- Ctrl + Shift + S : Scene 이름 지정 저장
- Ctrl + D : Object 복제
- Ctrl + N : New Scene 열기
- ESC : 프로그램 종료

## Scripting
### mono
- mono 라이브러리를 활용해 간단한 C# Scripting 구현
- Scripting의 규칙을 정의한 Script Core, 실제 코드를 작성한 Script Module로 이루어짐

## Scene
### yaml-cpp
- yaml-cpp를 활용해 Scene을 serialize, deserialize 가능
- .ale 확장자를 가진 파일로 Scene 저장

## ECS (Entity Component System)
### entt
- entt 라이브러리를 활용해 Entity Component System 구현
- TransformComponent, MeshRendererComponent, RigidbodyComponent 등 여러 Component를 정의해, 데이터를 효과적으로 처리 가능
