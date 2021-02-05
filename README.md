# Minishell_Toyproject In MCNL
## multiprocess 기반 원격 mini shell 프로그램 작성
### - 일종의 Shell, Telnet 같은 프로그램 구현
### - 서버 / 클라이언트 구조
### - 클라이언트가 서버에 접속하려면 서버는 클라이언트를 핸들링 할 수 있는 프로세스 생성
### - 클라이언트의 명령어를 서버에서 실행(e.g. ls, cd, cat, cp 등)하고 실행결과를 클라이언트 화면에 출력
