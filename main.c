#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes para o máximo de usuários e de registros
#define MAX_USUARIOS 50
#define MAX_REGISTROS 1000

void limpa() { // Função para limpar o console
  system("clear || cls");
}

void limpar_buffer_entrada() { // Função para limpar o '\n' do buffer
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void pause() { // Função para pausar e esperar o Enter do usuário para
              // prosseguir
  printf("\nPressione Enter para continuar...");
  limpar_buffer_entrada();
}

void lerString(char *dest, int tam) {
  // Função para fazer a leitura de strings e
  // guardar corretamente no vetor
  fgets(dest, tam, stdin);
  dest[strcspn(dest, "\n")] = 0;
}

// --------------
// Tipos
// --------------
typedef enum { FUNCIONARIO, GESTOR, ADMIN } TipoUsuario;

// Estrutura do usuário, seja ele funcionário, gestor ou admin
typedef struct {
  int id;
  char login[50];
  char senha[50];
  TipoUsuario tipo;
  float salarioHora;
} Usuario;

typedef struct {
  char usuario[50];
  int horas;
  float multiplicador;
  float valorFinal;
  int aprovado; // 0 = pendente, 1 = aprovado, -1 = rejeitado
} RegistroHoras;

// ------------------
// Banco em Memoria
// ------------------
Usuario usuarios[MAX_USUARIOS];
int numUsuarios = 0;

RegistroHoras registros[MAX_REGISTROS];
int numRegistros = 0;

Usuario usuarioLogado;
int logado = 0;

int proximoID = 1;

// -----------------
// Persistencia em arquivos DB
// -----------------
void salvarUsuarios() {
  FILE *f = fopen("usuarios.db", "wb");
  if (!f) {
    printf("Erro ao salvar usuários. \n");
    return;
  }
  fwrite(&numUsuarios, sizeof(int), 1, f);
  fwrite(usuarios, sizeof(Usuario), numUsuarios, f);
  fclose(f);
}

void carregarUsuarios() {
  FILE *f = fopen("usuarios.db", "rb");
  if (!f) {
    return;
  }
  fread(&numUsuarios, sizeof(int), 1, f);
  if (numUsuarios > 0 && numUsuarios <= MAX_USUARIOS) {
    fread(usuarios, sizeof(Usuario), numUsuarios, f);

    proximoID = 1;
    for (int i = 0; i < numUsuarios; i++) {
      if (usuarios[i].id >= proximoID) {
        proximoID = usuarios[i].id + 1;
      }
    }
  } else {
    numUsuarios = 0;
  }
  fclose(f);
}

void salvarRegistros() {
  FILE *f = fopen("registros.db", "wb");
  if (!f) {
    printf("Erro ao salvar registros. \n");
    return;
  }
  fwrite(&numRegistros, sizeof(int), 1, f);
  fwrite(registros, sizeof(RegistroHoras), numRegistros, f);
  fclose(f);
}

void carregarRegistros() {
  FILE *f = fopen("registros.db", "rb");
  if (!f) {
    return;
  }
  fread(&numRegistros, sizeof(int), 1, f);
  if (numRegistros > 0 && numRegistros <= MAX_REGISTROS) {
    fread(registros, sizeof(RegistroHoras), numRegistros, f);
  } else {
    numRegistros = 0;
  }
  fclose(f);
}

// ------------
// UTILS
// ------------
const char *tipoToString(TipoUsuario t) {
  switch (t) {
  case FUNCIONARIO:
    return "FUNCIONARIO";
  case GESTOR:
    return "GESTOR";
  case ADMIN:
    return "ADMIN";
  default:
    return "DESCONHECIDO";
  }
}

// ------------
// FUNÇÕES USUARIOS
// ------------
void adicionarFuncionarioSimples() {
  if (numUsuarios >= MAX_USUARIOS) {
    printf("Limite de usuários atingido. \n");
    return;
  }
  Usuario u;

  printf("Login do novo funcionário: ");
  lerString(u.login, 50);

  printf("Senha: ");
  lerString(u.senha, 50);

  printf("Salário por hora (R$): ");
  scanf("%f", &u.salarioHora);
  limpar_buffer_entrada();

  u.tipo = FUNCIONARIO;

  u.id = proximoID++;
  usuarios[numUsuarios++] = u;
  salvarUsuarios();

  printf("Funcionário '%s' adicionado com sucesso. \n", u.login);
}

void adicionarUsuario(TipoUsuario tipo) {
  if (numUsuarios >= MAX_USUARIOS) {
    printf("Limite de usuários atingido. \n");
    return;
  }
  Usuario u;

  printf("Login do novo usuário: ");
  lerString(u.login, 50);

  printf("Senha: ");
  lerString(u.senha, 50);

  u.tipo = tipo;

  u.id = proximoID++;
  usuarios[numUsuarios++] = u;
  salvarUsuarios();

  printf("Usuário '%s' adicionado com sucesso como '%s'. \n", u.login,
         tipoToString(tipo));
}

void listarFuncionarios() {
  printf("\nFuncionários Cadastrados:\n");
  printf("=============================\n");
  int encontrou = 0;
  for (int i = 0; i < numUsuarios; i++) {
    if (usuarios[i].tipo == FUNCIONARIO) {
      printf("ID: %d | Login: '%s'\n", usuarios[i].id, usuarios[i].login);
      encontrou = 1;
    }
  }
  if (!encontrou) {
    printf("Nenhum funcionário cadastrado.\n");
  }
  printf("=============================\n");
}

void listarTodosUsuarios() {
  printf("\nUsuários Cadastrados:\n");
  printf("===========================================\n");
  for (int i = 0; i < numUsuarios; i++) {
    printf("ID: %d | Login: %-15s | Tipo: %s\n", usuarios[i].id,
           usuarios[i].login, tipoToString(usuarios[i].tipo));
  }
  printf("===========================================\n");
}

void removerFuncionario() {
  int id;

  listarFuncionarios();

  printf("ID de funcionário a remover: ");
  scanf("%d", &id);
  limpar_buffer_entrada();

  int encontrado = 0;
  for (int i = 0; i < numUsuarios; i++) {
    if (usuarios[i].id == id && usuarios[i].tipo == FUNCIONARIO) {

      for (int j = i; j < numUsuarios - 1; j++) {
        usuarios[j] = usuarios[j + 1];
      }

      numUsuarios--;
      salvarUsuarios();
      printf("Funcionário removido com sucesso.\n");
      encontrado = 1;
      break;
    }
  }

  if (!encontrado) {
    printf("Funcionário não encontrado.\n");
  }
}

void removerUsuarioAdmin() {
  int id;

  listarTodosUsuarios();

  printf("ID do usuário a remover: ");
  scanf("%d", &id);
  limpar_buffer_entrada();

  if (id == usuarioLogado.id) {
    printf("Você não pode remover o próprio usuario logado.\n");
    return;
  }

  int encontrado = 0;
  for (int i = 0; i < numUsuarios; i++) {
    if (usuarios[i].id == id) {
      for (int j = i; j < numUsuarios - 1; j++) {
        usuarios[j] = usuarios[j + 1];
      }

      numUsuarios--;
      salvarUsuarios();
      printf("Usuário removido com sucesso. \n");
      encontrado = 1;
      break;
    }
  }

  if (!encontrado) {
    printf("Usuário não encontrado.\n");
  }
}

// ------------
// Registro de Horas
// ------------

void registrarHoras() {
  if (numRegistros >= MAX_REGISTROS) {
    printf("Limite de registros atingidos.\n");
    return;
  }
  int horas;
  int horario_1, ocasiao_1, resultado;
  float multiplicador = 1.0;

  Usuario *func = NULL;

  for (int i = 0; i < numUsuarios; i++) {
    if (strcmp(usuarioLogado.login, usuarios[i].login) == 0) {
      func = &usuarios[i];
      break;
    }
  }

  if (!func || func->salarioHora <= 0) {
    printf("Erro: funcionario sem salário definido. \n");
    return;
  }

  printf("\n======== Registro de Horas ========\n");

  printf(
      "Em qual horario e qual ocasião o funcionario executou essas horas?\n");
  printf("  ====================================\n");
  printf("|| Horários:                   ||\n");
  printf("|| (1) Entre 5h e 22h          ||\n");
  printf("|| (2) Entre 22h e 5h          ||\n");
  printf("|| Ocasião:                    ||\n");
  printf("|| (3) Dia de semana/Sábado    ||\n");
  printf("|| (4) Domingo/Feriado         ||\n");
  printf("  ====================================\n");

  printf("Horário (1 ou 2): ");
  scanf("%d", &horario_1);
  limpar_buffer_entrada();

  printf("Ocasião (3 ou 4): ");
  scanf("%d", &ocasiao_1);
  limpar_buffer_entrada();

  if (!(horario_1 >= 1 && horario_1 <= 2 && ocasiao_1 >= 3 && ocasiao_1 <= 4)) {
    printf("Opções inválidas. Registro cancelado.\n");
    return;
  }

  resultado = horario_1 + (ocasiao_1 * 10);

  if (resultado == 31) {
    multiplicador = 1.5;
  } else if (resultado == 41) {
    multiplicador = 2.0;
  } else if (resultado == 32) {
    multiplicador = 1.7;
  } else if (resultado == 42) {
    multiplicador = 2.2;
  }

  printf("Quantas horas deseja registrar: ");
  scanf("%d", &horas);
  limpar_buffer_entrada();

  if (horas <= 0) {
    printf("Número de horas inválido.\n");
    return;
  }

  float valorFinal = horas * multiplicador * func->salarioHora;

  RegistroHoras r;
  strcpy(r.usuario, usuarioLogado.login);
  r.horas = horas;
  r.multiplicador = multiplicador;
  r.valorFinal = valorFinal;
  r.aprovado = 0;

  registros[numRegistros++] = r;
  salvarRegistros();

  printf("Horas registradas com sucesso. Aguardando aprovação.\n");
}

void visualizarHoras() {
  printf("\nSeus Registros de Horas:\n");
  printf("========================================================\n");

  int encontrou = 0;
  for (int i = 0; i < numRegistros; i++) {
    if (strcmp(usuarioLogado.login, registros[i].usuario) == 0) {
      printf("Horas: %d | Multiplicador: %.2fx | Valor: R$ %.2f | Status: ",
             registros[i].horas, registros[i].multiplicador,
             registros[i].valorFinal);

      if (registros[i].aprovado == 0) {
        printf("Pendente\n");
      } else if (registros[i].aprovado == 1) {
        printf("Aprovado\n");
      } else {
        printf("Rejeitado\n");
      }
      encontrou = 1;
    }
  }

  if (!encontrou) {
    printf("Nenhum registro encontrado.\n");
  }
  printf("========================================================\n");
}

void validarRegistros() {
  int encontrou = 0;

  printf("\nRegistros Pendentes:\n");
  printf("===================================================================\n");
  printf("Ind | Usuário            | Horas | Multiplicador | Valor Final\n");
  printf("-------------------------------------------------------------------\n");

  for (int i = 0; i < numRegistros; i++) {
    if (registros[i].aprovado == 0) {
      printf("[%d] | %-18s | %-5d | %-13.2f | R$ %.2f\n", i,
             registros[i].usuario, registros[i].horas,
             registros[i].multiplicador, registros[i].valorFinal);
      encontrou = 1;
    }
  }
  printf("===================================================================\n");

  if (!encontrou) {
    printf("Não há registros pendentes. \n");
    return;
  }

  int idx;
  printf("Digite o indice do registro para validar (-1 para sair): ");
  scanf("%d", &idx);
  limpar_buffer_entrada();

  if (idx == -1) {
    printf("Saindo da validação.\n");
    return;
  }

  if (idx < 0 || idx >= numRegistros) {
    printf("Índice inválido.\n");
    return;
  }

  if (registros[idx].aprovado != 0) {
    printf("Este registro não está pendente.\n");
    return;
  }

  int op;
  printf("1 = Aprovar | 2 = Rejeitar: ");
  scanf("%d", &op);
  limpar_buffer_entrada();

  if (op == 1)
    registros[idx].aprovado = 1;
  else if (op == 2)
    registros[idx].aprovado = -1;
  else {
    printf("Opção inválida.\n");
    return;
  }

  salvarRegistros();

  printf("Registro atualizado. \n");
}

// ------------
// Login // Logout
// ------------

int fazerLogin() { // Fazer login (se logado, retorna 1, caso contrário, retorna
                   // 0)
  char login[50], senha[50];

  printf("Login: ");
  lerString(login, 50);

  printf("Senha: ");
  lerString(senha, 50);

  for (int i = 0; i < numUsuarios; i++) {
    if (strcmp(usuarios[i].login, login) == 0 &&
        strcmp(usuarios[i].senha, senha) == 0) {
      usuarioLogado = usuarios[i];
      logado = 1;
      return 1;
    }
  }
  return 0;
}

void fazerLogout() { // Finalizar o programa
  logado = 0;
  usuarioLogado.id = 0;
  usuarioLogado.login[0] = '\0';
  usuarioLogado.senha[0] = '\0';
  usuarioLogado.tipo = FUNCIONARIO;
  usuarioLogado.salarioHora = 0;

  printf("Logout efetuado...\n");
  pause();
}

// ------------
// Relatórios
// ------------

void imprimirRelatorio() {
  int escolhaID = -1;

  if (usuarioLogado.tipo == FUNCIONARIO) {
    escolhaID = usuarioLogado.id;
  } else {
    listarFuncionarios();
    printf("\nDigite o ID do funcionário para exibir o relátorio: ");
    scanf("%d", &escolhaID);
    limpar_buffer_entrada();
  }

  char loginAlvo[50];
  int encontradoUser = 0;

  for (int i = 0; i < numUsuarios; i++) {
    if (usuarios[i].id == escolhaID && usuarios[i].tipo == FUNCIONARIO) {
      strcpy(loginAlvo, usuarios[i].login);
      encontradoUser = 1;
      break;
    }
  }

  if (!encontradoUser) {
    printf("Funcionário com ID %d não encontrado. \n", escolhaID);
    return;
  }

  limpa();
  printf("\n============= RELATÓRIO DO FUNCIONÁRIO =============\n");
  printf("Funcionário: %s (ID: %d)\n", loginAlvo, escolhaID);
  printf("======================================================\n");

  int encontrouRegistro = 0;
  float totalAprovado = 0;
  float totalPendente = 0;
  float totalRejeitado = 0;

  for (int i = 0; i < numRegistros; i++) {
    if (strcmp(registros[i].usuario, loginAlvo) == 0) {
      encontrouRegistro = 1;

      printf("Horas: %-5d | Mult: %.2fx | Valor: R$ %-10.2f | Status: ",
             registros[i].horas, registros[i].multiplicador,
             registros[i].valorFinal);

      if (registros[i].aprovado == 0) {
        printf("Pendente\n");
        totalPendente += registros[i].valorFinal;
      } else if (registros[i].aprovado == 1) {
        printf("Aprovado\n");
        totalAprovado += registros[i].valorFinal;
      } else {
        printf("Rejeitado\n");
        totalRejeitado += registros[i].valorFinal;
      }
    }
  }

  if (!encontrouRegistro) {
    printf("\nNenhum registro foi encontrado para esse usuário.\n");
  }

  printf("------------------------------------------------------\n");

  if (usuarioLogado.tipo != FUNCIONARIO) {
    printf("TOTAL APROVADO:   R$ %.2f\n", totalAprovado);
    printf("TOTAL PENDENTE:   R$ %.2f\n", totalPendente);
    printf("TOTAL REJEITADO:  R$ %.2f\n", totalRejeitado);
  } else {
    printf("TOTAL APROVADO:   R$ %.2f\n", totalAprovado);
    printf("TOTAL PENDENTE:   R$ %.2f\n", totalPendente);
  }

  printf("======================================================\n");
  pause();
}

// ------------
// Menus
// ------------

void menuFuncionario() { // Interface do portal do usuário
  int op;

  while (1) {
    limpa();
    printf("\n========================================\n");
    printf("||       Portal do Funcionário        ||\n");
    printf("========================================\n");
    printf("|| (1) Adicionar Horas                ||\n");
    printf("|| (2) Visualizar Horas               ||\n");
    printf("|| (3) Imprimir Relatório             ||\n");
    printf("|| (4) Sair                           ||\n");
    printf("========================================\n");

    printf("Selecione uma opção: ");
    scanf("%d", &op);
    limpar_buffer_entrada();

    switch (op) {
    case 1:
      limpa();
      registrarHoras();
      pause();
      break;
    case 2:
      limpa();
      visualizarHoras();
      pause();
      break;
    case 3:
      limpa();
      imprimirRelatorio();
      break;
    case 4:
      fazerLogout();
      return;
    default:
      printf("Opção inválida.\n");
      pause();
    }
  }
}

void menuGestor() { // Interface do portal do gestor
  int op;

  while (1) {
    limpa();
    printf("\n========================================\n");
    printf("||           Portal do Gestor         ||\n");
    printf("========================================\n");
    printf("|| (1) Adicionar funcionário          ||\n");
    printf("|| (2) Remover funcionário            ||\n");
    printf("|| (3) Listar funcionários            ||\n");
    printf("|| (4) Imprimir relatório             ||\n");
    printf("|| (5) Aprovar/Rejeitar Horas         ||\n");
    printf("|| (6) Sair                           ||\n");
    printf("========================================\n");

    printf("Selecione uma opção: ");
    scanf("%d", &op);
    limpar_buffer_entrada();

    switch (op) {
    case 1:
      limpa();
      adicionarFuncionarioSimples();
      pause();
      break;
    case 2:
      limpa();
      removerFuncionario();
      pause();
      break;
    case 3:
      limpa();
      listarFuncionarios();
      pause();
      break;
    case 4:
      limpa();
      imprimirRelatorio();
      break;
    case 5:
      limpa();
      validarRegistros();
      pause();
      break;
    case 6:
      fazerLogout();
      return;
    default:
      printf("Opção inválida.\n");
      pause();
    }
  }
}

void menuAdmin() { // Interface do portal do admin
  int op;

  while (1) {
    limpa();
    printf("\n========================================\n");
    printf("||      Portal do Administrador       ||\n");
    printf("========================================\n");
    printf("|| (1) Adicionar funcionário          ||\n");
    printf("|| (2) Adicionar gestor               ||\n");
    printf("|| (3) Listar usuários                ||\n");
    printf("|| (4) Imprimir relatório             ||\n");
    printf("|| (5) Remover usuários               ||\n");
    printf("|| (6) Aprovar/Rejeitar Horas         ||\n");
    printf("|| (7) Sair                           ||\n");
    printf("========================================\n");

    printf("Selecione uma opção: ");
    scanf("%d", &op);
    limpar_buffer_entrada();

    switch (op) {
    case 1:
      limpa();
      adicionarFuncionarioSimples();
      pause();
      break;
    case 2:
      limpa();
      adicionarUsuario(GESTOR);
      pause();
      break;
    case 3:
      limpa();
      listarTodosUsuarios();
      pause();
      break;
    case 4:
      limpa();
      imprimirRelatorio();
      break;
    case 5:
      limpa();
      removerUsuarioAdmin();
      pause();
      break;
    case 6:
      limpa();
      validarRegistros();
      pause();
      break;
    case 7:
      fazerLogout();
      return;
    default:
      printf("Opção inválida.\n");
      pause();
    }
  }
}

int main() {
  carregarUsuarios();
  carregarRegistros();

  if (numUsuarios == 0) {
    strcpy(usuarios[0].login, "admin");
    strcpy(usuarios[0].senha, "Admin@123");
    usuarios[0].tipo = ADMIN;
    usuarios[0].id = proximoID++;
    usuarios[0].salarioHora = 0;
    numUsuarios = 1;
    salvarUsuarios();
  }

  while (1) {
    limpa();
    printf("\n==================================================\n");
    printf("||   Sistema Digital de Controle de Horas Extras  ||\n");
    printf("==================================================\n");

    if (!fazerLogin()) {
      printf("Credenciais inválidas.\n");
      pause();
      continue;
    }

    if (usuarioLogado.tipo == FUNCIONARIO) {
      menuFuncionario();
    } else if (usuarioLogado.tipo == GESTOR) {
      menuGestor();
    } else if (usuarioLogado.tipo == ADMIN) {
      menuAdmin();
    }
  }

  return 0;
}