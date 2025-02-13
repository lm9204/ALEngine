# AfterLife Engine
## Project Architecture


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