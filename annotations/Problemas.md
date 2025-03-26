# Identificação de problemas

- [ ] Executar o histórico não esta funcionando
	- `history` resultando em `history: command not found`, em vez de exibir o histórico
- [ ] Execução de redirect aninhado resultando em arquivo vazio
	- `cat Makefile |>out` resultando em arquivo vazio
	- O que é estranho pois quando executado `cat Makefile |>out grep ".c"` ele funciona normalmente
- [ ] exportação de mesma variável está duplicando em vez de atualizar
- [ ] `export var=1 2` esta colocando	o valor 2 como variável de ambiente além de var=1
	- O comando original resulta ein: `export: not an identifier: 2` e colocar var=1 como variável de ambiente
- [ ] expandir variável com colchetes não esta funcionando
	- `echo ${var}` resultando em vazio, em vez de  `1`
- [ ] expandir variável dentro de aspas duplas resulta apenas no valor da variavel
	- `echo "$var 2"` resultando em `1`, em vez de  `1 2`
	- `echo "0$var"` resultando em `0$var`, em vez de  `01`
- [ ] expandir variável dentro de aspas simples esta ocorrendo, mas não deveria
	- `echo '$var 2'` resultando em `1 2`, em vez de  `$var 2`
- [ ] expandir vairiável colocado com aspas não esta funcionando
	- `echo $var"0"` resultando em vazio, em vez de `10`, o inverso também é valido `echo '0'$var` 
- [ ] expandir variável com underscore não esta funcionando
	- `_` está guardando o caminho absoluto do executável, em vez de conter o ultimo argumento executado na linha de comando, exemplo, no shell original, `echo pwd 1` registra `pwd` como saída, então `echo $_` deveria registrar `pwd` na saída, e se executado `$_` deveria executar como comando ...
- [ ] Echo em `$0` não esta retornando o nome do arquivo
	- `echo $0` resultando em nada, em vez do nome `minishell`
- [ ] PWD com mais de um argumento com comportamento diferente
	- `pwd 1` resultando na exibição da pasta, em vez de `pwd: too many arguments`
- [ ] UNSET sem argumento com comportamento diferente
	- `unset` resultando em `unset: not enough arguments`, em vez de `unset: not an identifier:`
- [ ] Navegar em diretórios usando apenas pontos com comportamentos diferentes
	- `.` resultando em `execve: Permission denied`, em vez de `.: not enough arguments`
	- `..` resultando em `execve: Permission denied`, em vez de retroceder a pasta anterior
- [ ] Navegar com CD com diversos pontos com comportamentos diferentes
	- `cd ...` resultando em `cd: No such file or directory`, em vez de navegar para dois diretórios acima
	- `cd ..` resultando em `cd: too many arguments`, em vez de retroceder a pasta anterior
- [ ] Exportação sem o sinal	de igual processa mas exporta a variável sem o sinal de igual
	- `export var` resultando em `var`, em vez de `var=` ... consequentemente o `unset var` não funciona por não ter o sinal de igual

## Cenários Estranhos

> Faz sentido cobrir?

- [ ] Echo usado por ultimo em pipe tem comportamento diferente
	- Quando o echo vai como ultimo comando em um pipe, no shell original exibe a saída logo de imeadiato, no minishell ele aguarda o comando anterior finalizar
	- [ ] `cat | echo 1` original exibi `1` e finaliza a leitura após o primeiro argumento lido, o minishell espera o delimitador
	- [ ] `sleep 2 | echo 3` original exibe	o `3` primeiro antes de processar o `sleep`, o	minishell aguarda o sleep primeiro