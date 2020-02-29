## UNRELEASED

### Added

- You can show/hide each testcase, and only run the non-hidden testcases.
- Add a drop-down menu for each testcase. You can delete a testcase / run a single testcase from it.
- Add a drop-down menu for testcases. You can remove empty, remove all, hide AC, show all, hide all, inverse hide/show from it.
- Add "Set Codeforces URL" in the right-click menu of tabs. You can enter the contest ID and problem code instead of the whole URL.
- Warn when submitting to CF and the problem code is 0.
- A system-wide tray icon.
- Windows icon for all OSs.

### Changed

- The "Hide AC" button is removed. (But a similiar action is added in the "More" menu of testcases.)
- Submitting to CF is enhanced. Now empty messages are not shown, some wrong verdicts are fixed, toast messages are shown only once per run. Some small bugs are also fixed.
- Now there's only one Diff Viewer per testcase. The output and expected will update automatically after each run. When pressing the diff button, the output and expected will update and the Diff Viewer will show and raise.

### Fixed

- Show line-breaks without "¶" in the Diff Viewer when "Display new-line characters in the Diff Viewer" option is not checked. (#181)
- Fix window size issue on Windows if the transparency is 100 and the window was maximized in the last session.

## v6.0

It's the first version after the versioning rules changed, so there's no change log before v6.0.
