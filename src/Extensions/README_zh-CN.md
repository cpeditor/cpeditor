# Extensions

此文件夹包含编辑器的可选扩展代码。缺少这部分代码不影响编辑器基本的功能。

**此文件夹中的所有代码以及选项均应处于`Extensions/$featurename`内。**

## 扩展列表

| 扩展 | 类 | 描述 |
|----------|------|-------------|
| CF Tool | `CFTool` | 通过 [cf-tool](https://github.com/xalanq/cf-tool) 提交代码至 Codeforces。设置项：`CF/Enable`、`CF/Path`、`CF/Show Toast Messages` |
| CSES CLI | `CSESTool` | 通过 `cses-cli` 提交代码至 [CSES](https://cses.fi)。设置项：`CSES CLI/Enable`、`CSES CLI/Path`、`CSES CLI/Show Toast Messages` |
| Clang Format | `ClangFormatter` | 使用 clang-format 格式化 C/C++ 代码 |
| YAPF | `YAPFormatter` | 使用 YAPF 格式化 Python 代码 |
| Competitive Companion | `CompanionServer` | 从 [Competitive Companion](https://github.com/jmerle/competitive-companion) 浏览器扩展接收题目数据（测试用例、URL） |
| Language Server | `LanguageServer` | 基于 LSP 的 C++、Java 和 Python 代码诊断 |
| WakaTime | `WakaTime` | 通过 [WakaTime](https://wakatime.com) 追踪编码时间 |

### CSES CLI 集成详情

CSES CLI 集成 (`CSESTool`) 采用与 CF Tool 相同的模式：

1. **启用/禁用**：在 首选项 → 扩展 → CSES CLI 中通过 `CSES CLI/Enable` 切换
2. **路径**：在 `CSES CLI/Path` 中设置 `cses-cli` 可执行文件的路径（默认：PATH 中的 `cses-cli`）
3. **URL 解析**：当设置 `cses.fi` 题目 URL 时（手动或通过 Competitive Companion），自动解析比赛和题目 ID。支持两种 URL 模式：
   - `cses.fi/{contest}/task/{taskId}`（标准题目和比赛题目）
   - `cses.fi/{contest}/{subcategory}/{taskId}`（多级分类结构）
4. **提交**：编译/运行工具栏中出现"Submit to CSES"按钮。点击后将当前代码保存至临时文件，并通过 `cses-cli submit -c {contest} -t {taskId} {file}` 提交
5. **回退解析**：如果 URL 解析失败，可从题目标题注释（如 `Contest: 647`、`Problem: A`）或注释中的 `cses.fi` URL 提取 CSES 标识符
6. **Toast 消息**：提交结果以 toast 通知形式显示（复用 CF Tool 的 toast 设置）