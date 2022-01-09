package comsys.vm;

public class WrongCmdTypeException extends Exception {
    @Override
    public String getMessage() {
        return "해당 명령어 타입에서는 현재 호출된 함수 사용이 불가능합니다.";
    }
}
