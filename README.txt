/* Universidade de Brasilia
*  Departamento de Ciencia da Computacao
*  Sistemas Operacionais - 1/2016
*  Trabalho 1 - Gerenciador de Memoria
*  Diego Marques de Azevedo
*  Samuel Vinicius Vieira Pala
*/

gcc --version: gcc (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4
SO: Linux Mint 17.2 Rafaela

ALOCADOR:

	FILA DE MENSAGEM: para as requisicoes e e para pids dos  processos.	
	MEMORIA COMPARTILHADA: para tabela de paginas e quantidade de frames.

SUBSTITUIDOR:
	
	SIGNAL: para tratar requisicao de fim do processo de substituicao.
	FILA DE MENSAGEM: para fila dos pids dos processos.
	MEMORIA COMPARTILHADA: obtem tabela de paginas e quantidade de frames.

PROCESSO USUARIO:
	
	SIGNAL: para tratar a remocao de fila de requisicoes e confirmacao de alocacao.
	FILA DE MENSAGEM: para pids dos processos, requisicao e confirmacao do alocador.

SHUTDOWN:
	MEMORIA COMPARTILHADA: para tratar (remover) a tabela de paginas.
	FILA DE MENSAGEM: para os pids dos processos e processo de remocao.
	
	
Utilização e Ordem de Lançamento:

1 - alocador
2 - substituidor
3 - processo_usr
4 - shutdown

Para a execução do processo_usr e necessario informar um arquivo de entrada, por linha de comando, contendo os numero das paginas que devem ser alocadas. Os numeros nas paginas devem ser separados por um espaço em branco.
Ex: ./processo_usr p1.txt

OBS: Para facilitar, existe um makefile que compila todos os arquivos de uma vez. Para utilizar, basta entrar na pasta onde esta localizado e digitar $make.
