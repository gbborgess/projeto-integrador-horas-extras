# ⏱️ Projeto Integrador: Sistema Digital de Controle de Horas Extras

Este projeto é um protótipo funcional de um Sistema Digital de Controle de Horas Extras, desenvolvido em Linguagem C como parte do Projeto Integrador dos cursos de Engenharia da Computação e Ciência da Computação da UVV, em parceria com a startup Base27.

## 🎯 Objetivo

O objetivo é criar a lógica de um sistema capaz de automatizar o processo de registro, validação e aprovação de horas extras, substituindo processos manuais que são lentos e suscetíveis a erros.

## 💻 Justificativa da Linguagem (Linguagem C)

A escolha da Linguagem C para este protótipo é puramente **didática e estratégica**, visando consolidar os fundamentos de programação estruturada.

Conforme a especificação do projeto (documento anexo), a Linguagem C não é ideal para sistemas comerciais em produção, mas é excelente para:

* Desenvolver o núcleo lógico da aplicação.
* Compreender o funcionamento interno e o fluxo de dados.
* Demonstrar a viabilidade lógica da solução.

Este código representa, portanto, o **protótipo lógico** que servirá de base para futuras versões em outras plataformas (como web ou mobile).

## ⚙️ Funcionalidades Implementadas

O sistema possui três níveis de acesso (Funcionário, Gestor e Admin) e permite:

* **Funcionário:**
    * Fazer login.
    * Registrar horas extras com diferentes multiplicadores (horário/ocasião).
    * Visualizar o status de suas horas (Pendente, Aprovado, Rejeitado).
* **Gestor:**
    * Adicionar e remover funcionários.
    * Aprovar ou rejeitar horas extras pendentes.
    * Gerar relatórios de horas de seus funcionários.
* **Admin:**
    * Gerenciamento total de usuários (incluindo Gestores).
    * Validação de horas (função de superusuário).

## 🚀 Como Executar

1.  Clone o repositório.
2.  Compile o arquivo `main.c` (ex: `gcc main.c -o sistema_horas`).
3.  Execute o programa (ex: `./sistema_horas`).
4.  O login de administrador padrão é:
    * **Login:** `admin`
    * **Senha:** `Admin@123`

---

### 👨‍💻 Autores

* Andrews Muniz Ott - EO1M
* Gabriel de Paula Borges - EO1M
* Gustavo da Silva Ferreira - EO1M
