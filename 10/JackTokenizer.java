package comsys.compiler;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class JackTokenizer {

    private BufferedReader bufferedReader;
    private List<String> totalTokens;
    private boolean isCmt = false;
    private static String curToken;

    public JackTokenizer(String filePath) {
        try {
            this.bufferedReader = new BufferedReader(new FileReader(filePath));
        } catch (Exception e) {}
    }

    public void tokenize() throws IOException {
        String delim = "{}()[].,;+-*/&|<>=~";

        totalTokens = bufferedReader.lines()
                .map(s -> s.replaceAll("//.*", "").trim())
                .map(s -> {
                    if(isCmt || s.startsWith("/*")) {
                        isCmt = !s.endsWith("*/");
                        return "";
                    }
                    else return s;
                })
                .flatMap(s -> {
                    StringTokenizer tokenizer = new StringTokenizer(s, delim, true);
                    List<String> tokens = new ArrayList<>();
                    // 여기서 hasMoreTokens 는 API 함수
                    while(tokenizer.hasMoreTokens()) {
                        String token = tokenizer.nextToken().trim();
                        if(token.startsWith("\"")) tokens.add(token);
                        else tokens.addAll(Arrays.asList(token.split(" ")));
                    }
                    return tokens.stream();
                })
                .filter(s -> !s.equals(""))
                .collect(Collectors.toList());

        bufferedReader.close();
    }

    public boolean hasMoreTokens() {
        return !totalTokens.isEmpty();
    }

    public void advance() {
        curToken = totalTokens.remove(0);
    }

    public String lookupNext() {
        curToken = totalTokens.get(0);
        return curToken;
    }

    // 현재 curToken 의 타입 반환
    public static TokenType tokenType() {
        Stream<String> keywordStream = Arrays.stream(Keyword.values()).map(Keyword::getValue);

        if(keywordStream.anyMatch(keyword -> keyword.equals(curToken.toLowerCase()))) return TokenType.KEYWORD;
        else if(curToken.matches("[{}()\\[\\].,;+\\-*/&|<>=~]")) return TokenType.SYMBOL;
        else if(curToken.matches("[0-9]+")) return TokenType.INT_CONST;
        else if(curToken.matches("^\".*\"$")) return TokenType.STRING_CONST;
        else return TokenType.IDENTIFIER;
    }

    public static Optional<Keyword> keyword() {
        if(tokenType() == TokenType.KEYWORD)
            return Arrays.stream(Keyword.values()).filter(k -> k.getValue().equals(curToken.toLowerCase())).findFirst();

        return Optional.empty();
    }

    public static String symbol() {
        switch (curToken) {
            case "<": return "&lt;";
            case ">": return "&gt;";
            case "\"": return "&quot;";
            case "&": return "&amp;";
            default: return curToken;
        }
    }

    public static String identifier() {
        return curToken;
    }

    public static int intVal() {
        return Integer.parseInt(curToken);
    }

    public static String stringVal() {
       return curToken.substring(1, curToken.length() - 1); // " " 문자 삭제
    }
}
