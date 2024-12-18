
## Sua primeira contribuição

-   [Como contribuir para um projeto de código aberto no GitHub](https://app.egghead.io/courses/how-to-contribute-to-an-open-source-project-on-github)
-   [Compile o CP Editor a partir do código-fonte](https://cpeditor.org/docs/installation/build-from-source/)
-   [Documentação do Qt](https://doc.qt.io/)
-   [Descubra problemas](https://github.com/cpeditor/cpeditor/contribute)

## Antes de escrever códigos

Se não houver um problema aberto sobre o que você pretende trabalhar, por favor, abra um problema antes de escrever códigos para um *pull request*, pelos seguintes motivos:

1.  Você pode receber ajuda de outras pessoas.
2.  Outros saberão que você está trabalhando nisso, evitando possíveis trabalhos duplicados.
3.  Você será notificado se a funcionalidade não for aceita ou se o "erro" na verdade não for um erro, economizando assim o seu tempo.

## Regras de contribuição

-   Lembre-se de que o objetivo é construir um editor para programadores competitivos, não para desenvolvedores.
-   Faça apenas uma coisa em um *pull request*. Se você quiser adicionar várias funcionalidades/corrigir vários erros que não são relacionados, abra múltiplos *pull requests*. Talvez você precise criar múltiplos *branches* para abrir esses *pull requests*. Você pode aprender sobre *branching* no Git [aqui](https://learngitbranching.js.org/).
-   Adicione novas funcionalidades no *branch* principal (*master*) e corrija bugs no *branch* ativo mais recente (*master*, beta e estável) em que o bug estiver presente. Veja também a [regra de versionamento](VERSIONING.md).
-   Por favor, escreva [mensagens de *commit* claras](https://chris.beams.io/posts/git-commit/). É recomendado também seguir o padrão de [Commits Convencionais](https://www.conventionalcommits.org/). Como estamos [agregando e mesclando *pull requests*](https://docs.github.com/en/free-pro-team@latest/github/collaborating-with-issues-and-pull-requests/about-pull-request-merges#squash-and-merge-your-pull-request-commits), as mensagens individuais do *commit* em um *pull request* não são tão importantes, mas o título do *pull request* ou a mensagem do único *commit* no *pull request* são cruciais.
-   Use Clang Format e o arquivo [`.clang-format`](.clang-format) para formatar seu código. Se ocorrer algum erro durante a formatação, tente atualizar seu `clang-format` para a [versão mais recente](https://releases.llvm.org/download.html).
-   Não remova funcionalidades, a menos que seja necessário. Adicione uma opção para desativá-las em vez disso.
-   Mantenha o arquivo de configurações sempre compatível com versões anteriores.
-   Todos devem seguir o [Código de Conduta](CODE_OF_CONDUCT.md) para construirmos uma comunidade livre de assédio.

## Documentação

A documentação está hospedada em [cpeditor.github.io](https://github.com/cpeditor/cpeditor.github.io). Você pode ler as [diretrizes de contribuição](https://github.com/cpeditor/cpeditor.github.io/blob/hugo/CONTRIBUTING.md) para mais informações.

## Traduções

As traduções estão no diretório [translations](translations). É recomendado usar o Qt Linguist para editá-las. No entanto, se você apenas deseja corrigir um erro de digitação, pode usar qualquer editor de texto que preferir. No Qt Linguist, quando uma tradução estiver concluída, pressione <kbd>Ctrl + Enter</kbd> para marcá-la como finalizada e ir para a próxima tradução inacabada. Se um texto não precisar de tradução, como "C++", você pode deixar a tradução vazia e marcá-la como finalizada. Certifique-se de corrigir todos os avisos no Qt Linguist.

Se quiser adicionar um novo idioma, peça ajuda aos desenvolvedores. Por exemplo, você pode abrir um problema solicitando o idioma desejado (será ainda melhor se você puder contribuir com as traduções após prepararmos tudo).

Se quiser ir mais longe: As strings traduzidas estão envoltas em `tr()` no código-fonte. Quando o código é alterado, você pode executar [updateTranslation.sh](tools/updateTranslation.sh) ou [updateTranslation.bat](tools/updateTranslation.bat) para atualizar as traduções. Leia o [Manual do Qt Linguist](https://doc.qt.io/qt-5/qtlinguist-index.html) se estiver interessado.

## O sistema de configurações

A estrutura básica do sistema de configurações é:

1.  As informações das configurações estão armazenadas em [settings.json](src/Settings/settings.json).
2.  `settings.json` é traduzido para `SettingsHelper.hpp` e `SettingsInfo.cpp` por [genSettings.py](src/Settings/genSettings.py).
3.  As configurações são exibidas na [PreferencesWindow](src/Settings/PreferencesWindow.hpp) e separadas em várias [PreferencesPage](src/Settings/PreferencesPage.hpp). A maioria das configurações é exibida em [PreferencesPageTemplate](src/Settings/PreferencesPageTemplate.hpp), que usa `SettingsInfo` para gerar a página.
4.  Em outras partes do CP Editor, você pode acessar as configurações por meio das funções em `SettingsHelper.hpp` ou usar o [SettingsManager](src/Settings/SettingsManager.hpp) para gerenciar as configurações. O objetivo de `SettingsHelper.hpp` é habilitar o auto-complete durante o desenvolvimento e reduzir a chance de erros de digitação no nome das configurações. O `SettingsManager` é geralmente usado em casos onde o nome da configuração é uma variável, por exemplo: `SettingsManager::get(QString("%1/Compile Command").arg(language)).toString()`.

Para aprender a adicionar/modificar configurações, consulte outras configurações ou leia a [documentação](src/Settings/README.md). Basicamente, você precisa modificar [settings.json](src/Settings/settings.json) e a parte que começa com `AddPageHelper(this)` em [PreferencesWindow.cpp](src/Settings/PreferencesWindow.cpp).

## Dicas

-   Reutilize códigos sempre que possível, mesmo que a implementação seja curta e simples.
-   Use [DefaultFileManager](src/Settings/DefaultPathManager.hpp) ao solicitar que o usuário escolha um caminho. (Com algumas exceções, incluindo escolher um caminho na janela de preferências, onde o caminho já é salvo nas configurações.)
-   Verifique o stderr ao testar. O stderr contém avisos do Qt e `qDebug()` no código, que são uma maneira rápida de detectar erros comuns.
-   Use `git submodule update --init --recursive` para atualizar submódulos.
