Super Fast Sort
=

## 전체 파일 구조 설명:
### gpu_sort 폴더:
gpu를 사용하는 바이토닉 정렬 코드가 들어있습니다. 코드만 존재하고, cmake 프로젝트에 포함되지 않습니다.
실행하려면 cmake를 수정해야 합니다. (백업용)

### includes 폴더:
모든 헤더 파일이 들어있습니다.

헤더 파일들 중 data_generator.h, utility.h, sort_algorithms.h 세 개의 파일만 
목적에 맞게 인클루드 해서 사용하면 됩니다.   
나머지 헤더 파일들은 내부적인 사용을 위해 존재하는 헤더 파일입니다.

sort_algorithms.h 파일이 가장 중요합니다.
모든 정렬 알고리즘들의 전방 선언이 있고, 각 알고리즘에 대한 설명도 들어있습니다.
해당 파일을 인클루드 해서 정렬 알고리즘을 사용하면 됩니다.

### sort_algorithms 폴더:
각 정렬 알고리즘이 cpp 파일로 들어있습니다. 원하는 정렬 알고리즘의 구현을 확인하실 수 있습니다.

intro_sort_utility.cpp 파일은 인트로 정렬 알고리즘에 쓰이는 부수적인 함수들이 들어있습니다.

### utility 폴더:
스레드 풀, 데이터 생성기, 시간 측정, 배열 정렬 확인 등 유틸리티 함수들이 모여 있습니다.
