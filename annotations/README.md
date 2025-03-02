# Minishell

A existência de shells está ligada à própria existência de TI. Na época, todos os desenvolvedores concordavam que se comunicar com um computador usando interruptores 1/0 alinhados era seriamente irritante.

Foi lógico que eles tiveram a ideia de criar um software para se comunicar com um computador usando linhas interativas de comandos em uma linguagem um pouco próxima à linguagem humana.

Este projeto permite viajar no tempo e voltar aos problemas que as pessoas enfrentavam quando o Windows não existia.

## Regras do Subject

> Funções permitidas: `readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs`

O shell deve:

- Exibir um prompt e esperar por um novo comando.
- Ter um histórico de trabalho.
- Pesquisar e iniciar o executável correto do comando 
	- Com base na variável PATH ou usando um caminho relativo ou absoluto.
- Evitar usar mais de uma variável global para indicar um sinal recebido. Considere as implicações: essa abordagem garante que seu manipulador de sinal não acessará suas estruturas de dados principais.

> A variável global não pode fornecer nenhuma outra informação ou acesso a dados além do número de um sinal recebido.
Portanto, usar estruturas do tipo "norm" no escopo global é proibido.

- Não interpretar aspas não fechadas ou caracteres especiais que não são requeridos pelo assunto como `\` (barra invertida) ou ; (ponto e vírgula).
- Lidar com `’` (aspas simples), pois impede o shell de interpretar os meta-caracteres na sequência entre aspas.
- Lidar com `"` (aspas duplas), pois impede o shell de interpretar os meta-caracteres na sequência entre aspas, exceto `$` (cifrão).
- Implementar redirecionamentos:
	- `<` deve redirecionar a entrada.
	- `>` deve redirecionar a saída.
	- `<<` deve receber um delimitador e, em seguida, ler a entrada até que uma linha contendo o delimitador seja identificado. 
		- No entanto, não precisa atualizar o histórico!
	- `>>` deve redirecionar a saída no modo append.
