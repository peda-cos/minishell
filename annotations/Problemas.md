# Identificação de problemas

- [x] ~~Exportação de variável com mais de um sinal de igual~~
	- ~~`export var==1` resultando em nada, em vez de `var="=1"` diferente do bash~~
- [x] ~~UNSET sem argumento com comportamento diferente~~
	- ~~`unset` resultando em `unset: not enough arguments`, em vez de não acontecer nada no bash~~
- [x] ~~Exportação de mesma variável está duplicando em vez de atualizar~~
- [x] ~~Expandir variável dentro de aspas duplas resulta apenas no valor da variável~~
	- ~~`echo "$var 2"` resultando em `1`, em vez de  `1 2`~~
	- ~~`echo "0$var"` resultando em `0$var`, em vez de  `01`~~
- [x] ~~Expandir variável colocado com aspas não esta funcionando~~
	- ~~`echo $var"0"` resultando em vazio, em vez de `10`, o inverso também é valido `echo '0'$var`~~
- [x] ~~Redirecionamento IN e OUT isolados, resultando em segment fault~~
	- ~~`< in` ou `> out` com comportamento diferente do bash~~ 

- [	] Exportação de variável com caracteres diferentes de alfanuméricos
	- `export v@r=1` sendo processado, em vez de resultar em `bash: export: 'v@ar': not a valid identifier` no bash
- [ ] Executar o histórico não esta funcionando
	- `history` resultando em `history: command not found`, em vez de exibir o histórico
	- Validar se `<<` não esta atualizando o histórico
- [ ] Execução de redirect aninhado resultando em arquivo vazio
	- `cat Makefile |>out` resultando em arquivo vazio
	- O que é estranho pois quando executado `cat Makefile |>out grep ".c"` ele funciona normalmente
- [ ] `export var=1 2` esta colocando	o valor 2 como variável de ambiente além de `var=1`
	- O bash resulta ein: `bash: export: '2': not a valid identifier` e coloca `var=1` como variável de ambiente
- [ ] Expandir variável dentro de aspas simples esta ocorrendo, mas não deveria
	- `echo '$var 2'` resultando em `1 2`, em vez de  `$var 2`
- [ ] Expandir variável com aspas simples dentro de aspas duplas divergente do bash
	- `echo "'$var'0"` resultando em `'$var'0`, em vez de `10`
	- `echo "'$var$var'"` resultando em `'$var$var'`, em vez de `11`
- [ ] Redirecionamentos com múltiplos pipes não estão funcionando
	- `echo 123 > a > b >> c` processa apenas o ultimo redirecionamento
- [ ] Comando `exit` deveria registrar "exit" no terminal?
	- "Validar se é um comportamento do `WSL` do windows ou se no unix acontece a mesma coisa
- [ ] Redirecionamento IN e OUT isolados `< in` ou `> out`, pendentes de implementação


## [[`IN_EVAL?`]] Vale a pena cobrir? 

- [ ] Expandir variável com underscore não esta funcionando
	- `_` está guardando o caminho do executável, em vez de conter o ultimo argumento executado na linha de comando, exemplo, no bash, `echo pwd` registra `pwd` como saída, então `echo $_` deveria registrar `pwd` também, e se executado `$_` deveria executar como comando ...
- [ ] Echo em `$0` não esta retornando o nome do arquivo
	- `echo $0` resultando em nada, em vez do nome `minishell`
- [ ] Expandir variável com colchetes não esta funcionando
	- Deveria ser coberto `{}` ? Não diz que sim e nem que não o SUBJECT
	- `echo ${var}` resultando em vazio, em vez de `1`
- [ ] Navegar em diretórios usando apenas pontos com comportamentos diferentes
	- `.` resultando em `execve: Permission denied`, em vez de `bash: .: filename argument required`
	- `..` resultando em `execve: Permission denied`, em vez de `..: command not found`
- [ ] Execução de ´.´ seguido de um arquivo, não esta funcionando
	- `. file.sh` resultando em `execve: Permission denied`, em vez de executar o arquivo

## Cenários Estranhos

> Faz sentido cobrir?

- [ ] Echo usado por ultimo em pipe tem comportamento diferente
	- Quando o echo vai como ultimo comando em um pipe, no shell original exibe a saída logo de imediato, no minishell ele aguarda o comando anterior finalizar
	- `cat | echo 1` original exibi `1` e finaliza a leitura após o primeiro argumento lido, o minishell espera o delimitador
	- [ ] `sleep 2 | echo 3` original exibe	o `3` primeiro antes de processar o `sleep`, o	minishell aguarda o sleep primeiro