## 初めてのコントリビュート

-   [How to contribute to an open source project on GitHub](https://app.egghead.io/courses/how-to-contribute-to-an-open-source-project-on-github)
-   [CP Editor をソースからビルドする](https://cpeditor.org/docs/installation/build-from-source/)
-   [Qt docs](https://doc.qt.io/)
-   [issuesを見つける](https://github.com/cpeditor/cpeditor/contribute)

## コードを書く前に

もし、あなたが取り組もうとしていることについて未解決の問題がある場合、以下の理由から、プルリクエストのコードを書く前に、問題を開いてください:

1.  他の人の助けを借りることができる。
2.  自分が取り組んでいることを他の人に知ってもらえるので、重複作業の可能性を減らせる。
3.  この機能が認められない場合、あるいは"バグ"が実はバグでなかった場合などに通知されるので、時間の節約になる。

## コントリビューションルール

-   目標は、開発者のためではなく、競技プログラマーのためのエディタを作ることであることを忘れないでください。
-   1つのプルリクエストで1つのことを行う。もし、複数の機能を追加したい/複数のバグを修正したい場合で、それらが関連性のないものであれば、複数のプルリクエストを開いてください。複数のプルリクエストを開くために複数のブランチを作成する必要がある場合があります。gitのブランチ作成については[こちら](https://learngitbranching.js.org/)で学ぶことができます。
-   新機能の追加は master ブランチで行い、バグの修正はそのバグがある最新のアクティブな (master, beta, stable) ブランチで行います。[バージョニングルール](VERSIONING.md)も参照してください。
-   [明確なコミットメッセージ](https://chris.beams.io/posts/git-commit/)を書いてください。また、 [Conventional Commits](https://www.conventionalcommits.org/) を書くことをお勧めします。[プルリクエストのスカッシュとマージ](https://docs.github.com/en/free-pro-team@latest/github/collaborating-with-issues-and-pull-requests/about-pull-request-merges#squash-and-merge-your-pull-request-commits)を行っているので、コミットメッセージはあまり重要ではなく、プルリクエストのタイトルや唯一のコミットのメッセージはより重要であると言えます。
-   コードのフォーマットには Clang Format と [`.clang-format`](.clang-format) を使ってください。もし、フォーマット中に何かおかしなことが起こったら、 `clang-format` を[最新版](https://releases.llvm.org/download.html)に更新してみてください。
-   必要でない限り、機能を削除しないでください。代わりに、それらを無効にするオプションを追加してください。
-   設定ファイルは常に後方互換性を保つようにしましょう。
-   ハラスメントのないコミュニティを作るために、誰もが[行動規範](CODE_OF_CONDUCT.md)を遵守すべきです。

## ドキュメント

ドキュメントは [cpeditor.github.io](https://github.com/cpeditor/cpeditor.github.io) でホストされています。詳しくは[コントリビューションガイド](https://github.com/cpeditor/cpeditor.github.io/blob/hugo/CONTRIBUTING.md)をご覧ください。

## 翻訳

翻訳は、 [translations](translations) ディレクトリにあります。 Qt Linguist を使用して編集することをお勧めします。しかし、誤字脱字を直すだけなら、好きなテキストエディタを使えばいいのです。 Qt Linguist では、翻訳が終了したら、 <kbd>Ctrl + Enter</kbd> を押して、終了したものとしてマークし、次の未完成の翻訳に進みます。もしテキストが翻訳を必要としない場合、例えば "C++" のように、翻訳を空白にして終了マークを付けることができます。 Qt Linguist のすべての警告を修正する必要があります。

新しい言語を追加したい場合、開発者に助けを求めることができます。例えば、希望する言語について issue を開くことができます（私たちが準備を終えた後に、あなたが翻訳を提供してくれるとよりよいでしょう）。

さらにもう一歩踏み込むなら 翻訳された文字列は、ソースコード中で `tr()` でラップされます。コードが変更されたら、 [updateTranslation.sh](tools/updateTranslation.sh) または [updateTranslation.bat](tools/updateTranslation.bat) を実行して翻訳を更新することができます。興味のある方は [Qt Linguist Manual](https://doc.qt.io/qt-5/qtlinguist-index.html) を読んでみてください。

## 設定システム

設定システムの基本構成は:

1.  設定情報は、 [settings.json](src/Settings/settings.json) に格納されています。
2.  `settings.json` は、 [genSettings.py](src/Settings/genSettings.py) によって `SettingsHelper.hpp` と `SettingsInfo.cpp` に変換されます。
3.  設定は [PreferencesWindow](src/Settings/PreferencesWindow.hpp) に表示され、いくつかの [PreferencesPage](src/Settings/PreferencesPage.hpp) に分割されます。ほとんどの設定は、 `SettingsInfo` を使ってページを生成する [PreferencesPageTemplate](src/Settings/PreferencesPageTemplate.hpp) に表示されます。
4.  CP Editor の他の部分では、 `SettingsHelper.hpp` 内の関数を使って設定を取得したり、 [SettingsManager](src/Settings/SettingsManager.hpp) を使って設定を管理したりすることができます。 `SettingsHelper.hpp` の目的は、開発中にオートコンプリートを有効にし、設定名のスペルを間違える可能性を減らすことです。 `SettingsManager` は通常、設定名が変数である場合に使用します。例えば、 `SettingsManager::get(QString("%1/Compile Command").arg(language)).toString()` です。

設定の追加・変更の方法は、他の設定を参考にするか、[ドキュメント](src/Settings/README.md)を読むとよいでしょう。基本的には、 [PreferencesWindow.cpp](src/Settings/PreferencesWindow.cpp) の [settings.json](src/Settings/settings.json) と `AddPageHelper(this)` で始まる部分を変更する必要があります。

## ヒント

-   実装が短くて簡単でも、可能な限りコードを再利用する。
-   ユーザーにパスの選択を求めるときは [DefaultFileManager](src/Settings/DefaultPathManager.hpp) を使用します。(環境設定ウィンドウでのパスの選択など、いくつかの例外はありますが、その場合、パスはすでに設定に保存されています)。
-   テストするときは、標準エラーメッセージを確認してください。標準エラー出力には、 Qt からの警告やコード中の `qDebug()` が含まれており、一般的なミスを素早く発見することができます。
-   サブモジュールを更新するには、 `git submodule update --init --recursive` を使ってください。
