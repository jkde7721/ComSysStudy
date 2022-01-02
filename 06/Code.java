package comsys;

public class Code {
    /*
    public static void main(String[] args) {
        Code code = new Code();
        System.out.println("dest 필드");
        System.out.println(code.dest(""));
        System.out.println(code.dest("M"));
        System.out.println(code.dest("D"));
        System.out.println(code.dest("MD"));
        System.out.println(code.dest("A"));
        System.out.println(code.dest("AM"));
        System.out.println(code.dest("AD"));
        System.out.println(code.dest("AMD"));

        System.out.println("\ncomp 필드");
        System.out.println(code.comp("0"));
        System.out.println(code.comp("1"));
        System.out.println(code.comp("-1"));
        System.out.println(code.comp("D"));
        System.out.println(code.comp("A"));
        System.out.println(code.comp("!D"));
        System.out.println(code.comp("!A"));
        System.out.println(code.comp("-D"));
        System.out.println(code.comp("-A"));
        System.out.println(code.comp("D+1"));
        System.out.println(code.comp("A+1"));
        System.out.println(code.comp("D-1"));
        System.out.println(code.comp("A-1"));
        System.out.println(code.comp("D+A"));
        System.out.println(code.comp("D-A"));
        System.out.println(code.comp("A-D"));
        System.out.println(code.comp("D&A"));
        System.out.println(code.comp("D|A"));
        System.out.println(code.comp("M"));
        System.out.println(code.comp("!M"));
        System.out.println(code.comp("-M"));
        System.out.println(code.comp("M+1"));
        System.out.println(code.comp("M-1"));
        System.out.println(code.comp("D+M"));
        System.out.println(code.comp("D-M"));
        System.out.println(code.comp("M-D"));
        System.out.println(code.comp("D&M"));
        System.out.println(code.comp("D|M"));

        System.out.println("\njump 필드");
        System.out.println(code.jump(""));
        System.out.println(code.jump("JGT"));
        System.out.println(code.jump("JEQ"));
        System.out.println(code.jump("JGE"));
        System.out.println(code.jump("JLT"));
        System.out.println(code.jump("JNE"));
        System.out.println(code.jump("JLE"));
        System.out.println(code.jump("JMP"));
    }
    */

    public String dest(String destCmd) {
        int d1 = 0, d2 = 0, d3 = 0;
        String result = "";
        if(destCmd.contains("A")) d1 = 1;
        if(destCmd.contains("D")) d2 = 1;
        if(destCmd.contains("M")) d3 = 1;
        result = "" + d1 + d2 + d3;
        return result;
    }

    public String comp(String compCmd) {
        int a = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0;
        String result = "";
        if(compCmd.contains("M")) a = 1;
        if(!compCmd.contains("D")) c1 = 1;
        if(a == 0 && !compCmd.contains("A")) c3 = 1;

        if(compCmd.equals("0")) { c2=0; c4=0; c5=1; c6=0; }
        else if(compCmd.equals("1")) { c2=1; c4=1; c5=1; c6=1; }
        else if(compCmd.equals("-1")) { c2=1; c4=0; c5=1; c6=0; }
        else if(compCmd.equals("D")) { c2=0; c4=1; c5=0; c6=0; }
        else if(compCmd.equals("A") || compCmd.equals("M")) { c2=1; c4=0; c5=0; c6=0; }
        else if(compCmd.equals("!D")) { c2=0; c4=1; c5=0; c6=1; }
        else if(compCmd.equals("!A") || compCmd.equals("!M")) { c2=1; c4=0; c5=0; c6=1; }
        else if(compCmd.equals("-D")) { c2=0; c4=1; c5=1; c6=1; }
        else if(compCmd.equals("-A") || compCmd.equals("-M")) { c2=1; c4=0; c5=1; c6=1; }
        else if(compCmd.equals("D+1")) { c2=1; c4=1; c5=1; c6=1; }
        else if(compCmd.equals("A+1") || compCmd.equals("M+1")) { c2=1; c4=1; c5=1; c6=1; }
        else if(compCmd.equals("D-1")) { c2=0; c4=1; c5=1; c6=0; }
        else if(compCmd.equals("A-1") || compCmd.equals("M-1")) { c2=1; c4=0; c5=1; c6=0; }
        else if(compCmd.equals("D+A") || compCmd.equals("D+M")) { c2=0; c4=0; c5=1; c6=0; }
        else if(compCmd.equals("D-A") || compCmd.equals("D-M")) { c2=1; c4=0; c5=1; c6=1; }
        else if(compCmd.equals("A-D") || compCmd.equals("M-D")) { c2=0; c4=1; c5=1; c6=1; }
        else if(compCmd.equals("D&A") || compCmd.equals("D&M")) { c2=0; c4=0; c5=0; c6=0; }
        else if(compCmd.equals("D|A") || compCmd.equals("D|M")) { c2=1; c4=1; c5=0; c6=1; }

        result = "" + a + c1 + c2 + c3 + c4 + c5 + c6;
        return result;
    }

    public String jump(String jumpCmd) {
        byte j1 = 0, j2 = 0, j3 = 0;
        String result = "";

        if(jumpCmd.contains("L")) j1 = 1;
        if(jumpCmd.contains("E")) {
            j2 = 1;
            if(jumpCmd.contains("N")) {
                j1 = 1; j2 = 0; j3 =1;
            }
        }
        if(jumpCmd.contains("G")) j3 = 1;
        if(jumpCmd.equals("JMP")) {
            j1 = 1; j2 = 1; j3 = 1;
        }
        result = "" + j1 + j2 + j3;
        return result;
    }
}
