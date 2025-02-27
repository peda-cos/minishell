# Checklist para a implementação do Minishell

### 1. **Comportamento básico do shell**
- [x] Exibir um prompt ao aguardar um novo comando.
- [x] Implementar histórico de comandos.
- [x] Buscar e lançar o executável correto com base na variável PATH ou usando caminho relativo ou absoluto.
- [x] Evitar o uso de mais de uma variável global para indicar um sinal recebido.
  - [x] Certificar-se de que a variável global usada só armazene o número do sinal recebido.
  - [x] Garantir que a variável global não contenha acessos a outras estruturas de dados.

### 2. **Interpretação de caracteres especiais**
- [x] Não interpretar aspas não fechadas ou caracteres especiais não exigidos pelo enunciado, como `\` (barra invertida) ou `;` (ponto e vírgula).
- [x] Interpretar corretamente o caractere `'` (aspas simples) para evitar interpretação de metacaracteres dentro da sequência.
- [x] Interpretar corretamente o caractere `"` (aspas duplas) para evitar interpretação de metacaracteres, exceto o `$` (sinal de cifrão).

### 3. **Redirecionamentos**
- [x] Implementar o redirecionamento `<` para redirecionamento de entrada.
- [x] Implementar o redirecionamento `>` para redirecionamento de saída.
- [ ] Implementar o redirecionamento `<<` que:
  - [ ] Recebe um delimitador.
  - [ ] Lê a entrada até encontrar uma linha contendo o delimitador.
  - [ ] Não atualiza o histórico de comandos.
- [x] Implementar o redirecionamento `>>` para redirecionamento de saída em modo de anexação (append).

### 4. **Pipes**
- [x] Implementar o uso de pipes `|` para conectar a saída de um comando ao input do próximo comando.

### 5. **Variáveis de ambiente**
- [x] Implementar a expansão de variáveis de ambiente, quando o caractere `$` for seguido por uma sequência de caracteres.
- [x] Implementar a expansão do ` $? ` que deve se expandir para o status de saída do pipeline em primeiro plano mais recentemente executado.

### 6. **Comportamento de sinais**
- [x] Implementar o comportamento dos sinais `ctrl-C`, `ctrl-D` e `ctrl-\` conforme o comportamento do bash.
  - [x] `ctrl-C` deve exibir um novo prompt em uma nova linha.
  - [x] `ctrl-D` deve sair do shell.
  - [x] `ctrl-\` não deve fazer nada.

### 7. **Builtins**
- [x] Implementar os seguintes comandos internos (builtins):
  - [x] **echo** com a opção `-n`.
  - [x] **cd** com caminho relativo ou absoluto.
  - [x] **pwd** sem opções.
  - [x] **export** sem opções.
  - [x] **unset** sem opções.
  - [x] **env** sem opções ou argumentos.
  - [x] **exit** sem opções.
