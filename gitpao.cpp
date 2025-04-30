#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

bool massaFeita = false;
bool arquivosAdicionados = false;
bool commitFeito = false;
string branchAtual = "master";  // Branch padrão

string capturarSaida(string comandoGit) {
    string resultado;
    FILE* pipe = _popen(comandoGit.c_str(), "r");
    if (!pipe) return "Erro ao executar comando.";

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        resultado += buffer;
    }
    _pclose(pipe);
    return resultado;
}

void exibirComTraducao(string saida, string tipo) {
    if (tipo == "push") cout << "📤 Tostadeira pronta! Pão enviado para a loja!\n";
    else if (tipo == "pull") cout << "📥 Mordida recebida! Atualizamos o pão da loja.\n";
    else if (tipo == "status") cout << "🧑‍🍳 Status da fornada:\n";
    else if (tipo == "log") cout << "📜 Receita dos commits:\n";
    else if (tipo == "branch") cout << "🌱 Pãezinhos disponíveis:\n";
    else if (tipo == "commit") cout << "🔥 O pão foi ao forno e assou direitinho!\n";
    else if (tipo == "init") cout << "🔧 Massa preparada! Começamos a padaria.\n";
    else if (tipo == "add") cout << "➕ Fatia separada para o forno!\n";
    else if (tipo == "remote") cout << "🏪 Padaria conectada com sucesso!\n";
    else if (tipo == "merge") cout << "🥣 Mistura feita! Massas combinadas.\n";

    if (!saida.empty()) cout << saida << endl;
}

void interpretar(string linha) {
    istringstream iss(linha);
    string cmd;
    iss >> cmd;

    if (cmd == "massa") {
        string out = capturarSaida("git init");
        exibirComTraducao(out, "init");
        massaFeita = true;
        arquivosAdicionados = false;
        commitFeito = false;
    }
    else if (cmd == "fatia") {
        if (!massaFeita) {
            cout << "⚠️ A massa ainda não foi preparada! Faça 'massa' primeiro.\n";
            return;
        }
        string arquivo;
        getline(iss, arquivo);
        if (arquivo.empty()) cout << "⚠️ Especifique o nome do arquivo.\n";
        else {
            string comando = "git add " + arquivo;
            string out = capturarSaida(comando);
            exibirComTraducao(out, "add");
            arquivosAdicionados = true;
        }
    }
    else if (cmd == "forno") {
        if (!massaFeita) {
            cout << "⚠️ Você precisa preparar a massa primeiro! ('massa')\n";
            return;
        }
        if (!arquivosAdicionados) {
            cout << "⚠️ Nenhuma fatia foi separada! Use 'fatia <arquivo>' antes.\n";
            return;
        }
        string mensagem;
        getline(iss, mensagem);
        if (mensagem.empty()) {
            cout << "⚠️ Escreva uma mensagem para o commit.\n";
        } else {
            string comando = "git commit -m \"" + mensagem.substr(1) + "\"";
            string out = capturarSaida(comando);
            exibirComTraducao(out, "commit");
            commitFeito = true;
        }
    }
    else if (cmd == "tostadeira") {
        if (!commitFeito) {
            cout << "⚠️ O pão ainda nem foi assado! Use 'forno' primeiro.\n";
            return;
        }

        string comando = "git push -u origin " + branchAtual;
        string out = capturarSaida(comando);
        if (out.find("fatal:") != string::npos) {
            cout << "⚠️ Tostadeira com problemas:\n" << out;
        } else {
            exibirComTraducao(out, "push");
        }
    }
    else if (cmd == "mordida") {
        string out = capturarSaida("git pull");
        exibirComTraducao(out, "pull");
    }
    else if (cmd == "padeiro") {
        string out = capturarSaida("git status");
        exibirComTraducao(out, "status");
    }
    else if (cmd == "receita") {
        string out = capturarSaida("git log --oneline");
        exibirComTraducao(out, "log");
    }
    else if (cmd == "paozinho") {
        string out = capturarSaida("git branch");
        exibirComTraducao(out, "branch");
    }
    else if (cmd == "padaria") {
        string url;
        getline(iss, url);
        if (url.empty()) cout << "⚠️ Digite a URL da padaria (remote).\n";
        else {
            string comando = "git remote add origin " + url.substr(1);
            string out = capturarSaida(comando);
            exibirComTraducao(out, "remote");
        }
    }
    else if (cmd == "mistura") {
        string branch;
        getline(iss, branch);
        if (branch.empty()) cout << "⚠️ Especifique o ramo para misturar.\n";
        else {
            string comando = "git merge " + branch.substr(1);
            string out = capturarSaida(comando);
            exibirComTraducao(out, "merge");
        }
    }
    else if (cmd == "criar") {
        string nomeBranch;
        getline(iss, nomeBranch);
        if (nomeBranch.empty()) {
            cout << "⚠️ Especifique o nome da nova branch.\n";
            return;
        }
        string comando = "git checkout -b " + nomeBranch.substr(1);
        string out = capturarSaida(comando);
        branchAtual = nomeBranch.substr(1);
        exibirComTraducao(out, "branch");
    }
    else if (cmd == "mudar") {
        string nomeBranch;
        getline(iss, nomeBranch);
        if (nomeBranch.empty()) {
            cout << "⚠️ Especifique o nome da branch para mudar.\n";
            return;
        }
        string comando = "git checkout " + nomeBranch.substr(1);
        branchAtual = nomeBranch.substr(1);
        string out = capturarSaida(comando);
        exibirComTraducao(out, "branch");
    }
    else if (cmd == "renomear") {
        string novoNome;
        getline(iss, novoNome);
        if (novoNome.empty()) {
            cout << "⚠️ Especifique o novo nome da branch.\n";
            return;
        }
        string comando = "git branch -M " + novoNome.substr(1);
        string out = capturarSaida(comando);
        branchAtual = novoNome.substr(1);
        cout << "📛 Branch renomeada para '" << branchAtual << "' com sucesso!\n";
    }
    else if (cmd == "sair") {
        cout << "👋 Padaria fechando! Até a próxima fornada.\n";
        exit(0);
    }
    else {
        cout << "❌ Comando desconhecido: " << cmd << endl;
    }
}

int main() {
    string linha;
    cout << "🥖 Bem-vindo ao GitPão v3.5 — CLI da padaria Git 🍞\n";
    cout << "⚙️  Comandos temáticos: massa → fatia → forno → renomear → padaria → tostadeira\n";
    cout << "(digite 'sair' para sair)\n" << endl;

    while (true) {
        cout << "> ";
        getline(cin, linha);
        interpretar(linha);
    }

    return 0;
}