- Implementar pipes (caractere `|`). A saída de cada comando no pipeline é conectada à entrada do próximo comando por meio de um pipe.
- Lidar com variáveis ​​de ambiente (`$` seguido por uma sequência de caracteres) que devem se expandir para seus valores.
- Lidar com `$?` que deve se expandir para o status de saída do pipeline de primeiro plano executado mais recentemente.
- Lidar com `ctrl-C`, `ctrl-D` e `ctrl-\` que devem se comportar como no bash.
- No modo interativo:
	- `ctrl-C` exibe um novo prompt em uma nova linha.
	- `ctrl-D` sai do shell.
	- `ctrl-\` não faz nada.
- Seu shell deve implementar os seguintes builtins:
	- `echo` com opção `-n`
	- `cd` com apenas um caminho relativo ou absoluto
	- `pwd` sem opções
	- `export` sem opções
	- `unset` sem opções
	- `env` sem opções ou argumentos
	- `exit` sem opções

> A função `readline` pode causar vazamentos de memória. Você não precisa consertá-los. 
> Mas isso não significa que seu próprio código, sim, o código que você escreveu, pode ter vazamentos de memória.

**Parte Bônus**: o programa tem que implementar:

- `&&` e `||` com parênteses para prioridades.
- Caracteres curinga `*` devem funcionar para o diretório de trabalho atual.

## Entendimentos

### Entendimento sobre `variável global`

> A variável global não pode fornecer nenhuma outra informação ou acesso a dados além do número de um sinal recebido

No contexto do projeto Minishell, o uso de variáveis globais deve ser extremamente limitado. 
Especificamente, uma variável global só pode ser usada para armazenar o número de um sinal recebido (como um sinal de interrupção ou término).

Isso implica que a variável global não deve ser usada para armazenar ou fornecer acesso a qualquer outro tipo de informação ou dados.
Essa restrição é importante para garantir que o manipulador de sinais não interfira nas principais estruturas de dados do programa, o que poderia causar comportamentos inesperados ou erros. 

Em resumo, a variável global deve ser usada apenas para sinalização e não para compartilhar dados entre diferentes partes do programa.

### Entendimento sobre `Lidar com $?`

> Lidar com `$?` que deve se expandir para o status de saída do pipeline de primeiro plano executado mais recentemente.

A expressão `$?` é usada em shells Unix para representar o status de saída do último comando executado.
No contexto do projeto Minishell, "Lidar com `$?` que deve se expandir para o status de saída do pipeline de primeiro plano executado mais recentemente.
Significa que o seu shell deve ser capaz de substituir `$?` pelo código de saída do último comando ou pipeline que foi executado em primeiro plano.

Por exemplo, se o último comando executado retornar um código de saída 0 (indicando sucesso), então `$?` deve ser substituído por `0`.
Se o comando retornar um código de erro, como `1`, então `$?` deve ser substituído por `1`. Isso permite que os usuários do shell verifiquem o sucesso ou falha dos comandos executados anteriormente.

> "primeiro plano" refere-se aos comandos ou processos que estão sendo executados diretamente pelo usuário e que têm controle do terminal. 

## Terminologias

- **Shell**: Um shell é um programa que atua como uma interface de usuário para acessar os serviços do sistema operacional. Ele aceita comandos do usuário e os executa, fornecendo feedback ao usuário sobre o resultado das operações. O shell é uma parte essencial de qualquer sistema operacional Unix ou Linux.
- **Prompt**: O prompt é um sinal ou mensagem que indica que o shell está pronto para receber comandos do usuário. Ele geralmente inclui informações como o nome do usuário, o nome do host e o diretório de trabalho atual.
- **Histórico de trabalho**: O histórico de trabalho é uma lista de comandos que foram executados anteriormente no shell. Ele permite que os usuários naveguem e reexecutem comandos anteriores sem precisar digitá-los novamente.
- **Redirecionamento**: O redirecionamento é um recurso que permite que a saída de um comando seja direcionada para um arquivo ou para a entrada de outro comando. Isso é útil para salvar a saída de um comando em um arquivo ou para fornecer a saída de um comando como entrada para outro comando.
- **Pipes**: Os pipes são usados para conectar a saída de um comando à entrada de outro comando. Isso permite que os comandos sejam encadeados juntos para formar pipelines complexos que realizam operações mais avançadas.
- **Variáveis de ambiente**: As variáveis de ambiente são variáveis que são definidas no ambiente do shell e podem ser acessadas por programas em execução no shell. Elas são usadas para armazenar informações como caminhos de diretórios, configurações de programas e outras informações úteis.
- **Builtins**: Builtins são comandos internos ao shell que são executados diretamente por ele, em vez de chamar um programa externo. Eles são usados para fornecer funcionalidades básicas, como navegação de diretórios, manipulação de variáveis de ambiente e controle do shell.
	- **echo**: O comando echo é usado para exibir uma linha de texto no terminal. Ele é frequentemente usado para exibir mensagens de status, variáveis de ambiente e outras informações.
	- **cd**: O comando cd é usado para alterar o diretório de trabalho atual do shell. Ele permite que os usuários naveguem entre diretórios no sistema de arquivos.
	- **pwd**: O comando pwd é usado para exibir o diretório de trabalho atual do shell. Ele fornece o caminho completo para o diretório em que o usuário está atualmente.
	- **export**: O comando export é usado para definir variáveis de ambiente no shell. Ele permite que os usuários definam variáveis que serão acessíveis por programas em execução no shell.
	- **unset**: O comando unset é usado para remover variáveis de ambiente do shell. Ele permite que os usuários limitem o escopo das variáveis de ambiente definidas no shell.
	- **env**: O comando env é usado para exibir as variáveis de ambiente definidas no shell. Ele fornece uma lista das variáveis de ambiente atuais e seus valores.
	- **exit**: O comando exit é usado para encerrar o shell. Ele permite que os usuários saiam do shell e retornem ao prompt do sistema operacional.
- **Funcões permitidas**:
	- **readline**: é usada para ler uma linha de entrada do usuário no shell. Ela fornece uma maneira conveniente de obter comandos do usuário.
	- **rl_clear_history**: é usada para limpar o histórico de comandos do shell. Ela remove todos os comandos anteriores da lista de histórico.
	- **rl_on_new_line**: é usada para notificar o shell de que uma nova linha de entrada foi iniciada. Ela é usada para atualizar o prompt e outras informações do shell.
	- **rl_replace_line**: é usada para substituir a linha de entrada atual do usuário no shell. Ela permite que os usuários editem a linha de entrada antes de enviá-la para execução.
	- **rl_redisplay**: é usada para redesenhar a linha de entrada do usuário no shell. Ela é usada para atualizar a exibição da linha de entrada após alterações.
	- **add_history**: é usada para adicionar um comando ao histórico de comandos do shell. Ela permite que os usuários naveguem e reexecutem comandos anteriores.
	- **printf**: é usada para exibir uma mensagem formatada no terminal. Ela é usada para exibir informações para o usuário.
	- **malloc**: é usada para alocar memória dinamicamente no shell. Ela é usada para criar buffers e estruturas de dados temporárias.
	- **free**: é usada para liberar a memória alocada dinamicamente no shell. Ela é usada para evitar vazamentos de memória.
	- **write**: é usada para escrever dados em um arquivo ou descritor de arquivo no shell. Ela é usada para exibir informações no terminal.
	- **access**: é usada para verificar se um arquivo ou diretório é acessível no shell. Ela é usada para verificar permissões de acesso.
	- **open**: é usada para abrir um arquivo ou criar um novo arquivo no shell. Ela é usada para acessar arquivos no sistema de arquivos.
	- **read**: é usada para ler dados de um arquivo ou descritor de arquivo no shell. Ela é usada para obter informações de entrada.
	- **close**: é usada para fechar um arquivo ou descritor de arquivo no shell. Ela é usada para liberar recursos após a leitura ou gravação de dados.
	- **fork**: é usada para criar um novo processo no shell. Ela é usada para executar comandos em segundo plano.
	- **wait**: é usada para esperar que um processo filho termine no shell. Ela é usada para sincronizar a execução de comandos.
	- **waitpid**: é usada para esperar que um processo filho específico termine no shell. Ela é usada para controlar a execução de processos.
	- **wait3**: é usada para esperar que um processo filho termine e obter informações sobre seu status no shell. Ela é usada para obter informações detalhadas sobre a execução de processos.
	- **wait4**: é usada para esperar que um processo filho termine e obter informações sobre seu status no shell. Ela é usada para obter informações detalhadas sobre a execução de processos.
	- **signal**: é usada para configurar um manipulador de sinal no shell. Ela é usada para lidar com sinais como interrupções e términos.
	- **sigaction**: é usada para configurar um manipulador de sinal avançado no shell. Ela é usada para lidar com sinais de forma mais flexível.
	- **sigemptyset**: é usada para inicializar um conjunto de sinais vazio no shell. Ela é usada para configurar manipuladores de sinal.
	- **sigaddset**: é usada para adicionar um sinal a um conjunto de sinais no shell. Ela é usada para configurar manipuladores de sinal.
	- **kill**: é usada para enviar um sinal a um processo no shell. Ela é usada para controlar a execução de processos.
	- **exit**: é usada para encerrar o shell. Ela é usada para sair do shell e retornar ao prompt do sistema operacional.
	- **getcwd**: é usada para obter o diretório de trabalho atual do shell. Ela é usada para fornecer informações sobre o ambiente do shell.
	- **chdir**: é usada para alterar o diretório de trabalho atual do shell. Ela é usada para navegar entre diretórios no sistema de arquivos.
	- **stat**: é usada para obter informações sobre um arquivo no shell. Ela é usada para verificar permissões e propriedades de arquivos.
	- **lstat**: é usada para obter informações sobre um arquivo no shell. Ela é usada para verificar permissões e propriedades de arquivos.
	- **fstat**: é usada para obter informações sobre um arquivo aberto no shell. Ela é usada para verificar permissões e propriedades de arquivos.
	- **unlink**: é usada para excluir um arquivo no shell. Ela é usada para remover arquivos do sistema de arquivos.
	- **execve**: é usada para executar um programa em um novo processo no shell. Ela é usada para iniciar novos comandos.
	- **dup**: é usada para duplicar um descritor de arquivo no shell. Ela é usada para redirecionar a entrada e saída de comandos.
	- **dup2**: é usada para duplicar um descritor de arquivo em um novo descritor no shell. Ela é usada para redirecionar a entrada e saída de comandos.
	- **pipe**: é usada para criar um pipe no shell. Ela é usada para conectar a saída de um comando à entrada de outro comando.
	- **opendir**: é usada para abrir um diretório no shell. Ela é usada para acessar arquivos e subdiretórios.
	- **readdir**: é usada para ler o conteúdo de um diretório no shell. Ela é usada para listar arquivos e subdiretórios.
	- **closedir**: é usada para fechar um diretório no shell. Ela é usada para liberar recursos após a leitura de arquivos.
	- **strerror**: é usada para obter uma mensagem de erro associada a um código de erro no shell. Ela é usada para fornecer informações sobre erros.
	- **perror**: é usada para exibir uma mensagem de erro no terminal no shell. Ela é usada para fornecer informações sobre erros.
	- **isatty**: é usada para verificar se um descritor de arquivo está associado a um terminal no shell. Ela é usada para verificar a entrada e saída de comandos.
	- **ttyname**: é usada para obter o nome do terminal associado a um descritor de arquivo no shell. Ela é usada para obter informações sobre o terminal.
	- **ttyslot**: é usada para obter o número do slot do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **ioctl**: é usada para controlar dispositivos de E/S no shell. Ela é usada para configurar e interagir com dispositivos.
	- **getenv**: é usada para obter o valor de uma variável de ambiente no shell. Ela é usada para acessar informações do ambiente do shell.
	- **tcsetattr**: é usada para configurar os atributos do terminal no shell. Ela é usada para controlar a entrada e saída do terminal.
	- **tcgetattr**: é usada para obter os atributos do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **tgetent**: é usada para obter as capacidades do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **tgetflag**: é usada para obter uma bandeira de capacidade do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **tgetnum**: é usada para obter um número de capacidade do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **tgetstr**: é usada para obter uma sequência de capacidade do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **tgoto**: é usada para expandir uma sequência de capacidade do terminal no shell. Ela é usada para obter informações sobre o terminal.
	- **tputs**: é usada para enviar uma sequência de capacidade do terminal para o terminal. Ela é usada para controlar a exibição no terminal.