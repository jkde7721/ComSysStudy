import re
from xml.sax.saxutils import escape
from constant import *

class JackTokenizer:


    def __init__(self, inputfile):
        self.file = open(inputfile,'r')
        self.lines = self.file.read()
        self.lines = self.remove_comments(self.lines)
        # keyword, delim, num, str, id의 표현식과 일치하느 것들 리스트로 반환
        self.words = self._word.findall(self.lines) 
        self.tokens = []
        for word in self.words:
            type = self.tokenType(word)
            #if type == T_NUM:
            #    word = int(word)
            #if type==T_STR:
            #    word = word[1:-2]
            
            self.tokens.append((type,word))
        self.cur_token_type = T_ERROR
        self.cur_token_val = 0
        #print(self.tokens)

    def fileclose(self):
        self.file.close()
    """
    // 한줄 주석
    /* */ -> (/** */ 포함)
    DOTALL(S) - . 이 줄바꿈 문자를 포함하여 모든 문자와 매치할 수 있도록 한다.
    MULTILINE(M) - 여러줄과 매치할 수 있도록 한다. (^, $ 메타문자의 사용과 관계가 있는 옵션이다)
    """
    _comment = re.compile(r'//[^\n]*\n|/\*(.*?)\*/', re.MULTILINE|re.DOTALL)
    def remove_comments(self, line):
        return self._comment.sub('', line) #주석 삭제
    
    _keyword_re = '|'.join(keywords)
    _sym_re = '['+re.escape(symbols)+']'
    _num_re = r'\d+'
    _str_re = r'"[^"\n]*"'
    _id_re = r'[\w\-]+'
    # keyword, delim, num, str, id 모두 포함
    _word = re.compile(_keyword_re+'|'+_sym_re+'|'+_num_re+'|'+_str_re+'|'+_id_re)
    
    def tokenType(self, word):
        if re.match(self._keyword_re, word) != None:
            return T_KEYWORD
        elif re.match(self._sym_re, word) != None:
            return T_SYM
        elif re.match(self._num_re, word) != None:
            return T_NUM
        elif re.match(self._str_re, word) != None:
            return T_STR
        elif re.match(self._id_re,  word) != None:
            return T_ID
        else:
            return T_ERROR




    
    def hasMoreTokens(self):
        return False if len(self.tokens) == 0 else True

    def advance(self):
        if self.hasMoreTokens():
            self.cur_token_type, self.cur_token_val = self.tokens.pop(0)
        

    def keyword(self):
        return self.cur_token_val
        
    def symbol(self):
        if self.cur_token_val == '<':
            return '&lt;'
        elif self.cur_token_val == '>':
            return '&gt;'
        elif self.cur_token_val == '\"':
            return '&quot;'
        elif self.cur_token_val == '&':
            return '&amp;'
        return self.cur_token_val
        
    def identifier(self):
        return self.cur_token_val
        
    def int_val(self):
        return int(self.cur_token_val)
        
    def string_val(self):
        return self.cur_token_val