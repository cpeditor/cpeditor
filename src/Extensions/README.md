# Extensions

This directory contains headers/sources that add additional features to 
the editor. Even if some feature in this folder does not work, 
the user can still use the editor to compile and test the programs.

**All code in this folder must be under namespace `Extensions` and its 
settings (if any) should be under `Extensions/$featurename`**

## Extensions

| Extension | Class | Description |
|-----------|-------|-------------|
| CF Tool | `CFTool` | Submit solutions to Codeforces via [cf-tool](https://github.com/xalanq/cf-tool). Settings: `CF/Enable`, `CF/Path`, `CF/Show Toast Messages` |
| CSES CLI | `CSESTool` | Submit solutions to [CSES](https://cses.fi) via `cses-cli`. Settings: `CSES CLI/Enable`, `CSES CLI/Path`, `CSES CLI/Show Toast Messages` |
| Clang Format | `ClangFormatter` | Format C/C++ code using clang-format |
| YAPF | `YAPFormatter` | Format Python code using YAPF |
| Competitive Companion | `CompanionServer` | Receive problem data (testcases, URLs) from [Competitive Companion](https://github.com/jmerle/competitive-companion) browser extension |
| Language Server | `LanguageServer` | LSP-based linting and diagnostics for C++, Java, and Python |
| WakaTime | `WakaTime` | Time tracking via [WakaTime](https://wakatime.com) |

### CSES Tool Details

The CSES CLI integration (`CSESTool`) follows the same pattern as CF Tool:

1. **Enable/Disable**: Toggle via `CSES CLI/Enable` in Preferences → Extensions → CSES CLI
2. **Path**: Set the path to the `cses-cli` executable (default: `cses-cli` on PATH) in `CSES CLI/Path`
3. **URL Parsing**: When a `cses.fi` problem URL is set (manually or via Competitive Companion), the contest and task ID are automatically parsed. Two URL patterns are supported:
   - `cses.fi/{contest}/task/{taskId}` (standard and contest tasks)
   - `cses.fi/{contest}/{subcategory}/{taskId}` (multi-level category structures)
4. **Submission**: A "Submit to CSES" button appears in the compile/run toolbar. Clicking it saves the current code to a temp file and submits via `cses-cli submit -c {contest} -t {taskId} {file}`
5. **Fallback parsing**: If URL parsing fails, CSES identifiers can be extracted from head comments (e.g., `Contest: 647`, `Problem: A`) or `cses.fi` URLs in comments
6. **Toast messages**: Submission verdicts are shown as toast notifications (reuses the CF Tool toast setting)