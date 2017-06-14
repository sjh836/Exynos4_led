# 임베디드(태블릿) 프로그래밍
M2모듈을 이용한 LED on/off

![동작사진](http://img1.daumcdn.net/thumb/R1920x0/?fname=http%3A%2F%2Fcfile10.uf.tistory.com%2Fimage%2F2648504B594145881E1F78)

m2\_led 모듈을 push하고 insmod로 모듈을 등록하면, load되자마자 module_init()를 호출한다. 이때 사용자가 정의한 함수를 넣어주면 된다. On-Off의 반복패턴으로 드라이버가 로딩될 떄 LED가 ON되도록 하였다.

이렇게 적재된 모듈을 test코드에서 `int dev = open("/dev/m2_led", O_WRONLY);`로 담고, 실행인자에 맞게 분기처리해주면서 실행해주면 된다.

test실행파일은 동영상이라 첨부를 못했다

## 개발환경
* 타깃 환경: 삼성전자 엑시노스(HBE-SM7-S4412 Enterprise)
* IDE: AndroX Studio (GCC 크로스 툴체인)