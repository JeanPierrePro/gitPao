@echo off
title Instalador do GitPão 🥖
color 0E

echo.
echo 🥖 Iniciando a instalação do GitPão...
echo.

:: Criar pasta C:\GitPao
if not exist "C:\GitPao" (
    mkdir "C:\GitPao"
    echo 📁 Padaria criada em C:\GitPao
) else (
    echo 📁 Padaria já existe em C:\GitPao
)

:: Copiar gitpao.exe
copy /Y gitpao.exe "C:\GitPao\"
echo ✅ Pão (gitpao.exe) colocado na padaria!

:: Adicionar C:\GitPao ao PATH do sistema
setx /M PATH "%PATH%;C:\GitPao"
echo 🔧 Caminho da padaria adicionado ao PATH do sistema.

echo.
echo 🧁 GitPão instalado com sucesso!
echo.
pause
