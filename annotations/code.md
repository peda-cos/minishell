## Considerações sobre o	código

### Indagações

- Por que não chamar os comandos de builtin	diretamente no `execve`?
	- Seria por que o subject pede eles limitados (sem opções)?
- Qual foi a necessidade `copy_env`?
	- Seria por que está alterando `env`? Se sim, por que não utilizar ela mesma?
- O uso de `fprintf` pode ser utilizado? Não está nas funções permitidas!
- `ft_strcmp` não deveria ter proteção contra parâmetros `NULL`?
- `(*env)[idx] = new_entry` conseguirá armazenar qualquer valor?
- Linha `164` do arquivo `builtins.c` era para ter um `else`?
- `signal_handler` não deveria resetar `g_signal_received` se o sinal não for `SIGINT`?
- Sobre arquivo `tokenizer.c`
	- Instruções while que avançam o indíce, um `if` com condição de `continue` não seria mais eficiente?
	- Linha 57:  vai ser executada?
	- Linha 58: `if` é realmente necessário?
- Linha 233 do arquivo `executor.c` porque da execução passando o mesmo valor? `dup2(0, STDIN_FILENO);`

### Não entendi

- Entender melhor a função `expand_variables`.
- Entender melhor as funções `rl_*` utilizadas em `signal_handler`.
- Entender melhor `SIGQUIt` e `SIG_IGN`

### Anotações

- A função `tokenize_input` quebra a string de entrada em tokens, separando-os por espaços e	tabs e atribuindo um tipo ao token armazenado.

## Observações em testes

- Builtins seriam sem opções ... (com exceção de `echo`)
	- Então os comandos não deveriam ter logica quando for passado com argumentos?
- `export` do shell está diferente do export diretamente do terminal
- `export <key>=<value>` não está atualizando	a variável quando chamado mais	de uma vez.
	- A chave está sendo adicionada novamente em vez de atualizar o valor.1
- Validar em ambiente linux (em vez de WSL)
	- Pipe com sleep com comportamento diferente da execução original
- `cd` com mais de um argumento com comportamento diferente do shell original (o original retorna mensagem de error)
- `echo` com caracteres especiais diferente do shell original