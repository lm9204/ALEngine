
# Skinning
*모델의 스켈레톤 애니메이션 기능은 리깅된 본(Bone) 정보를 활용해 애니메이션을 구현하는 핵심 요소입니다. 각 본은 자식 본들을 가지는 계층적 구조로 구성되며, 부모 본의 변환 정보를 재귀적으로 반영하여 최종 글로벌 행렬을 계산합니다.*

주요 처리 과정
	-	본 및 스켈레톤 구성:	모델의 리깅 정보를 기반으로 각 본을 추출하고, 부모-자식 관계를 설정하여 스켈레톤 구조를 구성합니다.
	-	키프레임 데이터 업로드: 애니메이션 데이터로부터 각 본에 대한 채널을 구성하고, 주어진 Timestep 기준으로 키프레임 데이터를 추출하여 각 본에 업로드합니다.
	- 재귀적 연산을 통한 로컬 및 글로벌 행렬 계산:
	-	각 본은 자신의 로컬 변환(키프레임 보간 결과)을 계산한 후, 부모 본과의 연산을 통해 글로벌 변환 행렬을 산출합니다.
	-	계산된 행렬은 GPU의 UBO(Uniform Buffer Object)에 업로드되어, 버텍스 셰이더에서 사용됩니다.

# Blending

애니메이션 블렌딩은 두 개 이상의 애니메이션 간 전환을 자연스럽게 처리하는 기능입니다.
이 기능은 CPU에서 두 애니메이션의 결과를 혼합한 후, GPU의 UBO에 업로드하여 일반 스키닝 연산과 동일한 방식으로 처리됩니다.

주요한 **Blend Factor (블렌딩 비율)**은 ( 진행된 시간 / 전환 시간 )의 비율로 연산됩니다. 

주요 처리 과정
	- 트랜지션 및 스테이트 기반 동작:
	- 외부의 트리거(예: 특정 입력, 상태 변경 요청)에 따라 매 프레임 블렌딩 연산을 수행합니다.
	- 이전 애니메이션이 만료되면 해당 시점의 포즈를 캡처하여 새 애니메이션과 부드럽게 전환합니다.
	- 블렌딩 연산: CPU에서 두 애니메이션의 각 본에 대한 변환 행렬을 받아 블렌딩 비율만큼 연산합니다.
	- 혼합된 최종 본 행렬은 GPU의 UBO에 업로드되어, 버텍스 셰이더에서 사용됩니다.

### 블렌딩 스테이트, 트랜지션
```c++
	stateManager->addState({"StateName", "AnimationName", "Looping:boolean", "Interruptible", "defaultBlendTime"});
	
	// lambda
	stateManager->addTransition({
    "FromStateName", "ToStateName",
		[](){
			// lambda function:boolean
		},
		0.5f // blend time:float
	});
	
	// function pointer with no parameter
	stateManager->addTransition({
		"FromStateName", "ToStateName",
		isNoInput, // function pointer
		0.5f
	});
	
	// function pointer with parameter
	stateManager->addTransition({
		"FromStateName", "ToStateName",
		std::bind(someCondition, m_Window->getNativeWindow(), 42), // bind parameter to function pointer
		0.5f
	});
```

### 블렌딩 외부 요청
```c++
// somewhere loop
while(condition)
{
  if (someCondition)
    stateManager->pushStateChangeRequest("TargetStateName");
  ...
}
```

# Instancing

애니메이션 인스턴싱은 동일한 애니메이션 데이터를 여러 엔티티가 공유하면서, 효율적으로 업데이트하고 렌더링할 수 있도록 하는 기술입니다.
특히 SAComponent를 활용해 각 엔티티의 애니메이션 정보를 저장하고, 데이터 동기화 및 최적화를 통해 성능 저하를 최소화합니다.

주요 처리 과정
	- SAComponent를 통한 애니메이션 정보 저장:
	- 각 엔티티는 SAComponent에 선택된 애니메이션, 현재 시간(Timestep), 진행된 키프레임 데이터, 루핑 변수 등 애니메이션 관련 정보를 저장합니다.
	- 애니메이션 데이터 업로드 및 본 데이터 갱신:
	- SAComponent에 저장된 데이터를 기반으로, 선택된 애니메이션의 키프레임 데이터를 각 본에 업로드합니다.
	- 이후, 각 본의 로컬 변환 행렬을 계산하기 위해 자식 본과의 재귀 연산을 수행하며, 계산된 데이터는 UBO를 통해 GPU에 전송됩니다.
	- 버텍스 셰이더에서는 업로드된 본 데이터를 사용하여 보간 처리(interpolation)를 진행합니다.
	- 시간 데이터 업데이트:
	- 각 프레임마다 기존 시간에 Timestep 만큼의 시간을 더해 진행된 데이터를 SAComponent에 다시 저장합니다.
