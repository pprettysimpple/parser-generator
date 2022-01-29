grammar Format;

@header {
    #include "../../../../src/grammar/grammar.h"
    #include "../../../../src/grammar/nterm.h"
    #include "../../../../src/grammar/term.h"
    #include "../../../../src/grammar/rule.h"
}

gram[grammar_t g]:
    gram_def[g] header[g] terms[g] start_state[g] nterms[g];

gram_def[grammar_t g]:
    '@grammar' ID {$g.set_name($ID.text);} ';';

header[grammar_t g]:
    '@header' rule_code {$g.add_header_code($rule_code.s);};

terms[grammar_t g] :
    '@terms'
    (term_rule[g])*;

term_rule[grammar_t g]:
    ID ':' REGEXP ';' {
        $g.add_term($ID.text, $REGEXP.text);
    };

start_state[grammar_t g]:
    '@start' ID {$g.set_start($ID.text);} ';';

nterms[grammar_t g]:
    '@rules' (nterm {$g.add_nterm($nterm.nonterm);})+;

nterm returns [nterm_t nonterm]:
    ID {$nonterm.set_name($ID.text);}
    ('[' nterm_params[$nonterm] ']')?
    ('returns' '[' nterm_returns[$nonterm] ']')?
    ':' rule1=nterm_rule {$nonterm.add_rule($rule1.r);}
    ('|' ((rule2=nterm_rule {$nonterm.add_rule($rule2.r);}) |
     ({$nonterm.add_rule(rule_t(null_rule));}))
     )* ';';

nterm_params[nterm_t nonterm]:
    type1=ID name1=ID {$nonterm.add_param($type1.text, $name1.text);}
    (',' type2=ID name2=ID {$nonterm.add_param($type2.text, $name2.text);})*;

nterm_returns[nterm_t nonterm]:
    type=ID name=ID {$nonterm.add_return($type.text, $name.text);}
        (',' type1=ID name1=ID {$nonterm.add_return($type1.text, $name1.text);})*;

nterm_rule returns [rule_t r] locals [std::string params, std::string code]:
    (ID ('[' rule_params {$params = $rule_params.s;}']')? (rule_code {$code = $rule_code.s;})? {
        $r.add($ID.text, $params, $code);
        $params = "";
        $code = "";
    })+;

rule_params returns[std::string s]:
    id1=ID {$s += $id1.text;} (',' id2=ID {$s += ", " + $id2.text;})*;

rule_code returns[std::string s]:
    SOURCE_CODE {$s = $SOURCE_CODE.text.substr(1, $SOURCE_CODE.text.size() - 2);}; // cut dollars

SOURCE_CODE: '$' (~[$])+ '$';
SKIP_WHITESPACES: [ \n\r\t]+ -> skip;
INT: [0-9]+;
ID: [a-zA-Z][a-zA-Z0-9_]*;

REGEXP: '"' (~('"'))* '"';
