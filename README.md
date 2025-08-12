# Unreal Engine Dedicated Server 프로젝트
## 플레이 영상


## 프로젝트 소개
해당 프로젝트는 FPS 게임에 데디케이티드 서버 모듈을 추가, AWS 서비스를 이용해 실시간 네트워크 동기화 외에도 회원가입 및 로그인, DB를 활용한 플레이 기록 및 랭킹 시스템을 구현했습니다.

## 작업 개요
1. C++ 기반 데디케이티드 서버(Dedicated Server) 모듈 구현
2. AWS GameLift Anywhere/EC2 Fleet을 활용한 게임 세션 관리 및 서버 배포
3. API Gateway, Lambda, DynamoDB 기반의 **서버리스(Serverless)** 백엔드 아키텍처 구축
4. AWS Cognito 연동을 통한 안전한 **회원가입/로그인** 시스템 구현
5. NoSQL(DynamoDB) 기반 데이터 모델링 및 랭킹 시스템 구축
6. CloudWatch를 통한 실시간 서버 로그 추적 및 에러 원인 분석
7. IAM 최소 권한 원칙에 따른 서비스별 접근 제어 및 보안 강화
8. **심리스 서버 트래블(Seamless Server Travel)** 구현으로 맵 이동 시 끊김 없는 플레이 경험 제공
9. UMG를 활용한 UI 제작
10. ```Data Asset```과 ```Gameplay Tag```를 활용한 유연한 API 엔드포인트 관리 시스템 설계
11. 언리얼 리플렉션 시스템 기반 **JSON 데이터 자동 파싱(Deserialize)** 기능 구현
12. ```PlayerState```을 활용하여 서버 트래블 간 플레이어 데이터 영속성 확보
13. ```LocalPlayerSubsystem``` 토큰 정보 저장 및 주기적인 액세스 토큰 갱신
14. ```GameState``` 및 **Fast TArray 직렬화를 이용한 실시간 접속자 목록 표시**
15. **핑퐁(Ping-Pong) 방식의 RTT(왕복 시간) 계산을 통한 클라이언트-서버 간 타이머 동기화** 
16. ```PlayerState```를 이용한 플레이어 정보(점수, 상태 등) 실시간 복제 및 게임 결과 기록
17. **델리게이트 전파(Broadcast)을 이용한 위젯과 게임 로직 간 결합도 최소화(Decoupling) 설계** 
18. 플레이 레벨 디자인

## 구현 상세
### AWS 기반 서버리스 게임 백엔드 아키텍처

<summary>자세히 보기(클릭)</summary>  
  
<img width="1729" height="812" alt="image" src="https://github.com/user-attachments/assets/c0451e37-f29a-43a9-8ebf-4950bcb09f04" />

